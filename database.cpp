#include "database.hpp"


bool Database::Connect(const std::string& host, const std::string& user, const std::string& pass, const std::string& database_name)
{
	try 
	{
		this->driver = sql::mysql::get_driver_instance();
		this->con = driver->connect(host, user, pass);
		this->con->setSchema(database_name);

		std::unique_ptr<sql::Statement> stmt(con->createStatement());
		stmt->execute(
			"CREATE TABLE IF NOT EXISTS clients ("
			"client_id INTEGER AUTO_INCREMENT PRIMARY KEY,"
			"client_name VARCHAR(255) NOT NULL,"
			"client_password VARCHAR(64) NOT NULL,"
			"client_email TEXT,"
			"client_address VARCHAR(255),"
			"client_age INTEGER"
			");"
		);
		return true;
	}
	catch (sql::SQLException)
	{
		return false;
	}

}

Database::ClientID Database::InsertClient(const Database::Client& client) 
{
	try 
	{
		if (!con->isValid())
			return -1;

		static const std::string query = "INSERT INTO clients (client_name, client_password, client_email, client_address, client_age) VALUES (?, ?, ?, ?, ?);";

		std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
		pstmt->setString(1, client.sName);
		pstmt->setString(2, client.sPass);
		pstmt->setString(3, client.sEmail);
		pstmt->setString(4, client.sAddress);
		pstmt->setInt(5, client.iAge);

		pstmt->executeUpdate();

		std::unique_ptr<sql::Statement> stmt(con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID() AS last_id;"));

		int id = -1;
		if (res->next())
		{
			id = res->getInt("last_id");
		}
	
		return id;
	}
	catch (sql::SQLException& e)
	{
		fmt::print("[MYSQL ERROR {}] File: {}:{} -> {}\n", e.getErrorCode(), __FILE__, __LINE__, e.what());
		//fmt::print("[MYSQL ERROR {}] File: {}:{} -> {}\n", e.getErrorCode(), __FILE__, __LINE__, e.what());
		return -1;
	}
}

bool Database::DeleteClient(const ClientID clientID) {
	try 
	{

		if (!con->isValid())
			return false;

		static const std::string query = "DELETE FROM clients WHERE client_id=?;";
		std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
		pstmt->setInt(1, clientID);
		pstmt->execute();

		return true;


	}
	catch (sql::SQLException& e) 
	{
		fmt::print("[MYSQL ERROR {}] File: {}:{} -> {}\n", e.getErrorCode(), __FILE__, __LINE__, e.what());
		return false;
	}
}

Database::Client Database::GetClientByID(ClientID clientid) {
	try
	{

		if (!con->isValid())
			return Client{};

		static const std::string query = "SELECT * FROM clients WHERE client_id=?;";
		std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
		pstmt->setInt(1, clientid);
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		Client client{};
		if (res->next()) 
		{
			client.clientid = clientid;
			client.sName = res->getString("client_name");
			client.sPass = res->getString("client_password");
			client.sEmail = res->getString("client_email");
			client.sAddress = res->getString("client_address");
			client.iAge = res->getInt("client_age");
		}
 
		return client;


	}
	catch (sql::SQLException& e)
	{
		fmt::print("[MYSQL ERROR {}] File: {}:{} -> {}\n", e.getErrorCode(), __FILE__, __LINE__, e.what());
	}
	return Client{};
}


std::vector<Database::Client> Database::GetClientsByName(const std::string& name) {
	std::vector<Client> clients{};
	try
	{

		if (!con->isValid())
			return clients;

		static const std::string query = "SELECT * FROM clients WHERE client_name=?;";
		std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
		pstmt->setString(1, name);
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		Client client{};
		while (res->next()) 
		{
			client.clientid = res->getInt("client_id");
			client.sName = res->getString("client_name");
			client.sPass = res->getString("client_password");
			client.sEmail = res->getString("client_email");
			client.sAddress = res->getString("client_address");
			client.iAge = res->getInt("client_age");
			clients.push_back(client);
		}
	}
	catch (sql::SQLException& e)
	{
		fmt::print("[MYSQL ERROR {}] File: {}:{} -> {}\n", e.getErrorCode(), __FILE__, __LINE__, e.what());
	}
	return clients;
}

int Database::ClientsCount()
{
	try
	{
		if (!con->isValid())
			return 0;

		std::unique_ptr<sql::Statement> stmt(con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT COUNT(*) AS clients_count FROM clients"));

		if (res->next())
		{
			return 	res->getInt("clients_count");
		}
	}
	catch (sql::SQLException& e)
	{
		fmt::print("[MYSQL ERROR {}] File: {}:{} -> {}\n", e.getErrorCode(), __FILE__, __LINE__, e.what());
	}
	return 0;
}

std::vector<Database::Client> Database::GetLastClients(int limit) {
	std::vector<Client> clients{};
	try
	{

		if (!con->isValid())
			return clients;

		static const std::string query = "SELECT * FROM clients LIMIT ?;";
		std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
		pstmt->setInt(1, limit);
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		Client client{};
		while (res->next())
		{
			client.clientid = res->getInt("client_id");
			client.sName = res->getString("client_name");
			client.sPass = res->getString("client_password");
			client.sEmail = res->getString("client_email");
			client.sAddress = res->getString("client_address");
			client.iAge = res->getInt("client_age");
			clients.push_back(client);
		}
	}
	catch (sql::SQLException& e)
	{
		fmt::print("[MYSQL ERROR {}] File: {}:{} -> {}\n", e.getErrorCode(), __FILE__, __LINE__, e.what());
	}
	return clients;
}