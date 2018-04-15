#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<string>
#include <Ws2tcpip.h>
#include<bitset>
#include<vector>
#include <time.h>
#include <cstdlib>

using namespace std;
bool flaga = 0;
string pom;
SOCKET Connection;//This client's connection to the server

				  //wyznaczanie czasu w ktorym odsylamy inf. do klienta

char * znacznik_czasu() {

	time_t czas;
	struct tm * ptr;
	time(&czas);
	ptr = localtime(&czas);
	char * data = asctime(ptr);

	return data;

} //wyznaczanie id sesji
int idsesji(string pom) {
	//cout <<"POM ! ! ! ! "<< pom << endl;
	string numer;
	int id = 0;
	int counter = 0;
	for (int i = 0; i < pom.size() - 1; i++) {

		if (counter == 3 && id == 0) {
			numer = pom[i - 3];
			numer = numer + pom[i - 2];
			id = stoi(numer);
		}
		if (pom[i] == ';') {
			counter++;
		}

	}
	//cout << "ID SESJI: " << id << endl;
	pom = "";
	return id;
}
//struktura protokolu
struct Prot {

	string operacja;
	string odpowiedz;
	int NSekwencyjny;
	int identyfikator;
	string dane;
	string czas;
};
//niewykorzystywana funkcja
void odbierz_wiadomosc_sesji() {
	char * buffer = new char[1024];
	pom = "";
	recv(Connection, buffer, 1024, NULL);
	for (int i = 0; i < 1024; i++) {
		if (buffer[i] == buffer[100]) {
			break;
		}
		else {
			pom = pom + buffer[i];
		}
	}
	//cout << pom << endl;
	std::cout << "Czy chcesz kontynuowac? 1 = tak, 0 = nie" << endl;
	int decyzja;
	string abc;
	cin >> abc;
	if (abc == "1" || abc == "0") {
		decyzja = stoi(abc);
	}
	else {
		decyzja = 0;
	}

	Prot prot;

	// BRAK CHÊCI NA PO£¥CZENIE !!!!!!!!!!!!!!!!!!!!!!!!!
	if (decyzja == 0) {
		closesocket(Connection);
		WSACleanup();
		exit(0);
	}

	if (decyzja == 1) {

	}
	else {
		//closesocket(Connection);
		//WSACleanup();
		//exit(0);
	}

	string przesylanie = prot.operacja + to_string(prot.NSekwencyjny) + to_string(prot.identyfikator);
	//cout << "TO CO PRZESYLAM " << przesylanie << endl;
	const char *cstr = przesylanie.c_str();

	//send(Connection, cstr , przesylanie.size(), NULL);

	if (decyzja == 0) {
		closesocket(Connection);
		WSACleanup();
		exit(0);
	}

}

//wysylanie wiadomosci do serwera
void wyslij(std::string wiadomosc) {
	Prot p1;
	p1.czas = znacznik_czasu();
	string czas = "Czas=" + p1.czas + ";";
	p1.operacja = "Operacja=wyslij;";
	p1.odpowiedz = "";
	p1.dane = stoi(wiadomosc);
	p1.NSekwencyjny = 1;
	string NS = "NSekwencyjny=" + to_string(p1.NSekwencyjny) + ";";
	p1.identyfikator = idsesji(pom);

	string przesylanie;
	if (p1.odpowiedz == "") {
		przesylanie = p1.operacja + NS + "Identyfikator:" + to_string(p1.identyfikator) + ";" + czas;
	}
	else {
		przesylanie = p1.odpowiedz + NS + "Identyfikator:" + to_string(p1.identyfikator) + ";" + czas;
	}
	const char *cstr = przesylanie.c_str();
	//cout << "PRZESYLANIE   WWW " << przesylanie << endl;
	send(Connection, cstr, przesylanie.size(), NULL);


	//delete cstr;
}
//wysylanie 2 wiadomosci do serwera
void wyslij2(std::string wiadomosc) {
	Prot p1;
	p1.czas = znacznik_czasu();
	string czas = "Czas=" + p1.czas + ";";
	//p1.operacja = "Operacja=wyslij;";
	p1.odpowiedz = "";
	p1.dane = "Dane=" + wiadomosc + ";";
	p1.NSekwencyjny = 0;
	string NS = "NSekwencyjny=" + to_string(p1.NSekwencyjny) + ";";
	p1.identyfikator = idsesji(pom);

	string przesylanie;
	cout << p1.dane << "D A N E " << endl;

	przesylanie = p1.dane + NS + "Identyfikator:" + to_string(p1.identyfikator) + ";" + czas;

	//cout << "TO CO PRZESYLAM " << przesylanie << endl;


	const char *cstr = przesylanie.c_str();
	//cout << idsesji << " IDSESJI" << endl;
	//cout << "PRZESYLANIE   ZZZ " << przesylanie << endl;
	send(Connection, cstr, przesylanie.size(), NULL);

	//delete cstr;
}
//obsluga watku
void ClientThread()
{
	int bufferlength = 1024;
	while (true)
	{

		char * buffer = new char[bufferlength];

		recv(Connection, buffer, bufferlength, NULL);
		// TU ZAMYKAMY KLIENTA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		string pomocn;
		int counter = 0;
		for (int i = 0; i < 1024; i++) {
			if (counter == 4) {
				break;
			}
			if (buffer[i] == ';') {
				counter++;
			}
			pomocn = pomocn + buffer[i];
		}
		pom = pomocn;
		int licznik = 0;
		for (int i = 0; i < pomocn.size(); i++) {
			if (pomocn[i] == ';') {
				break;
			}
			licznik++;
		}

		string podstawa;
		for (int i = 0; i < licznik; i++) {
			podstawa = podstawa + pomocn[i];
		}
		//while (podstawa.size() < 32) {
		//podstawa.push_back('0');
		//}
		//rownie dobrze moglibysmy dac if podstawa == czas do konca, tak jest szybciej
		//to samo obowiazuje przy kolejnych ifach, ktore obsluguja rozne operacje i odpowiedzi

		cout << podstawa << endl;
		if (podstawa[4] == '=') {
			if (podstawa[podstawa.size() - 2] != '=') {
				cout << "pozostalo " << podstawa[podstawa.size() - 2] << podstawa[podstawa.size() - 1] << "[s]" << endl;
			}
			else cout << "pozostalo " << podstawa[podstawa.size() - 1] << "[s]" << endl;
		}
		if (podstawa.size()>17)
			if (podstawa[17] == 'e') {
				cout << "wygrales " << endl;
				//ExitThread(NULL);
				system("cls");
				closesocket(Connection);
				WSACleanup();
				exit(0);
				//cin.ignore(2);
				flaga = 1;
				exit(0);
			}
		if (podstawa.size()>31)
			if (podstawa[31] == 's') {
				cout << "przegrales " << endl;
				//ExitThread(NULL);
				system("cls");
				closesocket(Connection);
				WSACleanup();
				exit(0);
				//cin.ignore(2);
				flaga = 1;
				exit(0);
			}
		//if (podstawa[4] != '='&&podstawa[31] != 's'&&podstawa[17] == 'e') {
		cout << pomocn << endl;
		//}
		delete[] buffer;
	}
}
int main() {
	time_t czas;
	struct tm * ptr;
	time(&czas);
	ptr = localtime(&czas);
	char * data = asctime(ptr);
	//std::cout << "Data: " << data;
	char * asctime(const struct tm * ptr);
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr;
	inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
	int addrlen = sizeof(addr);
	addr.sin_addr = addr.sin_addr;
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	// jezeli uda sie polaczyc z serwerem, to pojawi sie komunikat
	std::cout << "udalo sie polaczyc z serwerem!" << std::endl;
	//odbierz_wiadomosc_sesji();
	char * buffer = new char[1024];
	recv(Connection, buffer, 1024, NULL);

	//pom="";
	int counter = 0;
	for (int i = 0; i < 1024; i++) {
		if (counter == 4) {
			break;
		}
		if (buffer[i] == ';') {
			counter++;
		}
		pom = pom + buffer[i];
	}
	//cout << pom << endl;
	string liczba;
	/*
	for (int i = pom.size() - 2; i <= pom.size() - 1; i++) {
	liczba += pom[i];
	cout << "PETLA" << liczba << endl;
	}
	int x = atoi(liczba.c_str());
	idsesji = x;
	*/
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	string wejscie;
	while (true)
	{

		std::cout << "Podaj liczbe: " << endl;
		cin >> wejscie;
		wyslij(wejscie.c_str());
		wyslij2(wejscie.c_str());
		Sleep(10);
		if (flaga == 1) {
			system("cls");
			closesocket(Connection);
			WSACleanup();
			exit(0);
			//cin.ignore(2);
			flaga = 1;
		}
	}
	return 0;
}