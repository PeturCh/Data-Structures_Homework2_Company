#pragma once
#include <sstream>
#include <string>
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
        {
        }
    } *root{};

    node *copy(node *root)
    {
        return root ? new node(root->data, copy(root->child), copy(root->brother)) : root;
    }

    void clear(node *root)
    {
        if (root)
        {
            clear(root->child);
            clear(root->brother);
            delete root;
        }
    }

    bool find(const node *root, const std::string &key) const
    {
        if (!root)
            return false;
        if (root->data == key)
            return true;
        return find(root->brother, key) || find(root->child, key);
    }

    node *getNode(node *root, const std::string &key) const
    {
        if (!root)
            return nullptr;
        if (root->data == key)
            return root;
        if(root->brother)
            return getNode(root->brother, key);
        else
            return getNode(root->child, key);
    }

    node* getFather(node *prev, node* curr, const string &name) const
    {
        if (!curr)
            return nullptr;
        if (curr->data == name)
            return prev ? prev : throw std::invalid_argument("No Father!\n");
        if(curr->brother)
            return getFather(curr->brother, curr->brother, name);
        else if(curr->child)
            return getFather(curr, curr->child, name);
    }

    int num_employees(node *r) const
    {
        if (!r)
            return 0;

        if (r->brother)
            return 1 + num_employees(r->brother);

        if (r->child)
            return 1 + num_employees(r->child);

        return 1;
    }

    int num_overloaded(int level, node *r) const
    {
        if (!r)
            return 0;

        if (r->child)
        {
            if (num_employees(r->child) > level)
                return 1 + num_overloaded(level, r->child);
            else
                num_overloaded(level, r->brother);
        }

        return num_overloaded(level, r->brother);
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
                //std::getline(ss, fatherName, '-');
                //std::getline(ss, childName);

                if (!root && fatherName == "Uspeshnia")
                {
                    //std::cout<<"134\n";
                    root = new node("Uspeshnia", nullptr, root);
                    root->child = new node(childName, nullptr, root->child);
                    continue;
                }

                if (find(fatherName))
                {
                    //std::cout<<"142\n";
                    node *oldChild = getNode(root, fatherName)->child;
                    oldChild = new node(childName, nullptr, oldChild);
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
        return "";
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
        return num_employees(root);
    }
    int num_overloaded(int level = 20) const
    {
        return num_overloaded(level, root);
    }

    string manager(const string &name) const
    {
        return getFather(root, root, name)->data;
    }

    int num_subordinates(const string &name) const
    {
        node *firstChild = getNode(root, name)->child;
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
        return 500;
    }

    bool fire(const string &who)
    {
        return 0;
    }
    bool hire(const string &who, const string &boss)
    {
        return 0;
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