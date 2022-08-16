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
const string password = "abc1221?!W";
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
//////////////Here, enter your Schema name, which you created earlier in your DB////////////////////////////////////
        con->setSchema("librarydb");
//////////////Create tables, their columns, indexes, links, etc. for our libraryDB (if not existed)//////////////////////////////
        cout << "Creating libraryDB tables...\n\n";
        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS library_adress (library_adress_ID INT(10) PRIMARY KEY NOT NULL, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS employee (employee_ID INT(10) PRIMARY KEY NOT NULL, name VARCHAR(50) NOT NULL, position VARCHAR(50) NOT NULL, username VARCHAR(50) NOT NULL, password VARCHAR(64) NOT NULL);");        
        stmt->execute("CREATE TABLE IF NOT EXISTS employee_adress (employee_adress_ID INT(10) PRIMARY KEY NOT NULL, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");  
        stmt->execute("CREATE TABLE IF NOT EXISTS customer (customer_ID INT(10) PRIMARY KEY NOT NULL, name VARCHAR(50) NOT NULL, registration_date DATE NOT NULL, status_toomuch VARCHAR(25), status_toolong VARCHAR(25));");
        stmt->execute("CREATE TABLE IF NOT EXISTS customer_adress (customer_adress_ID INT(10) PRIMARY KEY NOT NULL, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS book (book_ID INT(10) PRIMARY KEY NOT NULL, isbn VARCHAR(25) NOT NULL, title VARCHAR(50) NOT NULL, author VARCHAR(50) NOT NULL, publisher VARCHAR(25) NOT NULL, genre VARCHAR(25) NOT NULL, quantity_available INT(10) NOT NULL, status VARCHAR(25) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS rental_status (rental_status_ID INT(10) PRIMARY KEY NOT NULL, rent_customer_ID INT(10) NOT NULL, rent_book_ID INT(10) NOT NULL, rent_date DATE NOT NULL, CONSTRAINT FOREIGN KEY (rent_book_ID) REFERENCES book (book_ID), CONSTRAINT FOREIGN KEY (rent_customer_ID) REFERENCES customer (customer_ID));");
        stmt->execute("CREATE TABLE IF NOT EXISTS return_status (return_status_ID INT(10) PRIMARY KEY NOT NULL, return_customer_ID INT(10) NOT NULL, return_book_ID INT(10) NOT NULL, return_date DATE NOT NULL, CONSTRAINT FOREIGN KEY (return_book_ID) REFERENCES book (book_ID), CONSTRAINT FOREIGN KEY (return_customer_ID) REFERENCES rental_status (rent_customer_ID));");
        cout << "Welcome to libraryDB. Please logIn or Register as employee:\n\n";
        cout << "1. LogIn\n"; 
        cout << "2. Register\n";
        cout << "0. Exit\n\n";
        int option1;
        switch (option1) {
        case 1:
            cout << "Your username: ";
            cout << "Your password: ";
            break;
        case 2:
            cout << "First name: ";
            cout << "Name: ";
            cout << "Your position: ";
            cout << "Adress: ";
            cout << "ZipCode: ";
            cout << "City: ";
            cout << "Country: ";
            cout << "Your position: ";
            break;
        case 0:
            // code block
            break;
        default:
            // code block
        }


        delete stmt;
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