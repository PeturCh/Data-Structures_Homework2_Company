#pragma once
//#include <sstream>
#include <string>
#include <queue>
#include <exception>

using std::string;
class Hierarchy
{
private:
    size_t size = 0;
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

    std::vector<std::string> getLine(const std::string &s)
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
                            std::cout<<"majka ti deiba\n";
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

    string print(node *n) const
    {
        string result;
        if(!n)
            return "";

        auto child = n->child;
        while(child)
        {
            result += n->data;
            result.push_back('-');
            result += child->data;
            result.push_back('\n');
            child = child->brother;
        }

        return result + print(n->brother) + print(n->child);

    }

    int num_employees(node *r) const
    {
        if(!r)
            return 0;
        return 1 + num_employees(r->brother) + num_employees(r->child);
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

    const std::vector<node*> who_to_modernize() const
    {
        std::vector<node*> modernizeNodes;
        std::queue<node*> queue;
        int level = 1;
 
        queue.push(root);
 
        // extra NULL is pushed to keep track
        // of all the nodes to be pushed before
        // level is incremented by 1
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
        std::swap(this->size, size);
    }

    Hierarchy(const Hierarchy &r)
    {
        root = copy(r.root);
        size = r.size;
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
                    root->child = new node(childName, nullptr, root->child);
                    size += 2;
                    continue;
                }

                if (find(fatherName))
                {
                    auto father = getNode(root, fatherName);
                    node *oldChild = father->child;
                    oldChild = new node(childName, nullptr, oldChild);
                    father->child = oldChild;
                    size++;
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

    void operator=(const Hierarchy &h)
    {
        if (&h != this) {
            clear(root);
            root = copy(h.root);
        }
    }

    string print() const
    {
        return print(root);
    }

    int longest_chain() const
    {
        return 0;
    }
   
    bool find(const string &name) const
    {
        return find(root, name);
    }

    int num_employees() const
    {
        //return size;
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
        return getFather(root, root, name)->data;
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
        //std::cout<<"\n--------------------------------------\n";
        auto curr = getNode(root, who);
        if(!curr)
        {
            //std::cout<<"what : "<< who <<std::endl;
            return -1;
        }
        unsigned long salary = num_subordinates(curr) * 500;
        auto subordinate = curr->child;
        while(subordinate)
        {
            if(subordinate->child)
                salary += (num_employees(subordinate->child)) * 50;
            subordinate = subordinate->brother;
        }
        //std::cout<<who<<" "<<salary<<std::endl;
        //std::cout<<print();
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

        else
        {
            if(father->child->data == who)
            {
                father->child = toFire->brother;
                auto current = toFire->brother;
                while(current->brother)
                {
                    current = current->brother;
                }
                current->brother = toFire->child;
                delete toFire;
                return true;
            }
            else
            {
                auto current = father->child;
                while(current->brother->data != who)
                {
                    current = current->brother;
                }
                current->brother = current->brother->brother;

                while(current->brother)
                {
                    current = current->brother;
                }
                current->brother = toFire->child;
                delete toFire;
                return true;
            }
        }
        return false;
    }

    bool hire(const string &who, const string &boss)
    {
        auto newBoss = getNode(root, boss);
        if(!newBoss || who == "Uspeshnia")
            return false;

        if(find(who))
        {
            auto employee = getNode(root, who);
            auto oldBoss = getFather(root, root, who);

            if(oldBoss->child->data == who)
            {
                oldBoss->child = employee->brother;
                newBoss->child = new node(employee->data, employee->child, newBoss->child);
                return true;
            }
            else
            {
                auto current = oldBoss->child;
                while(current->brother->data != who)
                {
                    current = current->brother;
                }
                current->brother = current->brother->brother;
                newBoss->child = new node(employee->data, employee->child, newBoss->child);
                return true;
            }
        }
    
        else
        {
            auto newBoss = getNode(root, boss);
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
        //auto curr = root->child;
        //std::queue<node*> nodes;
        //nodes.push(curr);
        //while(curr->brother)
        //{
        //    nodes.push(curr->brother);
        //    curr = curr->brother;
        //}
        //curr = nodes.front();







        //std::vector<string> visited;
        //std::stack<node*> nodes;
        //Hierarchy h(*this);
        //auto curr = h.root->child;
        //nodes.push(curr);
        //while(curr)
        //{
        //    if(curr->brother)
        //    {
        //        nodes.push(curr->brother);
        //        curr = curr->brother;
        //        continue;
        //    }
        //    if(curr->child)
        //    {
        //        nodes.push(curr->child);
        //        curr = curr->child;
        //        continue;
        //    }
        //    visited.push_back(curr->data);
        //    nodes.pop();
        //    curr = nodes.top();
        //    
        //}

    }

    void modernize()
    {
        std::vector<node*> employees = who_to_modernize();
        for (auto &&employee : employees)
            fire(employee->data);
    }

    Hierarchy join(const Hierarchy &right) const
    {
        //if(!checkJoin(right))

        
    }

    //If you need it - add more public methods here
};