
#define CLIENTDB_H
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <istream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <ctime>
using namespace std;
class client
{
    unsigned int ID; //Not used
    bool Set=false;
    string name;
    string surname;
    vector <unsigned int> rentedIDs;
    vector <unsigned int> rentHistory;
public:
    bool isSet()
    {
        return Set;
    }
    void printClientData()
    {
        if(Set||1)cout<<"ID:"<<ID<<endl<<"Name:"<<name<<endl<<"Surname:"<<surname<<endl;
        ofstream log;
        log.open(".\\Logs\\ClientsOp.txt", ios::app);
        char time[19];
        std::time_t t = std::time(nullptr);
        strftime(time,19,"%Y:%m:%d %T",gmtime(&t));
        log<<"[OPR] Display client data for:"<<ID<<endl;
        log.close();
    }
    string clientName()
    {
        return name;
    }
    string clientSurname()
    {
        return surname;
    }
    string clientIDstring()
    {
        stringstream ss;
        ss<<ID;
        string output=ss.str();
        return output;
    }
    unsigned int clientID()
    {
        return ID;
    }
    vector <unsigned int>clientRentedIDs()
    {
        return rentedIDs;
    }
    int numberOfRented()
    {
        return rentedIDs.size();
    }
    unsigned int clientRentedID(int index)
    {
        if((unsigned int)index>=rentedIDs.size())return 0;
        return rentedIDs[index];
    }
    void setClient(string inName, string inSurname, unsigned int inID)
    {
        name=inName;
        surname=inSurname;
        ID=inID;
        Set=true;
    }
    bool dumpClientToFile(string folderPath)
    {
        stringstream filePath;
        filePath<<folderPath<<"Clients\\"<<ID<<".txt";
        ofstream outputFile;
        outputFile.open(filePath.str(),ios::trunc);
        if(!outputFile.good())
        {
            ofstream log;
            log.open(".\\Logs\\ClientsOp.txt", ios::app);
            log<<"Failed to dump client:"<<ID<<endl<<"  Name:"<<name<<endl<<"  Surname:"<<surname<<endl;
            log.close();
            outputFile.close();
            return false;
        }else
        {
            ofstream log;
            log.open(".\\Logs\\ClientsOp.txt", ios::app);
            log<<"Dumped client:"<<ID<<endl<<"  Name:"<<name<<endl<<"  Surname:"<<surname<<endl;
            log.close();
        }
        outputFile<<ID<<endl<<name<<endl<<surname<<endl<<rentedIDs.size()<<endl;
        for(unsigned int i=0;i<rentedIDs.size();i++)outputFile<<rentedIDs[i]<<endl;
        outputFile<<rentHistory.size()<<endl;
        for(unsigned int i=0;i<rentHistory.size();i++)outputFile<<rentHistory[i]<<endl;
        return true;
    }
    bool loadClientFromFile(string folderPath,unsigned int newID)
    {
        stringstream filePath;
        filePath<<folderPath<<"Clients\\"<<newID<<".txt";
        fstream inputFile;
        inputFile.open(filePath.str(),ios::in);
        if(!inputFile.good())
        {
            ofstream log;
            log.open(".\\Logs\\ClientsOp.txt", ios::app);
            log<<"Failed to find client:"<<ID<<endl;
            log.close();
            inputFile.close();
            return false;
        }else
        {
            int numberOf;
            unsigned int in;
            inputFile>>ID>>name>>surname>>numberOf;
            ofstream log;
            log.open(".\\Logs\\ClientsOp.txt", ios::app);
            log<<"Loaded client:"<<ID<<endl<<"  Name:"<<name<<endl<<"  Surname:"<<surname<<endl;
            log.close();
            int i=0;
            while(i<numberOf)
            {
                inputFile>>in;
                rentedIDs.push_back(in);
                i--;
            }
            i=0;
            inputFile>>numberOf;
            while(i<numberOf)
            {
                inputFile>>in;
                rentHistory.push_back(in);
                i--;
            }
        }
        Set=true;
        return true;
    }

    /*client(string inName,string inSurname,unsigned int inID)
    {
        name=inName;
        surname=inSurname;
        ID=inID;
        //for(unsigned int i=0;i<name.length();i++)ID+=256*i*(unsigned int)name[i];
        // for(unsigned int i=0;i<surname.length();i++)ID+=256*i*(unsigned int)surname[i];
        ofstream log;
        log.open("./Logs/ClientsOp.txt", ios::app);
        log<<"New client:"<<ID<<endl<<"  Name:"<<name<<endl<<"  Surname:"<<surname<<endl;
        log.close();
    }*/
};
class clientDB
{
    list<unsigned int> unusedIDs;
   // unsigned int lastID=0;
    vector<client> clients;
public:
    bool loadClientsFromFolder(string folderPath)
    {
        fstream inputFile;
        stringstream filePath;
        filePath<<folderPath<<"Clients\\ListOfClients.txt";
        inputFile.open(filePath.str(),ios::in);
        if(!inputFile.good())
        {
            ofstream log;
            log.open(".\\Logs\\dbOps.txt", ios::app);
            log<<"Failed to open File:"<<filePath.str()<<endl;
            log.close();
            return false;
        }
        ofstream log;
        unsigned int prevID=0;
        if(inputFile.eof())
        {
            log<<"Empty file:"<<filePath.str()<<endl;
            log.close();
            inputFile.close();
            return false;
        }
        log<<"Adding clients from:"<<folderPath<<"Clients\\"<<endl;
        while(1)
        {
            unsigned int newID;
            inputFile>>newID;
            if(inputFile.eof())break;
            cout<<"nID:"<<newID<<endl;
            EmptyClient:
                client inClient;
                if(newID==prevID+1||!newID)
                {
                    prevID=newID;
                    if(inClient.loadClientFromFile(folderPath,newID))
                        log<<"  Loaded client:"<<newID<<endl;
                        else log<<"  Failed to load Client:"<<newID;
                    clients.push_back(inClient);
                }else
                {
                    unusedIDs.push_back(++prevID);
                    clients.push_back(inClient);
                    goto EmptyClient;
                }

        }
        log.close();
        return true;
    }
    bool addClient(string name, string surname)
    {
        client inClient;
//        cout << "UNUSEDIDS: ";
//        for(int i = 0; i < unusedIDs.size(); i++)
//        {
//            cout << unusedIDs.front()+i << " ";
//        }
//        cout << endl;
        if(unusedIDs.size())
        {
            inClient.setClient(name,surname,unusedIDs.front());
            clients[unusedIDs.front()].setClient(name,surname,unusedIDs.front());
            unusedIDs.pop_front();
        }else
        {
            inClient.setClient(name,surname,clients.size());
            clients.push_back(inClient);
        }
       // lastID=clients.size();
        return true;
    }
    bool dumpDBtoFile(string folderPath)
    {
        ofstream log;
        log.open(".\\Logs\\dbOps.txt", ios::app);

        stringstream filePath;
        filePath<<folderPath<<"Clients\\ListOfClients.txt";
        fstream outputFile(filePath.str(),ios::out);
        if(!outputFile.good())
        {
            ofstream log;
            log.open(".\\Logs\\dbOps.txt", ios::app);
            log<<"Failed to dump DB to File:"<<filePath.str()<<endl;
            log.close();
            return false;
        }
        log<<"Dumping clients:"<<endl;
        for(unsigned int i=0;i<clients.size();i++)
        {
            outputFile<<clients[i].clientID()<<endl;
            log<<"  "<<clients[i].clientID();
            if(clients[i].dumpClientToFile(folderPath))log<<" succes"<<endl;else log<<" failure"<<endl;
        }
        outputFile.close();
        log.close();
        return true;
    }
    unsigned int size(void)
    {
        return clients.size();
    }
    void  clientData(unsigned int i)
    {
        clients[i].printClientData();
    }
};
// CLIENTDB_H
