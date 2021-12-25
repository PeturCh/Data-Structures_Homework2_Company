#pragma once
#include <sstream>
#include <string>
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
        {
        }
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
        {
            //std::cout<<"tuka li ";
            //std::cout<<key<<std::endl;
            return nullptr;
        }
        if (curr->data == key)
        {
            //std::cout<<curr->data<<std::endl;
            return curr;
        }

        //std::cout<<"za: "<<curr->data<<std::endl;
         
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
        //std::cout<<"printCalled\n";
        if(!n)
            return "";

        auto child = n->child;
        while(child)
        {
            //std::cout<<"za "<<n->data<<std::endl;
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
        string clearedData = data;
        clearedData.erase(std::remove(clearedData.begin(), clearedData.end(), ' '), clearedData.end());
        std::stringstream ss;
        ss << clearedData;
        while (!ss.eof())
        {
            string line;
            std::getline(ss, line);
            if (line.find('-') != string::npos)
            {
                //std::cout<<"124\n";
                size_t pos = line.find('-');
                string fatherName, childName;
                fatherName = line.substr(0, pos);
                childName = line.substr(pos + 1, 100);
                //std::cout<<"realno e "<<fatherName << "-" <<childName<<std::endl;
                //std::getline(ss, fatherName, '-');
                //std::getline(ss, childName);

                if (!root && fatherName == "Uspeshnia")
                {
                    //std::cout<<"134\n";
                    root = new node("Uspeshnia", nullptr, root);
                    root->child = new node(childName, nullptr, root->child);
                    size += 2;
                    continue;
                }

                if (find(fatherName))
                {
                    //std::cout<<"142\n";
                    //if(fatherName == "Slavi")
                    //    std::cout<<print();

                    auto father = getNode(root, fatherName);
                    //if(!father)
                    //{
                    //    std::cout<<"NULLPTR\n";
                    //    std::cout<<fatherName;
                    //}
                    //std::cout<<father->data<<std::endl;
                    //std::cout<<"ALOOOOO1\n";
                    //if(father->child)
                    //    std::cout<<father->child->data<<std::endl;
                    node *oldChild = father->child;
                    //std::cout<<"ALOOOOO2\n";
                    oldChild = new node(childName, nullptr, oldChild);
                    //std::cout<<"ALOOOOO3\n";
                    father->child = oldChild;
                    size++;
                    //std::cout<<"ALOOOOO4\n";
                    //std::cout<<print();




                    //std::cout<<(oldChild==nullptr)<<std::endl;
                    //if(oldChild)
                    //    std::cout<<oldChild->data<<std::endl;
                }
                else throw std::invalid_argument("No such father!\n");
            }
            else if(line != "")
            {
                //std::cout<<"150\n";
                throw std::invalid_argument("shablaba\n");
            }
            //std::cout<<"ebasimamata\n";
        }
    }

    ~Hierarchy() noexcept
    {
        clear(root);
    }

    void operator=(const Hierarchy &) = delete;

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

    void incorporate()
    {
        return;
    }
    void modernize()
    {
        return;
    }

    Hierarchy join(const Hierarchy &right) const
    {
        return right;
    }

    //If you need it - add more public methods here
};