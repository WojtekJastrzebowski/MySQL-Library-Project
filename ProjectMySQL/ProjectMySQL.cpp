/*
    Title			: C++ & MySQL LinkProject
    Language		: C++
    Author			: Wojciech Jastrzebowski
    Date Created	: 13-08-2022
*/
#include <stdlib.h>
#include <iostream>
//////////////MySQL header files/////////////////////////////////////////////////////////////////////////
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;
//////////////Enter your DB data here////////////////////////////////////////////////////////////////////
const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "";
//////////////Main function Start////////////////////////////////////////////////////////////////////////
int main() {
//////////////Attempt to connection with DB//////////////////////////////////////////////////////////////
    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* pstmt;
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
//////////////Here, enter your Schema name, which you created earlier////////////////////////////////////
        con->setSchema("bookshopDB");
//////////////Create tables, their columns, indexes, links, etc. for our DB//////////////////////////////
        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS inventory");
        cout << "Finished dropping table (if existed)" << endl;
        stmt->execute("CREATE TABLE inventory (id serial PRIMARY KEY, name VARCHAR(50), quantity INTEGER);");
        cout << "Finished creating table" << endl;
        delete stmt;
    
        pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?,?)");
        pstmt->setString(1, "banana");
        pstmt->setInt(2, 150);
        pstmt->execute();
        cout << "One row inserted." << endl;
    
        pstmt->setString(1, "orange");
        pstmt->setInt(2, 154);
        pstmt->execute();
        cout << "One row inserted." << endl;
    
        pstmt->setString(1, "apple");
        pstmt->setInt(2, 100);
        pstmt->execute();
        cout << "One row inserted." << endl;
    
        delete pstmt;
        delete con;
    }
    catch (sql::SQLException& e){
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    system("pause");
    return 0;
}