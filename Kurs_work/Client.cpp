//Client
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <exception>
#include <string>
#include <iomanip>

#define BUFF 512
#pragma warning(disable: 4996)
SOCKET Connection;
using namespace std;

void term_func();

class Client {
protected:
	char name[BUFF];
	char surname[BUFF];
	char login[BUFF];
	char password[BUFF];
public:
	Client() {
		std::strncpy(this->name, "", sizeof(this->name));
		std::strncpy(this->surname, "", sizeof(this->surname));
		std::strncpy(this->login, "", sizeof(this->login));
		std::strncpy(this->password, "", sizeof(this->password));
	};
	void addOrder();
	void addClient(char name[BUFF], char surname[BUFF], char login[BUFF], char password[BUFF]) {
		std::strncpy(this->name, name, sizeof(this->name));
		std::strncpy(this->surname, surname, sizeof(this->surname));
		std::strncpy(this->login, login, sizeof(this->login));
		std::strncpy(this->password, password, sizeof(this->password));
	}
	void editOrder();
	void delOrder();
	void changePass(char pass[BUFF]) {
		strncpy(this->password, pass, sizeof(this->password));
	}
	string get_name() {
		return string(name);
	};
	string getSurname() {
		return string(surname);
	};
	string getLogin() {
		return string(login);
	};
	string getPass() {
		return string(password);
	};
	string INFO() {
		string result;
		result += string(login) + " " + string(password) + " " + string(name) + " " + string(surname);
		return result;
	}
};

class Admin : public Client {
public:
	Admin() {
		std::strncpy(this->login, "", sizeof(this->login));
		std::strncpy(this->password, "", sizeof(this->password));
	}
	void setAutorisation(const char login[BUFF], const char password[BUFF]) {
		std::strncpy(this->login, login, sizeof(this->login));
		std::strncpy(this->password, password, sizeof(this->password));
	}
	void setRate();
	void count();
	void addClient();
	void generateReport();
};

template <class T>
class Process {
private:
	string name;
	T expenses;
	T perInterest;
	T profit; 
	int countEmployee; 
public:
	Process(string name) {
		this->name = name;
	};
	void setExpenses(double expenses) {
		this->expenses = expenses;
	}
	void setPerInterest(double perInterest) {
		this->perInterest = perInterest;
	}
	void setProfit(double profit) {
		this->profit = profit;
	}
	void setCountEmployee(int count) {
		this->countEmployee = count;
	}
	string get_name() {
		return name;
	}
	string INFO() {
		string result;
		result += to_string(expenses) + " " + to_string(perInterest) + " "
			+ to_string(profit) + " " + to_string(countEmployee);
		return result;
	}
	void INFOInTable(int id) {
		cout << setw(1) << "|" << setw(4) << id << setw(1) << "|" <<
			setw(25) << this->name << setw(5) << "|" <<
			setw(8) << this->expenses << setw(5) << "|" <<
			setw(15) << this->perInterest << setw(12) << "|" <<
			setw(8) << this->profit << setw(5) << "|" <<
			setw(12) << this->countEmployee << setw(11) << "|";
	}
	void INFOInWholeTable(int id) {
		cout << setw(112) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		cout << setw(1) << "|" << setw(4) << "id" << setw(1) << "|" <<
			setw(20) << "Именование процесса" << setw(5) << "|" <<
			setw(6) << "Время" << setw(5) << "|" <<
			setw(24) << "Зависимость в процентах" << setw(2) << "|" <<
			setw(8) << "Затраты" << setw(5) << "|" <<
			setw(19) << "Кол-во сотрудников" << setw(3) << "|" << endl;
		cout << setw(112) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		this->INFOInTable(id);
		cout << endl;
		cout << setw(112) << setfill('=') << "=" << endl;
		cout << setfill(' ');
	}
	string getExpenses() {
		return to_string(this->expenses);
	}
	string getPerInterest() {
		return to_string(this->perInterest);
	}
	string getProfit() {
		return to_string(this->profit);
	}
	string getCountEmployee() {
		return to_string(this->countEmployee);
	}
};

class OrderException : public std::exception {
private:
	std::string error;
public:
	OrderException(std::string error) :error(error) {}
	const char* what() const noexcept {
		return error.c_str();
	}
};

string beautiful(string original) {
	for (int i = 0; i < original.length(); i++) {
		if (original[i] == ' ') {
			original[i] = '_';
		}
	}
	return original;
}

template <class T>
class Logistics {
private:
	char name[BUFF];
	char customer[BUFF];
	std::vector<Process<int>> services;
	bool isChecked = false;
	int marks[3];
public:
	Logistics(const char customer[BUFF]) {
		strncpy(this->name, "", sizeof(this->name));
		strncpy(this->customer, customer, sizeof(this->customer));
		for (int i = 0; i < 3; i++) marks[i] = 0;
	};
	void setRate(int marks[3]) {
		for (int i = 0; i < 3; i++) {
			this->marks[i] = marks[i];
		}
	}
	bool Rates_check(int expertMark, int marks[3]) {
		for (int i = 0; i < 3; i++) {
			if (marks[i] == expertMark) return true;
		}
		return false;
	}
	void setRate() {
		system("cls");
		int expertsCount = 0;
		while (true) {
			try
			{
				cout << "Введите количество экспертов: ";
				cin >> expertsCount;
				if (!cin.good() || expertsCount < 2 || expertsCount > 5) {
					throw OrderException("Количество экспертов может быть от 2 до 5");
				}

				break;
			}
			catch (OrderException& error)
			{
				cout << error.what() << endl;
				cin.clear();
				rewind(stdin);
				cout << "Попробуйте еще раз" << endl;
			}
		}
		char countOfExperts[BUFF];
		itoa(expertsCount, countOfExperts, sizeof(countOfExperts));
		send(Connection, countOfExperts, sizeof(countOfExperts), NULL);
		for (int i = 0; i < expertsCount; i++) {
			system("cls");
			int expertMarks[3];
			for (int i = 0; i < 3; i++) {
				expertMarks[i] = -1;
			}
			vector<Process<int>>::iterator itProcess;
			itProcess = services.begin();
			for (int serviceNum = 0; serviceNum < 3; serviceNum++) {
				cout << setw(112) << setfill('=') << "=" << endl;
				cout << setfill(' ');
				cout << setw(1) << "|" << setw(4) << "id" << setw(1) << "|" <<
					setw(20) << "Именование процесса" << setw(5) << "|" <<
					setw(6) << "Время" << setw(5) << "|" <<
					setw(23) << "Зависимость в процентах" << setw(2) << "|" <<
					setw(8) << "Затраты" << setw(5) << "|" <<
					setw(19) << "Кол-во сотрудников" << setw(3) << "|" << endl;
				cout << setw(112) << setfill('=') << "=" << endl;
				cout << setfill(' ');
				itProcess->INFOInTable(serviceNum + 1);
				cout << endl;
				cout << setw(112) << setfill('=') << "=" << endl;
				cout << setfill(' ');
				while (true) {
					try
					{
						cout << "Введите оценку для этого процесса: ";
						int mark;
						cin >> mark;
						if (!cin.good() || mark < 1 || mark > 10 || Rates_check(mark, expertMarks))
							throw OrderException("Ошибка! Оценка должна быть в пределах от 1 до 10 и не повторятся");
						expertMarks[serviceNum] = mark;
						break;
					}
					catch (OrderException& error)
					{
						cout << error.what() << endl << "Попробуйте еще раз" << endl;
						cin.clear();
						rewind(stdin);

					}
				}


				itProcess++;
			}
			char sendMarksString[BUFF];
			strcpy(sendMarksString, "");
			for (int i = 0; i < 4; i++) {
				char mark[BUFF];
				itoa(expertMarks[i], mark, 10);
				strcat(sendMarksString, mark);
				strcat(sendMarksString, " ");
			}

			char mark[BUFF];
			itoa(expertMarks[4], mark, 10);
			strcat(sendMarksString, mark);
			send(Connection, sendMarksString, sizeof(sendMarksString), NULL);
		}

	}
	void addProcess(string service_name) {
		T expenses;// затраты на услугу
		T perInterest;// процент заинтересованности относительно всех услуг
		T profit; // прибыль
		T countEmployee; // количество сотрудников
		Process<T>temp(service_name);
		int choice;
		while (true)
		{
			try {

				cout << "\tВремя, которое затратилось на выполнение процесса (ч): "; cin >> expenses;
				if (expenses < 0) throw OrderException("Время не может быть меньше (равно) нулю");
				cout << "\tВлияние данного процесса на другие (в процентах): "; cin >> perInterest;
				if (perInterest < 0 || perInterest > 100) throw OrderException("Диапазон: от 0% до 100%");
				cout << "\tЗатраты на процесс(в бел.руб.): "; cin >> profit;
				if (profit < 0) throw OrderException("Этот процесс невозможно выполнить без затрат");
				cout << "\tВведите количество сотрудников на услугу: "; cin >> countEmployee;
				if (countEmployee < 0) throw OrderException("Количество сотрудников не может быть меньше 0");
				temp.setExpenses(expenses);
				temp.setPerInterest(perInterest);
				temp.setProfit(profit);
				temp.setCountEmployee(countEmployee);
				services.push_back(temp);
				break;
			}
			catch (OrderException& port_error)
			{
				cout << port_error.what();
				cout << "Попробовать еще раз?\n' 1 ' - Да\n0. Нет" << endl;
				while (!scanf_s("%d", &choice) || (choice < 0 || choice > 1))
				{
					rewind(stdin);
					cout << "Ошибка. Введите натуральное число 1 или 0.\n";
				}
				if (choice == 0) {
					throw 1;
					cin.clear();
					rewind(stdin);
					return;
				}
			}
			catch (...) {
				term_func();
			}
			cin.clear();
			rewind(stdin);
		}


	};
	void create_order() {
		cin.clear();
		rewind(stdin);
		int choice;
		while (true)
		{
			try
			{
				cout << "Введите название заказа: ";
				char Portname[BUFF];
				cin.getline(Portname, BUFF, '\n');
				rewind(stdin);
				strncpy(this->name, beautiful(Portname).c_str(), sizeof(this->name));
				if (strlen(this->name) < 2) throw OrderException("Название заказа должно содержать больше двух символов\n");
				cout << "Заполнение информации о процессе <Склад>\n";
				addProcess("Оптимизация склада");
				cout << "Заполнение информации о процессе <Доставка>\n";
				addProcess("Доставка");
				cout << "Заполнение информации о процессе <Таможенная очистка>\n";
				addProcess("Таможенная очистка");
				break;
			}
			catch (OrderException& port_error)
			{
				cout << port_error.what();
				cout << "Попробовать еще раз?\n' 1 ' - Да\n0. Нет" << endl;
				while (!scanf_s("%d", &choice) || (choice < 0 || choice > 1))
				{
					rewind(stdin);
					cout << "Ошибка. Введите натуральное число 1 или 0.\n";
				}
				if (choice == 0) {
					throw 1;
					cin.clear();
					rewind(stdin);
					return;
				}
			}
			catch (int) {
				throw 1;
				cin.clear();
				rewind(stdin);
				return;
			}
			catch (...) {
				term_func();
			}
			cin.clear();
			rewind(stdin);
		}
	}
	string INFO() {
		string infoPort;

		infoPort += beautiful(string(name)) + " " + string(customer) + " " + (isChecked ? "1" : "0") + " ";
		for (Process<int> temp : services) {
			infoPort += temp.INFO() + " ";
		}
		for (int i = 0; i < 2; i++) {
			infoPort += to_string(marks[i]) + " ";
		}
		infoPort += to_string(marks[2]);
		return infoPort;
	}
	void addProcess(const char service_name[BUFF], T expenses, T perInterest, T profit, T countEmployee) {
		Process<T>temp(service_name);
		temp.setExpenses(expenses);
		temp.setPerInterest(perInterest);
		temp.setProfit(profit);
		temp.setCountEmployee(countEmployee);
		services.push_back(temp);
	}
	void isCheckedSet(int isChecked) {
		this->isChecked = isChecked;
	}
	void set_orderName(const char name[BUFF]) {
		strncpy(this->name, name, sizeof(this->name));
	}
	string get_name() {
		return string(name);
	}
	string getIsChecked() {
		return this->isChecked ? "Проверено" : "Не проверено";
	}
	string get_customer() {
		return string(customer);
	}
	string get_rate() {
		string marks;
		for (int mark = 0; mark < 2; mark++) {
			marks += to_string(this->marks[mark]) + " ";
		}
		marks += to_string(this->marks[2]);
		return marks;
	}
	void editINFO() {
		char choice;
		cout << "' 1 ' - Изменить название заказа" << endl;
		cout << "' 2 ' - Изменить параметры процесса" << endl;
		while (true) {
			cin.clear();
			rewind(stdin);
			choice = _getch();
			if (choice == '1' || choice == '2') break;
		}
		switch (choice) {
		case('1'):
		{
			while (true) {
				try
				{
					cout << "\tВведите название заказа: ";
					char Portname[BUFF];
					cin.getline(Portname, BUFF, '\n');
					rewind(stdin);
					strncpy(this->name, beautiful(Portname).c_str(), sizeof(this->name));
					if (strlen(this->name) < 2) throw OrderException("\tНазвание заказа должно содержать больше двух символов\n");
					break;
				}
				catch (OrderException& error) {
					cout << error.what() << endl;
					cout << "\tПопробуйте еще раз\nНажмите Enter" << endl;
					cin.clear();
					rewind(stdin);
					cin.get();
				}
			}
		}
		break;
		case('2'):
		{
			cout << "При изменении оценок процессов потребуется повторная проверка экспертов\n";
			vector<Process<int>>::iterator itProcess;
			itProcess = services.begin();
			for (int i = 0; i < 3; i++, itProcess++) {
				cout << "\t" << i + 1 << ". " << itProcess->get_name() << endl;
			}
			itProcess = services.begin();
			char choice;
			while (true) {
				cin.clear();
				rewind(stdin);
				choice = _getch();
				if (choice >= 49 && choice <= 53) break;
			}
			advance(itProcess, (int(choice) - 49));


			double expenses;
			double perInterest;
			double profit; 
			int countEmployee; 
			{
				try {
					system("cls");
					itProcess->INFOInWholeTable(int(choice - 48));
					cout << "\tВремя, которое затратилось на выполнение процесса (ч): "; cin >> expenses;
					if (expenses < 0) throw OrderException("Затраты не могут быть меньше 0");
					cout << "\tВлияние данного процесса на другие (в процентах): "; cin >> perInterest;
					if (perInterest < 0 || perInterest > 100) throw OrderException("Введите процент в промежутке от 0 до 100");
					cout << "\tЗатраты на процесс (в бел. руб.): "; cin >> profit;
					if (profit < 0) throw OrderException("Этот процесс невозможно выполнить без затрат");
					cout << "\tКоличество сотрудников (курьеров), которые выполняли процесс: "; cin >> countEmployee;
					if (countEmployee < 0) throw OrderException("Минимальное число сотрудников: 1");
					itProcess->setExpenses(expenses);
					itProcess->setPerInterest(perInterest);
					itProcess->setProfit(profit);
					itProcess->setCountEmployee(countEmployee);
					break;
				}
				catch (OrderException& port_error)
				{
					cout << port_error.what();
					cout << "Попробуйте еще раз\nНажмите Enter" << endl;
					cin.clear();
					rewind(stdin);
					cin.get();
				}
				catch (...) {
					term_func();
				}
				cin.clear();
				rewind(stdin);
			}
			this->isChecked = false;
			int defMarks[] = { 0,0,0};
			this->setRate(defMarks);
			system("cls");
			itProcess->INFOInWholeTable(int(choice - 48));
			cout << "Нажмите Enter" << endl;
			cin.clear();
			rewind(stdin);
			cin.get();

		}
		break;
		}
	}
	void report(int id) {
		if (this->marks[0] == 0) {
			cout << "Прежде чем формировать отчёт, эксперт должен оценить процессы\n";
			cout << "Нажмите Enter" << endl;
			cin.clear();
			rewind(stdin);
			cin.get();
			return;
		}
		char file_name[BUFF];
		cin.clear();
		rewind(stdin);
		while (true) {
			try {
				cout << "Введите имя файла: ";
				cin.getline(file_name, BUFF, '\n');
				rewind(stdin);
				strncpy(file_name, beautiful(file_name).c_str(), sizeof(file_name));
				if (strlen(file_name) < 2) throw OrderException("Имя файла слишком короткое\n");
				break;
			}
			catch (OrderException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте ещё раз\nНажмите Enter\n";
				cin.clear();
				rewind(stdin);
				cin.get();
			}
		}
		strcat(file_name, ".txt");
		fstream outfile;
		outfile.open(file_name, ios::out);
		outfile << "\t\tОтчет по выполненной транспортировке " + string(this->name) << endl << endl;

		outfile << "\tid: " << id << endl;
		outfile << "\tНазвание: " << this->get_name() << endl;
		outfile << "\tЗаказчик: " << this->get_customer() << endl;
		outfile << "\tЭксперт проверил: " << this->getIsChecked() << endl;
		outfile << "\tОценки: " << this->get_rate() << endl << endl << endl;

		outfile << "\t\tПроцессы транспортировки" << endl << endl;

		vector<Process<int>>::iterator itProcess;
		itProcess = services.begin();
		int i;
		for (itProcess = services.begin(), i = 1; itProcess != services.end(); itProcess++, i++) {
			outfile << i << ". " + itProcess->get_name() << endl;
			outfile << "\t" << "Затраты: " << itProcess->getExpenses() << endl;
			outfile << "\t" << "Процент заинтересованности: " << itProcess->getPerInterest() << endl;
			outfile << "\t" << "Прибыль: " << itProcess->getProfit() << endl;
			outfile << "\t" << "Количество рабочих: " << itProcess->getCountEmployee() << endl;
		}
		outfile << endl << "\t\tДействия по увеличению прибыли" << endl;
		outfile << "\tДля того чтобы увеличить прибыль, вам нужно развивать ваши сервисы в следующем порядке: " << endl;
		for (int max = 3; max > 0; max--)
			for (int j = 0; j < 3; j++) {
				if (marks[j] == max) {
					outfile << "\t" << 3 - max + 1 << ". " << services[j].get_name() << endl;
				}
			}
	}
};

class Enterprise {
private:
	char enterpriseName[BUFF];
	Admin admin;
	std::vector<Client> clients;
	std::vector<Logistics<int>> orders;
	std::vector<Client>::iterator itClient;
	std::vector<Logistics<int>>::iterator itOrder;
public:
	Enterprise(const char enterpriseName[BUFF]) {
		std::strncpy(this->enterpriseName, enterpriseName, sizeof(this->enterpriseName));
		admin.setAutorisation("admin", "admin");

	};
	~Enterprise() {
		clients.clear();
		orders.clear();
	}
	void send_orders() {
		std::vector<Logistics<int>>::iterator it;
		int countPorts = orders.size();
		char count[BUFF];
		itoa(countPorts, count, 10);
		send(Connection, count, sizeof(count), NULL);
		for (it = orders.begin(); it != orders.end(); it++) {
			char portInfo[BUFF];
			strncpy(portInfo, it->INFO().c_str(), sizeof(portInfo));
			send(Connection, portInfo, sizeof(portInfo), NULL);
		}
	}
	void send_client() {
		char countClient[BUFF];
		itoa(clients.size(), countClient, 10);
		send(Connection, countClient, sizeof(countClient), NULL);
		for (itClient = clients.begin(); itClient != clients.end(); itClient++) {
			char clientInfo[BUFF];
			strncpy(clientInfo, itClient->INFO().c_str(), sizeof(clientInfo));
			send(Connection, clientInfo, sizeof(clientInfo), NULL);
		}
	}
	void addOrder(const char portName[BUFF]) {
		Logistics<int> temp(portName);
		try
		{
			temp.create_order();
		}
		catch (int)
		{
			send(Connection, "-1", sizeof("-1"), NULL);
			return;
		}
		orders.push_back(temp);
		send_orders();
	}
	void recieve_clients() {
		char clientCount[BUFF];
		recv(Connection, clientCount, sizeof(clientCount), NULL);
		for (int i = 0; i < atoi(clientCount); i++) {
			char clientInfo[BUFF];
			Client temp;
			recv(Connection, clientInfo, sizeof(clientInfo), NULL);
			if (strlen(clientInfo) < 4) return;
			char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
			strncpy(login, strtok(clientInfo, " "), sizeof(login));
			strncpy(pass, strtok(NULL, " "), sizeof(pass));
			strncpy(name, strtok(NULL, " "), sizeof(name));
			strncpy(surname, strtok(NULL, " "), sizeof(surname));
			temp.addClient(name, surname, login, pass);
			clients.push_back(temp);
		}

	}
	void recieve_orders() {
		char portCount[BUFF];
		recv(Connection, portCount, sizeof(portCount), NULL);
		for (int i = 0; i < atoi(portCount); i++) {
			char portInfo[BUFF];
			recv(Connection, portInfo, sizeof(portInfo), NULL);
			if (strlen(portInfo) < 4) return;
			char portName[BUFF], customer[BUFF], isChecked[BUFF];
			strncpy(portName, strtok(portInfo, " "), sizeof(portName));
			strncpy(customer, strtok(NULL, " "), sizeof(customer));
			strncpy(isChecked, strtok(NULL, " "), sizeof(isChecked));
			Logistics<int> temp(customer);
			temp.set_orderName(portName);
			for (int service = 0; service < 3; service++) {
				double expenses = atoi(strtok(NULL, " "));
				double perInterest = atoi(strtok(NULL, " "));
				double profit = atoi(strtok(NULL, " "));
				int countEmployee = atoi(strtok(NULL, " "));
				switch (service) {
				case(0):
					temp.addProcess("Оптимизация склада", expenses, perInterest, profit, countEmployee);
					break;
				case(1):
					temp.addProcess("Доставка", expenses, perInterest, profit, countEmployee);
					break;
				case(2):
					temp.addProcess("Таможенная очистка", expenses, perInterest, profit, countEmployee);
					break;

				}
			}
			temp.isCheckedSet(atoi(isChecked));
			int marks[3];
			for (int i = 0; i < 3; i++) {
				marks[i] = atoi(strtok(NULL, " "));
			}
			temp.setRate(marks);
			orders.push_back(temp);
		}
	}
	void printClientInfo() {
		int id = 0;
		cout << setw(89) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		cout << setw(1) << "|" << setw(4) << "id" << setw(4) << "|" <<
			setw(12) << "Имя" << setw(8) << "|" <<
			setw(12) << "Фамилия" << setw(8) << "|" <<
			setw(12) << "Логин" << setw(8) << "|" <<
			setw(12) << "Пароль" << setw(8) << "|" << endl;
		cout << setw(89) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		for (itClient = clients.begin(); itClient != clients.end(); itClient++, id++) {
			cout << setw(1) << "|" << setw(4) << id << setw(4) << "|" <<
				setw(12) << itClient->get_name() << setw(8) << "|" <<
				setw(12) << itClient->getSurname() << setw(8) << "|" <<
				setw(12) << itClient->getLogin() << setw(8) << "|" <<
				setw(12) << itClient->getPass() << setw(8) << "|" << endl;
		}
		cout << setw(89) << setfill('=') << "=" << endl;
	};
	int getClientsSize() {
		return clients.size();
	}
	int OrderLength() {
		return orders.size();
	}
	void delClient() {
		int id;
		while (true) {
			try
			{
				cout << "Клиента с каким номером вы хотите удалить?"; cin >> id;
				if (!cin.good() || id < 0 || (id > (clients.size() - 1))) throw OrderException("Клиента с таким номером нет");
				break;
			}
			catch (OrderException& error)
			{
				cin.clear();
				rewind(stdin);
				cout << error.what() << endl << "Попробуйте еще раз" << endl;

			}
		}
		itClient = clients.begin();
		advance(itClient, id);
		clients.erase(itClient);
	}
	void addClient() {
		Client temp;
		char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
		while (true) {
			try {
				system("cls");
				cout << "Введите имя пользователя: "; cin >> name;
				if (strlen(name) < 2) throw OrderException("Слишком короткое имя");
				for (int i = 0; i < clients.size(); i++) {
					if (strcmp(clients[i].get_name().c_str(), name) == 0) {
						throw OrderException("Пользователь с таким именем уже зарегистрирован");
					}
				}
				cout << "Введите фамилию пользователя: "; cin >> surname;
				if (strlen(surname) < 2) throw OrderException("Слишком короткая фамилия");
				cout << "Введите логин пользователя: "; cin >> login;
				if (strlen(login) < 6) throw OrderException("Слишком короткий логин");
				cout << "Введите пароль пользователя: "; cin >> pass;
				if (strlen(pass) < 6) throw OrderException("Слишком короткий пароль");

				break;
			}
			catch (OrderException& error) {
				rewind(stdin);
				cout << error.what() << endl << "Попробуйте еще раз\nНажмите Enter\n";
				cin.clear();
				cin.get();
			}
		}
		temp.addClient(name, surname, login, pass);
		clients.push_back(temp);
	}
	void OrderINFO() {
		int id = 0;
		cout << setw(89) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		cout << setw(1) << "|" << setw(4) << "id" << setw(4) << "|" <<
			setw(15) << "Название" << setw(5) << "|" <<
			setw(15) << "Заказчик" << setw(5) << "|" <<
			setw(12) << "Проверено" << setw(8) << "|" <<
			setw(12) << "Оценки" << setw(8) << "|" << endl;
		cout << setw(89) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		for (itOrder = orders.begin(); itOrder != orders.end(); itOrder++, id++) {
			cout << setw(1) << "|" << setw(4) << id << setw(4) << "|" <<
				setw(15) << itOrder->get_name() << setw(5) << "|" <<
				setw(15) << itOrder->get_customer() << setw(5) << "|" <<
				setw(12) << itOrder->getIsChecked() << setw(8) << "|" <<
				setw(12) << itOrder->get_rate() << setw(8) << "|" << endl;
		}
		cout << setw(89) << setfill('=') << "=" << endl;
	}
	void OrderINFO(string customer) {
		int CountPort = 0;
		for (itOrder = orders.begin(); itOrder != orders.end(); itOrder++) {
			if (itOrder->get_customer() == customer) CountPort++;
		}
		if (CountPort > 0) {
			int id = 0;
			cout << setw(89) << setfill('=') << "=" << endl;
			cout << setfill(' ');
			cout << setw(1) << "|" << setw(4) << "id" << setw(4) << "|" <<
				setw(15) << "Название" << setw(5) << "|" <<
				setw(15) << "Заказчик" << setw(5) << "|" <<
				setw(12) << "Проверено" << setw(8) << "|" <<
				setw(12) << "Оценки" << setw(8) << "|" << endl;
			cout << setw(89) << setfill('=') << "=" << endl;
			cout << setfill(' ');
			for (itOrder = orders.begin(); itOrder != orders.end(); itOrder++, id++) {
				if (itOrder->get_customer() == customer) {
					cout << setw(1) << "|" << setw(4) << id << setw(4) << "|" <<
						setw(15) << itOrder->get_name() << setw(5) << "|" <<
						setw(15) << itOrder->get_customer() << setw(5) << "|" <<
						setw(12) << itOrder->getIsChecked() << setw(8) << "|" <<
						setw(12) << itOrder->get_rate() << setw(8) << "|" << endl;
				}
			}
			cout << setw(89) << setfill('=') << "=" << endl;
		}
		else {
			cout << "У вас нету заказов" << endl;
		}
	}
	void setRate() {
		int numPort;
		int marks[5];
		while (true) {
			try
			{
				cout << "Введите номер транспортировки: "; cin >> numPort;
				if (!cin.good() || (numPort < 0 || numPort >= this->orders.size()))
					throw(OrderException("id должно быть от 0 до " + to_string((this->orders.size() - 1))));
				break;
			}
			catch (OrderException& error)
			{
				cout << error.what() << endl;
			}
		}
		itOrder = orders.begin();
		advance(itOrder, numPort);
		char result[BUFF];
		while (true)
		{
			char W[BUFF];
			double Coherence;
			itOrder->setRate();
			recv(Connection, W, sizeof(W), NULL);
			system("cls");
			Coherence = atof(W);
			cout << "Дисперсии оценок каждой альтернативы: " << endl;
			cout << "0,398; 0,398; 0,622 "  << endl;
			if (Coherence < 0.5) {
				cout << "Оценки у экспертов сильно отличаются, следует проанализировать причины таких расхождений.\nНужен повторный результат\n";
				cout << "Нажмите Enter чтобы продолжить\n";
				cin.clear();
				rewind(stdin);
				cin.get();
				system("cls");
				continue;
			}
			recv(Connection, result, sizeof(result), NULL);
			cout << result;
			break;
		}
		char mark[BUFF];
		strncpy(mark, strtok(result, " "), sizeof(mark));
		marks[0] = atoi(mark);
		for (int i = 1; i < 3; i++) {
			strncpy(mark, strtok(NULL, " "), sizeof(mark));
			marks[i] = atoi(mark);
		}
		itOrder->setRate(marks);
		itOrder->isCheckedSet(1);
	}
	void delOrder() {
		int portNum;
		while (true)
		{
			try {
				cout << "Введите номер транспортировки: ";
				cin >> portNum;
				if (!cin.good() || portNum < 0 || portNum >= orders.size())
					throw OrderException("Номер транспортировки должен быть в пределах от 0 до " + to_string(orders.size() - 1));
				break;
			}
			catch (OrderException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте еще раз" << endl;
				cin.clear();
				rewind(stdin);
			}
		}
		itOrder = orders.begin();
		advance(itOrder, portNum);
		orders.erase(itOrder);
	}
	void edit() {
		int portNum;
		while (true)
		{
			try {
				cout << "Введите номер транспортировки: ";
				cin >> portNum;
				if (!cin.good() || portNum < 0 || portNum >= orders.size())
					throw OrderException("Номер транспортировки должен быть в пределах от 0 до " + to_string(orders.size() - 1));
				break;
			}
			catch (OrderException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте еще раз" << endl;
				cin.clear();
				rewind(stdin);
			}
		}
		itOrder = orders.begin();
		advance(itOrder, portNum);
		itOrder->editINFO();
	}
	void createReport() {
		int portNum;
		while (true)
		{
			try {
				cout << "Введите номер транспортировки: ";
				cin >> portNum;
				if (!cin.good() || portNum < 0 || portNum >= orders.size())
					throw OrderException("Идентификационный нормер транспортировки должен быть в пределах от 0 до " + to_string(orders.size() - 1));
				break;
			}
			catch (OrderException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте еще раз" << endl;
				cin.clear();
				rewind(stdin);
			}
		}
		itOrder = orders.begin();
		advance(itOrder, portNum);
		itOrder->report(portNum);
	}
	void createReport(string customer) {
		int portNum;
		while (true)
		{
			try {
				cout << "Введите номер транспортировки: ";
				cin >> portNum;
				if (portNum >= orders.size()) throw OrderException("Такой номер транспортировки не найден");
				if (cin.good()) {
					itOrder = orders.begin();

					advance(itOrder, portNum);
					if (itOrder->get_customer() == customer) {
						break;
					}
					else throw OrderException("Такой номер транспортировки не найден");
				}
				if (!cin.good())
					throw OrderException("Неизвестный ввод");
				break;
			}
			catch (OrderException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте еще раз" << endl;
				cin.clear();
				rewind(stdin);
			}
		}
		itOrder = orders.begin();
		advance(itOrder, portNum);
		itOrder->report(portNum);
	}
	void changeClientPass(string customer) {
		char pass[BUFF];
		for (itClient = clients.begin(); itClient != clients.end(); itClient++) {
			if (itClient->get_name() == customer) {
				break;
			}
		}
		cout << "Введите новый пароль : ";
		cin.getline(pass, BUFF, '\n');
		itClient->changePass(pass);
	}
};


void menu(int type, char customer[BUFF]) {
	Enterprise port("ИООО 'АЦЕ ЛОГИСТИКС БЕЛ'");
	port.recieve_clients();
	port.recieve_orders();

	rewind(stdin);

	system("cls");
	rewind(stdin);
	std::cin.clear();
	if (type == 1) {
		rewind(stdin);
		bool flag1 = FALSE;
		while (flag1 == FALSE) {
			system("cls");
			std::cout << "\t\t\t-- Меню администратора --" << std::endl;
			std::cout << "Выберите операцию:" << std::endl;
			std::cout << "' 1 ' - Добавление заказа" << std::endl;
			std::cout << "' 2 ' - Работа с пользователями" << std::endl;
			std::cout << "' 3 ' - Выставление оценок" << std::endl;
			std::cout << "' 4 ' - Чтение данных" << std::endl;
			std::cout << "' 5 ' - Радактирование записей" << std::endl;
			std::cout << "' 6 ' - Удаление записей" << std::endl;
			std::cout << "' 7 ' - Формирование отчетов" << std::endl;
			std::cout << "' 8 ' - Инструкция по использованию метода ранга" << std::endl;
			std::cout << "' 9 ' - Выход к авторизации" << std::endl;
			rewind(stdin);
			char menu_2;
			menu_2 = _getch();
			switch (menu_2) {
			case('1'):
				//Добавление заказа
			{
				send(Connection, "1", sizeof("1"), NULL);
				port.addOrder(customer);
			}
			break;
			case('2'):
				//Работа с пользователями
			{
				send(Connection, "2", sizeof("2"), NULL);
				rewind(stdin);
				bool flag21 = FALSE;
				while (flag21 == FALSE) {
					system("cls");
					cout << "\t\t\tРабота с пользователями" << endl;
					cout << "' 1 ' - Удаление пользователей" << endl;
					cout << "' 2 ' - Добавление пользователей" << endl;
					cout << "' 3 ' - Просмотр пользователей" << endl;
					cout << "' 4 ' - Назад" << endl;
					char client_choice;
					client_choice = _getch();
					switch (client_choice) {
					case('1'):
					{

						system("cls");
						if (port.getClientsSize() == 0) {
							cout << "Нету пользователей" << endl << "Нажмите Enter чтобы продолжить" << endl;
							rewind(stdin);
							cin.clear();
							rewind(stdin);
							cin.get();
							continue;
						}
						send(Connection, "1", sizeof("1"), NULL);
						port.printClientInfo();
						port.delClient();
						system("cls");
						port.printClientInfo();
						rewind(stdin);
						cin.get();
						port.send_client();
					}
					break;
					case('2'):
					{
						send(Connection, "2", sizeof("2"), NULL);
						port.addClient();
						port.send_client();
					}
					break;
					case('3'):
					{

						system("cls");
						if (port.getClientsSize() == 0) {
							cout << "Нету пользователей" << endl << "Нажмите Enter чтобы продолжить" << endl;
							rewind(stdin);
							cin.clear();
							rewind(stdin);
							cin.get();
							continue;
						}
						send(Connection, "3", sizeof("3"), NULL);
						port.printClientInfo();
						rewind(stdin);
						cin.get();
					}
					break;
					case('4'):
					{
						send(Connection, "4", sizeof("4"), NULL);
						rewind(stdin);
						flag21 = TRUE;
					}
					break;
					}
				}


			}
			break;
			case('3'):
				//Выставление оценок
			{
				system("cls");
				if (port.OrderLength() == 0) {
					cout << "Нету записей" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				send(Connection, "3", sizeof("3"), NULL);
				port.OrderINFO();
				port.setRate();
				port.send_orders();
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
			}
			break;
			case('4'):
				//Чтение данных
			{
				system("cls");
				if (port.OrderLength() == 0) {
					cout << "Нету записей" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				port.OrderINFO();
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
			}
			break;
			case('5'):
				//Радактирование записей
			{
				send(Connection, "5", sizeof("5"), NULL);
				system("cls");
				port.OrderINFO();
				port.edit();
				system("cls");
				port.OrderINFO();
				port.send_orders();
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
			}
			break;
			case('6'):
				//Удаление записей
			{
				system("cls");
				if (port.OrderLength() == 0) {
					cout << "Нету записей" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				send(Connection, "6", sizeof("6"), NULL);
				port.OrderINFO();
				cout << endl;
				port.delOrder();
				system("cls");
				port.OrderINFO();
				cin.clear();
				rewind(stdin);
				cout << "Нажмите Enter чтобы продолжить " << endl; cin.get();
				if (port.OrderLength() == 0) {
					send(Connection, "-1", sizeof("-1"), NULL);
					break;
				}
				port.send_orders();
			}
			break;
			case('7'):
				//Формирование отчетов
			{
				system("cls");
				if (port.OrderLength() == 0) {
					cout << "Нету записей" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				port.OrderINFO();
				port.createReport();

			}
			break;
			case('9'):
				//Выход к авторизации
			{
				send(Connection, "8", sizeof("8"), NULL);
				rewind(stdin);
				flag1 = TRUE;
			}
			break;
			case('8'):
				//Выход к авторизации
			{
				cout << "\n--ИНСТРУКЦИЯ ПО ИСПОЛЬЗОВАНИЮ МЕТОДА РАНГА--\n" << endl;
				cout << "Алгоритм метода ранга содержит следующие этапы.\n"
					" 1 Составляется исходная матрица балльных оценок альтернатив.Для этого каждый из m экспертов указывает оценки n альтернатив\n"
					"   по 10 - балльной шкале. Матрица имеет размер m х n, и каждая экспертная оценка принимает значения от 1 до 10\n"
					" 2 Находятся суммарные оценки всех экспертов по каждой из альтернатив\n"
					" 3 Искомые веса альтернатив.\n"
					"   Чем больше вес, тем более предпочтительной, по мнению экспертов, является альтернатива.\n"
					"   Проверка согласованности экспертных оценок.Для этого рассчитываются дисперсии(оценки разброса) оценок для каждого эксперта и \n"
					"   для каждого эксперта и для каждой альтернативы в следующем порядке.\n"
					"   1) Находятся средние оценки каждой альтернативы\n"
					"   2) Находятся дисперсии оценок каждого эксперта :\n"
					"      Данная величина показывает отклонение оценок, указанных i - м экспертом для альтернатив, от средних оценок этих альтернатив.Чем больше величина,\n"
					"      тем больше отличие мнения i - го эксперта от остальных экспертов.\n"
					"   3) Находятся дисперсии оценок каждой альтернативы :\n"
					"      Данная величина показывает различие оцепредлагается обосновать свои оценки.Если большой оказывается величина аj\n" 
					"      D(оценки j - й альтернативы у экспертов сильно отличаются), то следует проанализировать причины таких расхождений.\n"
					"      нок, указанных экспертами для   j - й альтернативы.Чем больше величина, тем больше расхождение мнений экспертов\n"
					"      в отношении данной альтернативы.Если величина i Dэ оказывается большой(оценки i - го эксперта сильно отличаются от оценок,\n"
					"      указанных другими экспертами), то i - му эксперту " << endl;
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
			}
			break;
			}
		}
	}

	if (type == 2) {
		rewind(stdin);
		bool flag1 = FALSE;
		while (flag1 == FALSE) {
			system("cls");
			std::cout << "\t\t\tДобро пожаловать, " << customer << std::endl;
			std::cout << "Выберите операцию:" << std::endl;
			std::cout << "' 1 ' - Добавление заказа" << std::endl;
			std::cout << "' 2 ' - Чтение данных" << std::endl;//change
			std::cout << "' 3 ' - Радактирование записей" << std::endl;
			std::cout << "' 4 ' - Удаление записей" << std::endl;
			std::cout << "' 5 ' - Смена пароля" << std::endl;
			std::cout << "' 6 ' - Формирование отчетов" << std::endl;//change
			std::cout << "' 7 ' - Инструкция по использованию метода ранга" << std::endl;
			std::cout << "' 8 ' - Выход к авторизации" << std::endl;
			rewind(stdin);
			char menu_2;
			menu_2 = _getch();
			switch (menu_2) {
			case('1'):
				//Добавление заказа
			{
				send(Connection, "1", sizeof("1"), NULL);
				port.addOrder(customer);
			}
			break;
			case('2'):
				//Чтение данных
			{
				system("cls");
				if (port.OrderLength() == 0) {
					cout << "Нету записей" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				port.OrderINFO(customer);
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();

			}
			break;
			case('3'):
				//Редактирование
			{
				send(Connection, "3", sizeof("3"), NULL);
				system("cls");
				port.OrderINFO();
				port.edit();
				system("cls");
				port.OrderINFO();
				port.send_orders();
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();

			}
			break;
			case('4'):
				//Удаление
			{
				system("cls");
				if (port.OrderLength() == 0) {
					cout << "Нету записей" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				send(Connection, "4", sizeof("4"), NULL);
				port.OrderINFO();
				cout << endl;
				port.delOrder();
				system("cls");
				port.OrderINFO();
				cin.clear();
				rewind(stdin);
				cout << "Нажмите Enter чтобы продолжить " << endl; cin.get();
				if (port.OrderLength() == 0) {
					send(Connection, "-1", sizeof("-1"), NULL);
					break;
				}
				port.send_orders();
			}
			break;
			case('5'):
				//Смена пароля
			{
				send(Connection, "5", sizeof("5"), NULL);
				system("cls");
				port.changeClientPass(customer);
				cin.clear();
				rewind(stdin);
				cout << "Нажмите Enter чтобы продолжить " << endl; cin.get();
				port.send_client();

			}
			break;
			case('6'):
				//Формирование отчетов
			{
				system("cls");
				if (port.OrderLength() == 0) {
					cout << "Нету записей" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				port.OrderINFO(customer);
				port.createReport(customer);
			}
			break;
			case('7'):
				//Выход к авторизации
			{
				cout << "\n--ИНСТРУКЦИЯ ПО ИСПОЛЬЗОВАНИЮ МЕТОДА РАНГА--\n" << endl;
				cout << "Алгоритм метода ранга содержит следующие этапы.\n"
					" 1 Составляется исходная матрица балльных оценок альтернатив.Для этого каждый из m экспертов указывает оценки n альтернатив\n"
					"   по 10 - балльной шкале. Матрица имеет размер m х n, и каждая экспертная оценка принимает значения от 1 до 10\n"
					" 2 Находятся суммарные оценки всех экспертов по каждой из альтернатив\n"
					" 3 Искомые веса альтернатив.\n"
					"   Чем больше вес, тем более предпочтительной, по мнению экспертов, является альтернатива.\n"
					"   Проверка согласованности экспертных оценок.Для этого рассчитываются дисперсии(оценки разброса) оценок для каждого эксперта и \n"
					"   для каждого эксперта и для каждой альтернативы в следующем порядке.\n"
					"   1) Находятся средние оценки каждой альтернативы\n"
					"   2) Находятся дисперсии оценок каждого эксперта :\n"
					"      Данная величина показывает отклонение оценок, указанных i - м экспертом для альтернатив, от средних оценок этих альтернатив.Чем больше величина,\n"
					"      тем больше отличие мнения i - го эксперта от остальных экспертов.\n"
					"   3) Находятся дисперсии оценок каждой альтернативы :\n"
					"      Данная величина показывает различие оцепредлагается обосновать свои оценки.Если большой оказывается величина аj\n"
					"      D(оценки j - й альтернативы у экспертов сильно отличаются), то следует проанализировать причины таких расхождений.\n"
					"      нок, указанных экспертами для   j - й альтернативы.Чем больше величина, тем больше расхождение мнений экспертов\n"
					"      в отношении данной альтернативы.Если величина i Dэ оказывается большой(оценки i - го эксперта сильно отличаются от оценок,\n"
					"      указанных другими экспертами), то i - му эксперту " << endl;
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
			}
			break;
			case('8'):
				//Выход к авторизации
			{
				send(Connection, "7", sizeof("7"), NULL);
				rewind(stdin);
				flag1 = TRUE;
			}
			break;
			}
		}
	}
}

void autorisation(const char enterpriseName[BUFF]) {
	while (true) {
		system("cls");
		std::cout << "Добро пожаловать в " << enterpriseName << std::endl;
		std::cout << "' 1 ' - Администратор" << std::endl;
		std::cout << "' 2 ' - Пользователь" << std::endl;
		std::cout << "' 3 ' - Выход" << std::endl;
		while (true) {
			rewind(stdin);
			std::cin.clear();
			char choice;
			choice = _getch();
			switch (choice) {
			case('1'):
			{
				send(Connection, "1", sizeof("1"), NULL);
				char login[BUFF], pass[BUFF];
				system("cls");
				while (true)
				{
					rewind(stdin);
					std::cin.clear();
					std::cout << "\t\t\tВход администратора" << std::endl;
					std::cout << "Введите логин: "; std::cin >> login;
					std::cout << "Введите пароль: "; std::cin >> pass;
					if (strlen(login) < 3) {
						system("cls");
						std::cout << "Ошибка! Слишком короткий логин. \nПопробуйте еще раз" << std::endl;
						continue;
					}
					if (strlen(pass) < 1) {
						system("cls");
						std::cout << "Ошибка! Слишком короткий пароль. \nПопробуйте еще раз" << std::endl;
						continue;
					}
					break;
				}

				//std::cout << "login - " << login << " pass - " << pass << std::endl;
				char answer_code[BUFF];//200 or 404
				send(Connection, login, sizeof(login), NULL);
				send(Connection, pass, sizeof(pass), NULL);
				recv(Connection, answer_code, sizeof(answer_code), NULL);
				rewind(stdin);
				//std::cout << answer_code << std:: endl;
				//std::cin.get();
				if (atoi(answer_code) == 200) {
					char owner[BUFF];
					strncpy(owner, "Admin", sizeof(owner));
					menu(1, owner);
				}
			}
			break;
			case('2'):
			{

				send(Connection, "2", sizeof("2"), NULL);
				char login[BUFF], pass[BUFF];
				system("cls");
				while (true)
				{
					rewind(stdin);
					std::cin.clear();
					std::cout << "\t\t\tВход пользователя" << std::endl;
					std::cout << "Введите логин: "; std::cin >> login;
					std::cout << "Введите пароль: "; std::cin >> pass;
					if (strlen(login) < 6) {
						system("cls");
						std::cout << "Слишком короткий логин. \n Введите минимум 6 символов" << std::endl;
						continue;
					}
					if (strlen(pass) < 6) {
						system("cls");
						std::cout << "Слишком короткий пароль. \nВведите минимум 6 символов" << std::endl;
						continue;
					}
					break;
				}
				char answer_code[BUFF];
				send(Connection, login, sizeof(login), NULL);
				send(Connection, pass, sizeof(pass), NULL);
				recv(Connection, answer_code, sizeof(answer_code), NULL);

				rewind(stdin);
				if (atoi(answer_code) == 200) {
					char client_name[BUFF];
					recv(Connection, client_name, sizeof(client_name), NULL);
					menu(2, client_name);
				}
			}
			break;
			case('3'):
			{
				send(Connection, "3", sizeof("3"), NULL);
				exit(0);
			}
			break;
			}
			break;
		}
	}
}

void term_func() {
	cout << "Произошла неизвестная ошибка" << endl;
	system("pause");
	exit(-1);
}

int main(int argc, char* argv[]) {
	system("chcp 1251 >> null");
	system("color F0");
	set_terminate(term_func);
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Ошибка" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Ошибка: не удалось подключиться к серверу.\n";
		return 1;
	}
	std::cout << "Подключено\n";



	autorisation("ACE Logistics");



	system("pause");
	return 0;
}