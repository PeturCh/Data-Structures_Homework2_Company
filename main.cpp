#include <iostream>
#include <cstring>
#include <exception>
#include <fstream>
#include "implementation.hpp"

struct Object
{
    string name;
    Hierarchy h;
    bool saved;
};

string getArgument(string& line) 
{
    int i = 0;
    string argument;
    while(line[i] == ' ' )
        i++;

    while(line[i] != '\0' && line[i] != ' ' && line[i] != '\0')
        argument += line[i++];

    line.erase(0, i);
    return argument;
}

string getCommand(string& line) 
{
    size_t i = 0;
    string command;
    while(line[i] == ' ' )
        i++;

    while(line[i] != '\n' && line[i] != ' ' && line[i] != '\0')
        command += line[i++];

    line.erase(0, i);

    return command;
}

void getHelp()
{
    std::cout
            <<"----------------------------------------------------------------------------HELP---------------------------------------------------------------------------------"
            <<"\nhelp                                               gives available commands\n"
            <<"\nload <object name> <file name>                     loads data from the file and creates tree associated with the object name.\n"
                "                                                   If there is no file name given, reads the data from the console until end of file command (ctrl+z/cntrl+d)\n"
            <<"\nsave <object name> <file name>                     saves the hierarchy of the given object in the given file\n"
                "                                                   If there is no file name, prints the the info in the console\n"
            <<"\nfind <object name> <employee>                      chechs if there is an employee with the given name in the object\n"
            <<"\num_subordinates <object name> <employee>           prints teh number of direct subordinates of the given employee in the given hierarchy\n"
            <<"\nmanager <object name> <employee>                   prints the given employee's manager name from the given hierarchy\n"
            <<"\nnum_employees <object name>                        prints the number of employees in the given hierarchy\n"
            <<"\noverloaded <object name>                           prints the number of employees which have more than 20 subordinates (direct or not)\n"
            <<"\njoin  <object1 name> <object2 name> <result name>  joins the first two given hierarchies into a hierarchy with the given result name\n"
            <<"\nfire  <object name> <employee>                     remove the given employee from the given hierarchy\n"
            <<"\nhire  <object name> <employee> <manager>           hires the employee in given object as subordinate of the given manager\n"
            <<"\nsalary  <object name> <employee>                   prints the salary of the given employee from the given hierarchy\n"
            <<"\nincorporate <object name>                          incorprates the company (the given hierarchy)\n"
            <<"\nmodernize <object name>                            modernizes the company (the given hierarchy)\n"
            <<"\nexit                                               stops the program and asks if the changes (all new or modified objects) should be saved in file\n"
            <<"----------------------------------------------------------------------------END---------------------------------------------------------------------------------";
}

void run()
{
    std::vector<Object> objects;
    string line;
    string command;
    while(true)
    {
        std::cout<<"\n>";
        std::getline(std::cin , line);
        command = getCommand(line);
        string hierarchyName = getArgument(line);

        if(command == "help")
        {
            getHelp();
            continue;
        }

        else if(command == "exit")
        {
            char answer;
            string fileName;
            for(auto &&obj : objects)
            {
                if(obj.saved == false) 
                {
                    std::cout <<obj.name <<" is changed, but not saved to file.";
                    std::cout << " Do you want to save it ? (y/n) " << std::endl;
                    do
                    {
                        std::cin >> answer;
                    } while (answer != 'y' && answer != 'n');

                    if (answer == 'y')
                    {
                        std::cout<< "Enter file name: ";
                        std::cin>>fileName;

                        std::ofstream out(fileName);
                        string hierarchyData;
                        hierarchyData = obj.h.print();
                        out << hierarchyData;
                        out.close();
                        obj.saved = true;
                        std::cout<<obj.name<<" successfully saved!\n";
                    }
                }
            }
            std::cout<<"Goodbye!\n";
            break;
        }
        
        else if(command == "load")
        {
            string lines;
            auto fileName = getArgument(line);
            if(fileName.empty())
            {   
                string consoleLine;
                while(std::getline(std::cin, consoleLine))
                {
                    lines += (consoleLine + '\n');
                    consoleLine = "";
                }
                std::cin.clear();
            }
            else 
            {
                string fileLine;
                std::ifstream file(fileName);
                if (file.good() && file.is_open()) 
                {
                    while (!file.eof()) 
                    {
                        std::getline(file, fileLine);
                        lines += (fileLine + '\n');
                        fileLine = "";
                    }
                }
                file.close();
            }
            try
            {
                Hierarchy h(lines);
                objects.push_back(Object{hierarchyName, h, false});
            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << '\n';
            }
            std::cout<<"Hierarchy successfully loaded!\n";
        }

        else if(command == "save")
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to be saved! Use <help> for more info\n";
            
            else 
            {
                bool found = false;
                auto fileName = getArgument(line);
                if(fileName.empty())
                {
                    for (auto &&obj : objects)
                    {
                        if(obj.name == hierarchyName)
                        {
                            found = true;
                            std::cout<<obj.h.print();
                            break;
                        }
                    }
                }
                else
                {
                    for (auto &&obj : objects)
                    {
                        if(obj.name == hierarchyName)
                        {
                            found = true;
                            std::ofstream out(fileName);
                            string hierarchyData;
                            hierarchyData = obj.h.print();
                            out << hierarchyData;
                            out.close();
                            obj.saved = true;
                            std::cout<<hierarchyName<<" successfully saved!\n";
                            break;
                        }
                    }
                    
                }
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
            }
        }
            
        else if(command == "find")
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                string employeeName = getArgument(line);
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        if(obj.h.find(employeeName))
                            std::cout<< employeeName << " exists in " << hierarchyName <<std::endl;
                        else std::cout<< employeeName << " doesn't exists in " << hierarchyName <<std::endl;
                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
            }
            
        }

        else if(command == "num_subordinates")
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                string employeeName = getArgument(line);
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        int num = obj.h.num_subordinates(employeeName);
                        if(num == -1)
                            std::cout<<"There isn't an employee named "<<employeeName<<" in hierarchy "<<hierarchyName<<std::endl;
                        else std::cout<<"The number of subordinates of "<<employeeName<< " is " << num <<std::endl;    
                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
            }
        }

        else if(command == "manager")
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                string employeeName = getArgument(line);

                if(employeeName == "Uspeshnia")
                    std::cout<<"Uspeshnia hasn't got a manager!\n";

                else
                {
                    for (auto &&obj : objects)
                        if(obj.name == hierarchyName)
                        {
                            found = true;
                            string manager = obj.h.manager(employeeName);
                            if(manager == "")
                                std::cout<<"There isn't such employee in the given hierarchy! Employee name: "<<employeeName<<std::endl;
                            else std::cout<<"The manager of " << employeeName << " is " <<manager<<std::endl;        
                            break;
                        }
                
                    if(!found)
                        std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
                }
            }
        }

        else if(command == "num_employees")
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        std::cout<<hierarchyName<<" has " << obj.h.num_employees() <<" employees\n";
                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
            }
        }

        else if(command == "overloaded")
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        std::cout<<hierarchyName<<" has " << obj.h.num_overloaded() <<" overloaded employees\n";
                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
            }
        }

        else if(command == "join")
        {
            if(objects.size() < 2)
                std::cout<<"There aren't enough loaded hierarchies to join! Use <help> for more info\n";
            
            else
            {
                string hierarchy2Name = getArgument(line);
                string resultName = getArgument(line);
                Hierarchy *first{};
                Hierarchy *second{};
                int resultIndex = -1;
                bool found = false;
                for (size_t i = 0; i < objects.size(); i++)
                {
                    if(objects[i].name == hierarchyName)
                        first = &objects[i].h;
                    if(objects[i].name == hierarchy2Name)
                        second = &objects[i].h;
                    if(objects[i].name == resultName)
                        resultIndex = i;
                }

                if(!first || !second)
                {
                    std::cout<<"There is no loaded hierarchy with this name: ";
                    first ? std::cout<<hierarchy2Name<<std::endl : std::cout<<hierarchyName<<std::endl;
                }
                else
                {
                    if(resultIndex != -1)
                    {   
                        //dummy object
                        objects[resultIndex].h.~Hierarchy();
                        objects[resultIndex].name = "";
                        //if there was operator =
                        //std::vector<Object>::iterator it = objects.begin() + resultIndex;
                        //objects.erase(it);
                    }
                    Hierarchy result = first->join(*second);
                    objects.push_back(Object{resultName, result, false});
                    std::cout<<resultName<< " created.\n";
                }
            }
        }   

        else if(command == "fire")
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                string employeeName = getArgument(line);
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        if(obj.h.fire(employeeName))
                            std::cout<<employeeName << " was fired.\n";
                        else 
                            std::cout<<"Couldn't fire "<<employeeName<< " he is rather Uspeshnia or doesn't exists in "<< hierarchyName<<std::endl;

                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
            }
        }

        else if(command == "hire") 
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                string employeeName = getArgument(line);
                string managerName = getArgument(line);
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        if(obj.h.hire(employeeName, managerName))
                            std::cout<<employeeName << " was hired.\n";
                        else 
                            std::cout<<"Couldn't hire "<<employeeName<< " he is rather Uspeshnia or doesn't exists in "<< hierarchyName<<std::endl;

                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
                }
        }

        else if(command == "salary") 
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                string employeeName = getArgument(line);
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        int salary;
                        salary = (int)obj.h.getSalary(employeeName);
                        if(salary != -1)
                            std::cout<<employeeName << "'s salary is " << salary << " BGN.\n";
                        else 
                            std::cout<<"There isn't employee "<< employeeName<< " in "<< hierarchyName<<std::endl;

                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
            }
        }

        else if(command == "incorporate")  
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        obj.h.incorporate();
                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
                else std::cout<<hierarchyName<< " incorporated.\n";
            }
        }

        else if(command == "modernize")  
        {
            if(objects.empty())
                std::cout<<"There aren't any loaded hierarchies to search for! Use <help> for more info\n";
            
            else
            {
                bool found = false;
                for (auto &&obj : objects)
                    if(obj.name == hierarchyName)
                    {
                        found = true;
                        obj.h.modernize();
                        break;
                    }
                
                if(!found)
                    std::cout<<"There is no loaded hierarchy with this name: "<< hierarchyName <<std::endl;
                else std::cout<<hierarchyName<< " modernized.\n";
            }
        }
        
    }
}

int main()
{
    run();
    return 0;
}