#include <iostream>
#include <fmt/printf.h>
#include "menu.hpp"
#include "database.hpp"

Database db;


void ShowClient(Database::Client client);
void searchClient();
void insertClient();
void removeClient();
void ShowClients();
void showCount();

int main()
{
	if (!db.Connect("tcp://localhost:3306", "root", "", "sistema_registro"))
	{
		fmt::print("Houve algum erro ao tentar se conectar ao banco de dados.\nAperte qualquer tecla para finalizar o programa.");
		_getch();
		exit(EXIT_FAILURE);
	}


	fmt::print("Conectado ao banco de dados...\n");

	Menu menu;

	menu.AddOption("Pesquisar cliente", searchClient);
	menu.AddOption("Inserir cliente", insertClient);
	menu.AddOption("Ver clientes", ShowClients);
	menu.AddOption("Remover cliente", removeClient);
	menu.AddOption("Quantidade de clientes", showCount);
	menu.AddOption("Sair do programa cliente", [&]() {menu.StopMenu(); });
	menu.Active();
	return EXIT_SUCCESS;
}



void ShowClient(Database::Client client)
{
	fmt::print("ID : {}\n", client.clientid);
	fmt::print("Nome : {}\n", client.sName);
	fmt::print("Idade : {}\n", client.iAge);
	fmt::print("Email : {}\n", client.sEmail);
	fmt::print("Endereco : {}\n", client.sAddress);
}

void searchClient()
{
	std::cin.clear();
	std::string client_name;
	fmt::print("Nome completo do cliente ou clientID: ");
	std::getline(std::cin, client_name);

	if (client_name.empty())
	{
		system("cls");
		fmt::print("[!] Espaco em branco, digite um nome para pesquisa - lo.\n");
		_getch();
		return;
	}

	auto clientList = db.GetClientsByName(client_name);
	if (clientList.empty())
	{
		system("cls");
		fmt::print("[!] Nenhum cliente encontrado com esse nome.\n");
		_getch();
		return;
	}


	fmt::print("{}\n", std::string(80, '-'));
	for (auto& client : clientList)
	{
		ShowClient(client);
		fmt::print("{}\n", std::string(80, '-'));
	}


	_getch();

}

void insertClient()
{
	std::cin.clear();
	Database::Client client;
	fmt::print("Nome do cliente: ");
	std::getline(std::cin, client.sName);

	fmt::print("Senha do cliente: ");
	std::getline(std::cin, client.sPass);

	fmt::print("Email do cliente: ");
	std::getline(std::cin, client.sEmail);

	fmt::print("Endereco do cliente: ");
	std::getline(std::cin, client.sAddress);

	fmt::print("Idade do cliente: ");
	std::cin >> client.iAge;

	if (client.sName.empty() || client.sPass.empty())
	{
		fmt::print("Nome ou senha vazio, e obrigatorio esses valores.\n");
		_getch();
		return;
	}

	auto id = db.InsertClient(client);
	fmt::print("Inserido com sucesso o cliente ID {}!\n", id);
	_getch();
}

void removeClient()
{
	Database::ClientID clientID;
	std::cin.clear();
	fmt::print("ID do cliente: ");
	std::cin >> clientID;

	const auto client = db.GetClientByID(clientID);

	if (!client.clientid)
	{
		system("cls");
		fmt::print("[!] Nenhum cliente encontrado com esse ID.\n");
		_getch();
		return;
	}

	db.DeleteClient(clientID);
	fmt::print("Removido com sucesso o cliente {} !\n", client.sName);
	_getch();

}


void ShowClients()
{
	std::cin.clear();
	fmt::print("Digite a quantidade de clientes que voce deseja ver: ");
	int limit = 1;
	std::cin >> limit;

	auto clients = db.GetLastClients(limit);

	system("cls");
	if (clients.empty())
	{
		fmt::print("[!] Nenhum cliente registrado.\n");
		_getch();
		return;
	}



	// Acaba saindo mais rapido colocar tudo em uma string e imprimi-la do que imprimir um por um, isso ocupa uma boa memoria depedendo da quantidade(o preço a se pagar pela velocidade)
	// Para imprimir 110k de clientes um por um demorou 11201ms(11,2segundos), e salvando na string e imprimindo de uma vez demorou menos de 2 segundos.
	std::string output;
	fmt::print("{:<8} | {:<4}\n", "ID", "Nome");
	for (auto& client : clients)
	{
		output += fmt::format("{:<8} | {}\n", client.clientid, client.sName);
	}

	//Aqui limpamos o vector contento os clientes, pois em grandes quantidades ocupa uma memoria do caralho, e como dependemos que o usuario aperte alguma tecla para sair da tela e liberar a memoria, isso pode demorar um tempo inderteminado ocupando memoria atoa. 
	clients.clear();

	fmt::print(output);
	_getch();
}

void showCount()
{
	const int count = db.ClientsCount();
	fmt::print("Temos registrado em nosso banco de dados {} clientes.\n", count);
	_getch();
}

