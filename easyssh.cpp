//EasySSH: SSH Server Selector
//Written by Parker Hao Tian @ Beautiful British Columbia
//Sep 18 2016 v1.1
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Server
{
	string name;
	string command;
};
Server add_server(bool first){
	string temp;
	Server new_server;
	ofstream list("list_of_servers.txt", ios::app);
	cout << "Please enter the name of server: ";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	getline(cin, temp);
	if(!first)
		list << endl;
	list << temp << endl;
	new_server.name = temp;
	cout << "Please enter the SSH command used to access this server: ";
	getline(cin, temp);
	list << temp;
	new_server.command = temp;
	cout << "Server added!" << endl;
	list.close();
	return new_server;
}
void remove_server(vector<Server> li, int to_be_removed){
	ofstream new_list("list_of_servers.txt");
	bool first = true;
	int i = 0;
	while (i < to_be_removed){
		if(first){
			new_list << li[i].name << endl << li[i].command;
			first = false;
		}
		else{
			new_list << endl << li[i].name << endl << li[i].command;
		}
		i++;
	}
	i++;
	while (i < li.size()){
		if(first){
			new_list << li[i].name << endl << li[i].command;
			first = false;
		}
		else{
			new_list << endl << li[i].name << endl << li[i].command;
		}
		i++;
	}
	new_list.close();
}
char* port_forwarding(string& command){
	char* new_command = new char[command.size() + 11];
	int i = 0;
	char diff_com[] = "ssh -N -D 7070 ";
	while(i < 15){
		new_command[i] = diff_com[i];
		i++;
	}
	while(i < command.size()+11){
		new_command[i] = command[i - 11];
		i++;
	}
	new_command[i] = '\0';
	return new_command;
}
	
	
int main(){
	vector<Server> it;
	Server ser;
	cout << "EasySSH v1.1" << endl;
	ifstream list("list_of_servers.txt");
	if(!list || list.peek() == std::ifstream::traits_type::eof()){
		cout << "No server storaged in the system. Do you want to add a new one?(Y/N)"<< endl;
		char answer;
		cin >> answer;
		if(answer != 'y' && answer != 'Y'){
			cout << "Thank you for using!" << endl;
			return 0;
		}
		list.close();
		add_server(true);
		list.open("list_of_servers.txt");
	}
	while(list.good()){
		getline(list, ser.name);
		getline(list, ser.command);
		it.push_back(ser);
	}
	list.close();
	cout << "Please select from the following options:" << endl;
	char option;
	int option_in_num;
	while(true){
		int i = 0;
		while(i < it.size()){
			cout << "\t" << i+1 << ". " << it[i].name << endl;
			i++;
		}
		cout << "\t" << "a. Add a new server..." << endl;
		cout << "\t" << "r. Remove a server..." << endl;
		cout << "\t" << "p. Connect via port forwarding mode(Proxy)..." << endl;
		cout << "\t" << "q. Quit" << endl;
		cout << ":";
		cin >> option;
		option_in_num = static_cast<int>(option) - 48;
		if(option == 'a'){
			cout <<'a' << endl;
			it.push_back(add_server(false));
		}
		else if(option == 'r'){
			cout << 'r' << endl;
			int option2;
			cout << "Which of the servers above would you like to remove?" << endl;
			cout << ":";
			cin >> option2;
			remove_server(it, option2 - 1);
			cout << "Removed. Now please open this program again." << endl;
			break;
		}
		else if(option == 'p'){
			cout << "Entering port forwarding mode..." << endl;
			cout << "Please set the proxy of your browser as 127.0.0.1, port 7070. When you are finished, please use Control + C to exit!" << endl;
			cout << "Select from one of the servers above" << endl << ":";
			int option2;
			cin >> option2;
			system(port_forwarding(it[option2 - 1].command));
			cout << endl;
			break;
		}
		else if(option == 'q')
			break;
		else if(option_in_num <= it.size()){
			system(it[option_in_num - 1].command.c_str());
			break;
		}
		else{
			cout << "Wrong input! Abort" << endl;
			exit(1);
		}
	}
	cout << "Thank you for using!" << endl;
	return 0;
}