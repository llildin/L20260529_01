#include <iostream>

#include "jdbc/mysql_connection.h"
#include "jdbc/cppconn/driver.h"
#include "jdbc/cppconn/exception.h"
#include "jdbc/cppconn/resultset.h"
#include "jdbc/cppconn/statement.h"
#include "jdbc/cppconn/prepared_statement.h"

#ifndef _DEBUG
#pragma comment(lib, "mysqlcppconn")
#else
#pragma comment(lib, "debug/mysqlcppconn")
#endif //_DEBUG

int main()
{
	std::string UserID = "junios";
	std::string Password = "hello";
	try
	{
		sql::Driver* MyDriver;
		sql::Connection* MyConnection;
		sql::Statement* MyStatement;
		sql::ResultSet* MyResultSet;
		sql::PreparedStatement* MyPreparedStatement;

		MyDriver = get_driver_instance();
		MyConnection = MyDriver->connect("tcp://127.0.0.1", "lildin", "kckc9795@@");

		MyConnection->setSchema("membership");
		MyStatement = MyConnection->createStatement();
		//sql::SQLString Query = "select * from user where `user_id` = '";
		//Query = Query + UserID;
		//Query = Query + "' and `user_pw` = sha2('";
		//Query = Query + Password;
		//Query = Query + "', 512) and is_delete = 'N';";
		//MyResultSet = MyStatement->executeQuery(Query);

		sql::SQLString Query = "select * from user where `user_id` = ? and `user_pw` = sha2(? , 512) and is_delete = 'N';";
		MyPreparedStatement = MyConnection->prepareStatement(Query);
		MyPreparedStatement->setString(1, UserID);
		MyPreparedStatement->setString(2, Password);

		MyResultSet = MyPreparedStatement->executeQuery();

		if (MyResultSet->getRow() == 0)
		{
			std::cout << "아이디 혹은 비번이 틀립니다." << std::endl;
		}

		for (; MyResultSet->next();)
		{
			std::cout << MyResultSet->getInt("idx") << std::endl;
			std::cout << MyResultSet->getString("user_id") << std::endl;
			std::cout << MyResultSet->getString("user_pw") << std::endl;
			std::cout << MyResultSet->getString("name") << std::endl;
			std::cout << MyResultSet->getString("created_at") << std::endl;
			std::cout << MyResultSet->getInt("is_delete") << std::endl;
		}
	}
	catch (sql::SQLException Exception)
	{
		std::cout << Exception.what() << std::endl;
		std::cout << Exception.getSQLState() << std::endl;
	}

	return 0;
}