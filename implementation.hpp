#pragma once
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <exception>

using std::string;
class Hierarchy
{
private:
    struct node
    {
        std::string data{};
        node *child{}, *brother{};

        node() {}

        node(const std::string &data, node *child = nullptr, node *brother = nullptr)
            : data(data), child(child), brother(brother)
        {}
    } *root{};

    node *copy(node *r)
    {
        return r ? new node(r->data, copy(r->child), copy(r->brother)) : r;
    }

    void clear(node *r)
    {
        if (r)
        {
            clear(r->child);
            clear(r->brother);
            delete r;
        }
    }

    std::vector<std::string> getLine(const std::string &s) const
    {
        std::vector<std::string> result;
        std::string line;
        for (size_t i = 0; i < s.size(); i++)
        {
            if(s[i] != '\n')
                line.push_back(s[i]);
            else 
            {
                result.push_back(line);
                line = "";
            }
        }

        if(line != "")
            result.push_back(line);
        return result;
    }

    bool validateName(const std::string &name)
    {
        if(name == "" || name.find('-') != string::npos || allCharsSpaces(name)) 
            return false;

        for (size_t i = 0; i < name.length(); i++) //if there is space in tha name
            if(name[i] != ' ')
            {
                string afterSpace = name.substr(i);
                    for (size_t j = afterSpace.size() - 1; j >= 0; j--)
                    {
                        if(afterSpace[j] == ' ')
                            afterSpace.pop_back();
                        else if(afterSpace.find(' ') != string::npos)
                        {
                            return false;
                        }
                        else return true;
                    }
            }

        return true;
    }

    bool allCharsSpaces(const std::string &s) const
    {
        int n = s.length();
        for (int i = 0; i < n; i++)
            if (s[i] != ' ')
                return false;
 
        return true;
    }

    bool checkInputData(const std::string &s)
    {
        if(s == "")
            return true;

        std::vector<std::string> lines = getLine(s);
        if(lines.empty())
            return false;
        for (auto &&l : lines)
        {
            if (l.find('-') != string::npos)
            {
                size_t pos = l.find('-');
                string fatherName, childName;
                fatherName = l.substr(0, pos);
                childName = l.substr(pos + 1);
                if(!validateName(fatherName) || !validateName(childName))
                    return false;
            }
            else return false;
        }
        return true;
    }

    size_t brothersCount(node *n) const
    {
        if(!n)
            return 0;
        if(n->brother)
            return 1 + brothersCount(n->brother);
        return 0;
    }

    bool find(const node *r, const std::string &key) const
    {
        if (!r)
            return false;
        if (r->data == key)
            return true;
        return find(r->brother, key) || find(r->child, key);
    }

    int getLevel(const node* r, const string &name) const
    {
        if (!r) return 0;
        size_t level = 1;
        std::queue<const node*> nodes;
        nodes.push(r);
        nodes.push(nullptr);
        while(true)
        {
            const node* current = nodes.front();
            nodes.pop();

            if (!current) 
            {
                level++;
                if (nodes.empty()) return level;
                nodes.push(nullptr);
            }
            else 
            {
                if(current->data == name)
                    return level;
                for (const node* it = current->child; it; it = it->brother) 
                {
                    nodes.push(it);
                }
            }
        }
        return level;
    }

    node* getBiggerManager(const Hierarchy &h, node *manager, node *managerRight) const
    {
        if(managerRight->data == manager->data)
            return managerRight;

        if(getLevel(h.root, managerRight->data) < getLevel(root, manager->data))
            return managerRight;
        else if(getLevel(h.root, managerRight->data) > getLevel(root, manager->data))
            return manager;
        else if(manager->data < managerRight->data)
            return manager;
        else return managerRight;

    }

    node* highestPaid(node* curr) const 
    {
        node* result = curr;
        size_t highestSalary = getSalary(curr->data);
 
        node* temp = curr->brother;
        while (temp)
        {
            unsigned long tempSalary = getSalary(temp->data);
            if (tempSalary > highestSalary)
            {
                highestSalary = tempSalary;
                result = temp;
            }
            temp = temp->brother;
        }
        return result;
    }

    int longest_chain(node *r) const
    {
        if (!r) 
            return 0;
        return std::max(1 + longest_chain(r->child), longest_chain(r->brother));
    }

    node *getNode(node *curr, const std::string &key) const
    {
        if (!curr)
            return nullptr;

        if (curr->data == key)
            return curr;

        auto temp = getNode(curr->brother, key);
        return temp ? temp : getNode(curr->child, key);
    }

    node* getFather(node *prev, node* curr, const string &name) const
    {
        if (!curr)
            return nullptr;
        if (curr->data == name)
            return prev;
        
        auto temp = getFather(prev, curr->brother, name);
        return temp ? getFather(prev, curr->brother, name) : getFather(curr, curr->child, name);
    }

    std::vector<string> printSorted(node * r) const
    {
        std::vector<string> result;
        std::vector<string> level;
        if (!r) return result;
        std::queue<const node*> front;
        front.push(r);
        front.push(nullptr);
        for (;;) 
        {
            const node* current = front.front();
            front.pop();
            if (!current) 
            {
                for (size_t i = 0; i < level.size(); i++)
                {
                    for (size_t j = i + 1; j < level.size(); j++)
                    {
                        if(level[j] < level[i])
                        {
                            string temp = level[j];
                            level[j] = level[i];
                            level[i] = temp;
                        }
                    }
                }
                for (size_t i = 0; i < level.size(); i++)
                {
                    result.push_back(level[i]);
                }
                level.erase(level.begin(), level.end());
                
                if (front.empty()) return result;
                front.push(nullptr);
            }
            else 
            {
                string line;
                for (const node* it = current->child; it; it = it->brother) 
                {
                    line += current->data;
                    line.push_back('-');
                    line += it->data;
                    line.push_back('\n');
                    level.push_back(line);
                    line = "";
                    front.push(it);
                }
            }
        }
        return result;
    }

    int num_employees(node *r) const
    {
        if(!r)
            return 0;
        return 1 + num_employees(r->brother) + num_employees(r->child);
    }

    std::stack<node*> nodesForInc() const
    {
        std::stack<node*> nodes;
        std::queue<node*> q;

        q.push(root->child);
        while(!q.empty())
        {
            node* temp = q.front();
            q.pop();

            if(temp->brother)
            {
                node *highestPaidEmp = highestPaid(temp);
                nodes.push(highestPaidEmp);
            }

            if(temp->child)
                if(temp->child->brother)
                    q.push(temp->child);

            if(temp->brother)
            {
                node *curr = temp->brother;
                while(curr)
                {
                    if(curr->child)
                        if(curr->child->brother)
                            q.push(curr->child);

                    curr = curr->brother;
                }   
            }
        }
        return nodes;
    }

    int num_overloaded(int level, node *r) const
    {
        if (!r)
            return 0;
        
        if(!r->child)
            return 0;

        if(num_employees(r->child) > level)
            return 1 + num_overloaded(level, r->brother) + num_overloaded(level, r->child);
        else return num_overloaded(level, r->brother) + num_overloaded(level, r->child);
    }

    std::vector<node*> getAll(node *r) const
    {
        std::vector<node*> result;
        if(r->child)
        {
            result = getAll(r->child);
        }
        result.push_back(r);
        if(r->brother)
        {
            auto getAllBrother = getAll(r->brother);
            result.insert(result.end(), getAllBrother.begin(), getAllBrother.end());
        }
        return result;
    }

    std::vector<node*> getFathers(node *r, const Hierarchy &h) const
    {
        std::vector<node*> result;
        while(true)
        {
            r = getFather(h.root, h.root, r->data);
            if(r->data == "Uspeshnia" || !r)
                break;
            result.push_back(r);
        }
        return result;
    }

    bool checkJoin(const Hierarchy &right) const
    {
        std::vector<node*> rightNodes = getAll(right.root->child);
        for (auto &&n : rightNodes)
        {
            auto nodeInLeft = getNode(root, n->data);
            if(nodeInLeft)
            {
                std::vector<node*> managers = getFathers(n, right);
                for (auto &&manager : managers)
                {
                    if(find(nodeInLeft->child, manager->data))
                        return false;
                }
                
                managers = getFathers(nodeInLeft, *this);
                for (auto &&manager : managers)
                {
                    if(find(n->child, manager->data))
                        return false;
                }
            }
        }
        return true;
    }

    const std::vector<node*> nodesForModernization() const
    {
        std::vector<node*> modernizeNodes;
        std::queue<node*> queue;
        int level = 1;
 
        queue.push(root);
        queue.push(nullptr);
        while (!queue.empty()) 
        {
            node* temp = queue.front();
            queue.pop();
            if (temp == nullptr) 
            {
                if (queue.empty()) 
                    break;
 
                if (queue.front() != nullptr)
                    queue.push(nullptr);
                    
                level += 1;
            }
            else {
                if (level % 2 == 0 && temp->child != nullptr)
                    modernizeNodes.push_back(temp);
 
                if (temp->child)
                    queue.push(temp->child);
 
                if (temp->brother) 
                {
                    node* helper = temp->brother;
                    while (helper)
                    {
                        if (level % 2 == 0 && helper->child != nullptr)
                            modernizeNodes.push_back(helper);

                        if (helper->child != nullptr)
                            queue.push(helper->child);
 
                        helper = helper->brother;
                    }
                }             
            }
        }
        return modernizeNodes;
    }

public:

    Hierarchy(Hierarchy &&r) noexcept
    {
        std::swap(this->root, r.root);
    }

    Hierarchy(const Hierarchy &r)
    {
        root = copy(r.root);
    }

    Hierarchy(const string &data)
    {
        if(!checkInputData(data))
            throw std::invalid_argument("Not valid data given!\n");

        string clearedData = data;
        clearedData.erase(std::remove(clearedData.begin(), clearedData.end(), ' '), clearedData.end());
        std::vector<string> lines = getLine(clearedData);
        for (auto &&line : lines)
        {
            if (line.find('-') != string::npos)
            {
                size_t pos = line.find('-');
                string fatherName, childName;
                fatherName = line.substr(0, pos);
                childName = line.substr(pos + 1, 100);             
                if (!root && fatherName == "Uspeshnia")
                {
                    root = new node("Uspeshnia", nullptr, root);
                    root->child = new node(childName, nullptr, nullptr);
                    continue;
                }

                if (auto father = getNode(root, fatherName))
                {
                    auto currChild = father->child;
                    if(!currChild)
                        father->child = new node(childName, nullptr, nullptr);
                    else
                    {
                        while(currChild->brother)
                            currChild = currChild->brother;

                        currChild->brother = new node(childName, nullptr, nullptr);
                    }
                }
                else throw std::invalid_argument("No such father!\n");
            }
            else if(line != "")
            {
                throw std::invalid_argument("shablaba\n");
            }
        }
    }

    ~Hierarchy() noexcept
    {
        clear(root);
    }

    void operator=(const Hierarchy &h) = delete;

    string print() const
    {
        string result = "";
        std::vector<string> lines = printSorted(root);
        for (auto &&line : lines)
            result += line;

        return result;
        
    }

    int longest_chain() const
    {
        return longest_chain(root);
    }
   
    bool find(const string &name) const
    {
        return find(root, name);
    }

    int num_employees() const
    {
        return num_employees(root);
    }
    
    int num_overloaded(int level = 20) const
    {
        return num_overloaded(level, root);
    }

    string manager(const string &name) const
    {
        if(name == "Uspeshnia")
            return "";

        auto manager = getFather(root, root, name);
        return manager ? manager->data : "";
    }

    int num_subordinates(const string &name) const
    {
        node *father = getNode(root, name);
        if(!father)
            return -1;
        auto firstChild = father->child;
        size_t counter = 0;
        while(firstChild)
        {
            firstChild = firstChild->brother;
            counter++;
        }
        return counter;
    }

    int num_subordinates(const node* father) const
    {
        if(!father)
            return -1;
        auto firstChild = father->child;
        size_t counter = 0;
        while(firstChild)
        {
            firstChild = firstChild->brother;
            counter++;
        }
        return counter;
    }

    unsigned long getSalary(const string &who) const
    {
        auto curr = getNode(root, who);
        if(!curr)
            return -1;

        unsigned long salary = num_subordinates(curr) * 500;
        auto subordinate = curr->child;
        while(subordinate)
        {
            if(subordinate->child)
                salary += (num_employees(subordinate->child)) * 50;
            subordinate = subordinate->brother;
        }
        return salary;
    }

    bool fire(const string &who)
    {
        auto toFire = getNode(root, who);
        if(!toFire || who == "Uspeshnia")
            return false;
        auto father = getFather(root, root, who);

        if(num_subordinates(father) == 1)
        {
            father->child = toFire->child;
            delete toFire;
            return true;
        }

        else if(father->child->data == who)
            {
                father->child = toFire->brother;
                auto current = toFire->brother;
                while(current->brother)
                    current = current->brother;

                current->brother = toFire->child;
                delete toFire;
                return true;
            }
            else
            {
                auto current = father->child;
                while(current->brother->data != who)
                    current = current->brother;

                current->brother = current->brother->brother;

                while(current->brother)
                    current = current->brother;

                current->brother = toFire->child;
                delete toFire;
                return true;
            }
        return false;
    }

    bool hire(const string &who, const string &boss)
    {
        auto newBoss = getNode(root, boss);
        if(!newBoss || who == "Uspeshnia")
            return false;

        if(auto employee = getNode(root, who))
        {
            auto oldBoss = getFather(root, root, who);

            if(oldBoss->child->data == who)
            {
                auto oldChild = oldBoss->child;
                oldBoss->child = employee->brother;
                oldChild->brother = newBoss->child;
                newBoss->child = oldChild;
                return true;
            }
            else
            {
                auto current = oldBoss->child;
                while(current->brother->data != who)
                    current = current->brother;

                auto toMove = current->brother;
                current->brother = current->brother->brother;
                toMove->brother = newBoss->child;
                newBoss->child = toMove;
                return true;
            }
        }
    
        else
        {
            newBoss->child = new node(who, nullptr, newBoss->child);
            return true;
        }
        return false;
    }

    node* getNodesToStack(std::stack<node*> &s, node *r)
    {
        if(!r)
            return nullptr;

    }

    void incorporate()
    {
        if (root == nullptr) 
            return;
    
        std::stack<node*> forIncorporation = nodesForInc();

        while (!forIncorporation.empty())
        {
            node* curr = forIncorporation.top();
            forIncorporation.pop();
    
            if (curr->child)
            {
                node* parent = getFather(root, root, curr->data);
                if (parent->child->data == curr->data){
                    node* temp = curr->child;
                    while (temp->brother)
                    {
                        temp = temp->brother;
                    }
                    temp->brother = curr->brother;
                    curr->brother = nullptr;
                }
                else {
                    node* temp = parent->child;
                    while (temp->brother->data != curr->data)
                    {
                        temp = temp->brother;
                    }
                    temp->brother = curr->brother;
    
                    if (temp->brother)
                    {
                        while (temp->brother)
                        {
                            temp = temp->brother;
                        }
                        temp->brother = curr->child;
                        curr->brother = nullptr;
                    }
                    else 
                        temp->brother = curr->child;

                    node* helper = parent->child;
                    parent->child = curr;
                    curr->child = helper;
                }
            }
            else {
                curr->child = curr->brother;
                curr->brother = nullptr;
            }
        }
    }

    void modernize()
    {
        std::vector<node*> employees = nodesForModernization();
        for (auto &&employee : employees)
            fire(employee->data);
    }

    Hierarchy join(const Hierarchy &right) const
    {
        if(!checkJoin(right))
             throw std::invalid_argument("Can't join!\n");
        Hierarchy h(right);
        if(root == right.root)
            return h;
        
        node *curr = nullptr;
        std::queue<node*> q;
        q.push(root);
        while(!q.empty())
        {
            curr = q.front();
            q.pop();
            for(node* it = curr->child; it; it = it->brother)
            {
                if(h.find(it->data) && find(it->data))
                {
                    node* biggerManager = getBiggerManager(h, curr, h.getFather(h.root, h.root, it->data));
                    h.hire(it->data, biggerManager->data);
                }
                
                else
                    h.hire(it->data, curr->data);

                q.push(it);
            }

        }
        return h;
    }
};