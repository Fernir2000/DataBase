#include <iostream>
#include "include/clientDB.h"
#include <windows.h>
#include <unistd.h>
using namespace std;
clientDB newDB()
{
string in[5][2]={
    {"Jan",     "Nowak"},
    {"Karol",   "Wajrak"},
    {"Adam",    "Jankowski"},
    {"Zbigniew","Adamski"},
    {"Andrzej", "Wolski"}};
    clientDB clientsC;
    for(int i=0;i<5;i++)
    {
        cout<<clientsC.addClient(in[i][0],in[i][1])<<endl;
    }
    return clientsC;
}
void displayAll(clientDB inputDB)
{
    for(unsigned int i=0;i<inputDB.size();i++)
    {
        inputDB.clientData(i);
    }
}
int main()
{
    clientDB clients;
    int in;
    do
    {
        system("cls");
        cout<<"Pick action:"<<endl
        <<"  0) Stop Program"<<endl
        <<"  1) Load pre-set DB"<<endl
        <<"  2) Load DB from folder"<<endl
        <<"  3) Display DB"<<endl
        <<"  4) Add client"<<endl
        <<"  5) Dump DB to folder"<<endl;
        cin>>in;
        switch(in)
        {
        case 1:
            clients=newDB();
            break;
        case 2:
            {
                cout<<"Folder path:";
                string folderPath;
                cin>>folderPath;
                clients.loadClientsFromFolder(folderPath);
            }
        case 3:
            displayAll(clients);
            Sleep(5000);
            break;
        case 4:
            {string name,surname;
            cout<<"Input name and surname of new client"<<endl;
            cin>>name>>surname;
            clients.addClient(name,surname);
            }
            break;
        case 5:
            cout<<"Folder path:";
            {string folderPath;
            cin>>folderPath;
            clients.dumpDBtoFile(folderPath);}
            break;
        case 0:
            break;
        default:
            break;
        }

    }while(in);
    return 0;
}
