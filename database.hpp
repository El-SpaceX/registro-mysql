#include <mysql/jdbc.h>
#include <fmt/printf.h>
#include <string>
#include <vector>
#include <memory>

class Database 
{
public:
	typedef int ClientID;
	struct Client
	{
		ClientID clientid;
		std::string sName;
		std::string sPass;
		std::string sEmail;
		std::string sAddress;
		unsigned short iAge;
	};

	~Database() { delete this->con; }
	bool Connect(const std::string& host, const std::string& user, const std::string& pass, const std::string& database_name);




	Client GetClientByID(ClientID clientid);
	std::vector<Client> GetClientsByName(const std::string& name);
	std::vector<Database::Client> GetLastClients(int limit);
	int ClientsCount();


	ClientID InsertClient(const Client& client);
	bool DeleteClient(const ClientID clientID);
private:
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;
};