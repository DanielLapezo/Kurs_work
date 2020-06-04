#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
#pragma warning(disable: 4996)
SOCKET newConnection;
#define BUFF 512
#define k 3

class Cipher {
public:
	std::string encryption(std::string original) {
		std::string TC;
		for (int i = 0; i < (original.length()); i++) {
			TC = TC + (char)((original[i] + k));
		}
		return TC;
	}
	std::string decryption(std::string encOriginal) {
		std::string UnTC;
		for (int i = 0; i < (encOriginal.length()); i++) {
			UnTC = UnTC + (char)((encOriginal[i] - k));
		}
		return UnTC;
	}

};

class FileWork {
private:
	vector<string> clients;
	vector<string> orders;
	Cipher cipher;
public:
	void send_clients() {
		ifstream clients("Clients.txt");
		char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
		while (!clients.eof()) {
			clients >> login >> pass >> name >> surname;
			strncpy(login, cipher.decryption(login).c_str(), sizeof(login));
			strncpy(pass, cipher.decryption(pass).c_str(), sizeof(pass));
			this->clients.push_back(string(login) + " " + string(pass) + " " + string(name) + " " + string(surname));
		}
		clients.close();
		char countClients[BUFF];
		itoa(this->clients.size(), countClients, 10);
		send(newConnection, countClients, sizeof(countClients), NULL);
		vector<string>::iterator it;
		for (it = this->clients.begin(); it != this->clients.end(); it++) {
			char clientInfo[BUFF];
			strncpy(clientInfo, it->c_str(), sizeof(clientInfo));
			send(newConnection, clientInfo, sizeof(clientInfo), NULL);
		}
		this->clients.clear();
	}
	void send_orders() {
		string port;
		ifstream filePorts("Orders.txt");
		while (getline(filePorts, port)) {
			orders.push_back(port);
		}
		char portCount[BUFF];
		itoa(orders.size(), portCount, 10);
		send(newConnection, portCount, sizeof(portCount), NULL);
		vector<string>::iterator it;
		for (it = this->orders.begin(); it != this->orders.end(); it++) {
			char portInfo[BUFF];
			strncpy(portInfo, it->c_str(), sizeof(portInfo));
			send(newConnection, portInfo, sizeof(portInfo), NULL);
		}
		this->orders.clear();
	}
	void recieve_clients() {
		char countClient[BUFF];
		recv(newConnection, countClient, sizeof(countClient), NULL);
		if (atoi(countClient) == 0)
		{
			ofstream clients("Clients.txt");
			clients.close();
			return;
		}
		ofstream clients("Clients.txt");
		char clientInfo[BUFF];
		recv(newConnection, clientInfo, sizeof(clientInfo), NULL);
		char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
		strncpy(login, strtok(clientInfo, " "), sizeof(login));
		strncpy(pass, strtok(NULL, " "), sizeof(pass));
		strncpy(name, strtok(NULL, " "), sizeof(name));
		strncpy(surname, strtok(NULL, " "), sizeof(surname));
		clients << cipher.encryption(login) << " " << cipher.encryption(pass) << " " << name << " " << surname;
		for (int i = 1; i < atoi(countClient); i++) {
			recv(newConnection, clientInfo, sizeof(clientInfo), NULL);
			char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
			strncpy(login, strtok(clientInfo, " "), sizeof(login));
			strncpy(pass, strtok(NULL, " "), sizeof(pass));
			strncpy(name, strtok(NULL, " "), sizeof(name));
			strncpy(surname, strtok(NULL, " "), sizeof(surname));
			clients << endl;
			clients << cipher.encryption(login) << " " << cipher.encryption(pass) << " " << name << " " << surname;
		}
		clients.close();
		this->clients.clear();
	}
	void recieve_orders() {
		char answer_count[BUFF];
		recv(newConnection, answer_count, sizeof(answer_count), NULL);
		if (strcmp(answer_count, "-1") == 0) {
			ofstream seaPorts("Orders.txt");
			seaPorts.close();
			return;
		}

		int count = atoi(answer_count);
		char dataPorts[BUFF];
		ofstream seaPorts("Orders.txt");
		recv(newConnection, dataPorts, sizeof(dataPorts), NULL);
		seaPorts << dataPorts;
		for (int portNum = 1; portNum < count; portNum++) {
			recv(newConnection, dataPorts, sizeof(dataPorts), NULL);
			seaPorts << endl;
			seaPorts << dataPorts;
		}
		seaPorts.close();
	}

};

class Experts {
private:
	int rows;//количество альтернатив (3)
	int strings;
	int** marks;
	int** b;
	int* C;
	double* V;
	double W;
	double A1;
	double A2;
	double Di;
	double Da;
	int* result;
public:
	void getMarks() {
		this->rows = 3;
		char countOfExperts[BUFF];
		recv(newConnection, countOfExperts, sizeof(countOfExperts), NULL);
		this->strings = atoi(countOfExperts);
		marks = new int*[atoi(countOfExperts)];
		b = new int*[atoi(countOfExperts)];
		for (int i = 0; i < atoi(countOfExperts); i++) {
			char expertMarks[BUFF];
			marks[i] = new int[rows];
			b[i] = new int[rows];
			recv(newConnection, expertMarks, sizeof(expertMarks), NULL);
			marks[i][0] = atoi(strtok(expertMarks, " "));
			for (int markNum = 1; markNum < rows; markNum++) {
				marks[i][markNum] = atoi(strtok(NULL, " "));
			}
		}
		preferences();
	}
	void preferences() {
		int sum = 0;
		double A = 0;
		double A1 = 0;
		double A2 = 0;
		double* S;
		C = new int[rows];
		V = new double[rows];
		S = new double[rows];
		result = new int[rows];
		for (int i = 0; i < rows; i++) {
			C[i] = 0;
			V[i] = 0;
			S[i] = 0;
			result[i] = 0;
		}


		for (int i = 0; i < strings; i++) {
			for (int j = 0; j < rows; j++) {
				b[i][j] = marks[i][j];
			}
		}

		for (int j = 0; j < rows; j++) {
			for (int i = 0; i < strings; i++) {
				C[j] += b[i][j];
				S[j] += marks[i][j];
			}
		}
		for (int i = 0; i < rows; i++) {
			sum += C[i];
		}
		//Определение весов для каждой услуги
		for (int i = 0; i < rows; i++) {
			V[i] = double(C[i]) / double(sum);
		}

		for (int j = 0; j < rows; j++) {
			A += (1 / strings)*C[j]; //тут нужно будет найти средние оценки каждой альтернативы
		}
		for (int j = 0; j < rows; j++) {
			for (int i = 0; i < strings; i++) {
				A1 += pow((marks[i][j] - A), 2); //тут нужно будет найти средние оценки каждой альтернативы
			}
		}
		for (int i = 0; i < strings; i++) {
			for (int j = 0; j < rows; j++) {
				A2 += pow((marks[i][j] - A), 2);
			}
		}//тут нужно будет найти средние оценки каждой альтернативы
		W = 1 / (strings - 1)*A1; //дисперсии оценок для каждого эксперта

	}

	void sendMarks() {
		double max = -1;
		for (int i = 3; i >= 1; i--) {
			int maxIndex = -1;
			for (int j = 0; j < 3; j++) {
				if (V[j] > max) {
					max = V[j];
					maxIndex = j;
				}
			}
			V[maxIndex] = -10;
			result[maxIndex] = i;
			max = -1;
		}
		string sendMarks;
		for (int i = 0; i < 4; i++) {
			sendMarks += to_string(result[i]) + " ";
		}
		sendMarks += to_string(result[4]);
		char marks[BUFF];
		strncpy(marks, sendMarks.c_str(), sizeof(marks));
		send(newConnection, marks, sizeof(marks), NULL);

	}
	double getW() {
		return W;
	}
	double getDi() {
		return Di;
	}
	double getDa() {
		return Da;
	}
};

FileWork database;


void menu_admin() {
	//cout << "Открыто меню админа" << endl;
	database.send_clients();
	database.send_orders();

	char answer[BUFF];
	while (true) {
		recv(newConnection, answer, sizeof(answer), NULL);
		int choice = atoi(answer);
		char answer_count[BUFF]; // -1 для выхода, а иначе количество и дальше
		switch (choice) {
		case(1):
		{
			database.recieve_orders();
		}
		break;
		case(2):
		{
			bool flag = FALSE;
			while (flag == FALSE) {
				char clientChoice[BUFF];
				recv(newConnection, clientChoice, sizeof(clientChoice), NULL);
				switch (atoi(clientChoice)) {
				case(1):
					database.recieve_clients();
					break;
				case(2):
					database.recieve_clients();
					break;
				case(4):
					flag = TRUE;
					break;
				}

			}
		}
		break;
		case(3):
		{
			double W;
			Experts expert;
			while (true)
			{
				expert.getMarks();
				W = expert.getW();
				char wSend[BUFF];
				strncpy(wSend, to_string(W).c_str(), sizeof(wSend));
				send(newConnection, wSend, sizeof(wSend), NULL);
				if (W < 0.5) {
					continue;
				}
				else {
					expert.sendMarks();
					break;
				}

			}
			database.recieve_orders();

		}
		break;
		case(5):
		{
			database.recieve_orders();
		}
		break;
		case(6):
		{
			database.recieve_orders();
		}
		break;
		case(8):
		{
			return;
		}
		break;
		}
	}
}

void menu_client() {
	database.send_clients();
	database.send_orders();

	char answer[BUFF];
	while (true) {
		recv(newConnection, answer, sizeof(answer), NULL);
		int choice = atoi(answer);
		char answer_count[BUFF]; // -1 для выхода, а иначе количество и дальше
		switch (choice) {
		case(1):
		{
			database.recieve_orders();
		}
		break;

		case(3):
		{
			database.recieve_orders();
		}
		break;
		case(4):
		{
			database.recieve_orders();
		}
		break;
		case(5):
		{
			database.recieve_clients();
		}
		break;
		case(7):
		{
			return;
		}
		break;
		}
	}
}


void Admin() {
	Cipher cipher;
	char recvLogin[BUFF], recvPass[BUFF];
	recv(newConnection, recvLogin, sizeof(recvLogin), NULL);
	recv(newConnection, recvPass, sizeof(recvPass), NULL);
	std::string login, pass, name, surname;
	bool checked = FALSE;
	std::ifstream infile("Admins.txt");
	while (!infile.eof()) {
		infile >> login >> pass >> name >> surname;
		login = cipher.decryption(login);
		pass = cipher.decryption(pass);
		if (login == std::string(recvLogin) && pass == std::string(recvPass)) {
			send(newConnection, "200", sizeof("200"), 0);
			checked = TRUE;
			break;
		}
	}
	infile.close();
	if (!checked) {
		send(newConnection, "404", sizeof("404"), 0);
		return;
	}
	cout << "Admin вошел" << endl;
	menu_admin();
	cout << "Admin вышел" << endl;
	return;
}

void Client() {
	Cipher cipher;
	char recvLogin[BUFF], recvPass[BUFF];
	recv(newConnection, recvLogin, sizeof(recvLogin), NULL);
	recv(newConnection, recvPass, sizeof(recvPass), NULL);
	std::string login, pass, name, surname;
	bool checked = FALSE;
	std::ifstream infile("Clients.txt");
	while (!infile.eof()) {
		infile >> login >> pass >> name >> surname;
		login = cipher.decryption(login);
		pass = cipher.decryption(pass);
		if (login == std::string(recvLogin) && pass == std::string(recvPass)) {
			send(newConnection, "200", sizeof("200"), 0);
			checked = TRUE;
			break;
		}
	}
	infile.close();
	if (!checked) {
		send(newConnection, "404", sizeof("404"), 0);
		return;
	}
	cout << name << " вошел" << endl;
	char client_name[BUFF];
	strncpy(client_name, name.c_str(), sizeof(client_name));
	send(newConnection, client_name, sizeof(client_name), NULL);
	menu_client();
	cout << name << " вышел" << endl;
	return;
}

int main(int argc, char* argv[]) {
	system("color F0");
	system("chcp 1251 >> null");
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);


	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		std::cout << "Error #2\n";
	}
	else {
		std::cout << "Клиент подключен!\n";
		while (true)
		{
			char result[BUFF];
			recv(newConnection, result, sizeof(result), 0);
			int menu = atoi(result);
			switch (menu) {
			case(1):
				Admin();
				break;
			case(2):
				Client();
				break;
			}
			if (menu == 3) {
				break;
			}
		}

	}

	return 0;
}
