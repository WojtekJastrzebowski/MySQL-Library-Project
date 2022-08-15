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
//////////////Here, enter your Schema name, which you created earlier////////////////////////////////////
        con->setSchema("librarydb");
//////////////Create tables, their columns, indexes, links, etc. for our DB//////////////////////////////


        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS library_adress");
        cout << "library_adress table removed (if existed)" << endl;
        stmt->execute("CREATE TABLE library_adress (library_adress_ID INT(10) PRIMARY KEY NOT NULL, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");
        cout << "library_adress table has been created" << endl;

        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS employee");
        cout << "employee table removed (if existed)" << endl;
        stmt->execute("CREATE TABLE employee (employee_ID INT(10) PRIMARY KEY NOT NULL, name VARCHAR(50) NOT NULL, position VARCHAR(50) NOT NULL);");
        cout << "employee table has been created" << endl;

        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS employee_adress");
        cout << "employee_adress table removed (if existed)" << endl;
        stmt->execute("CREATE TABLE employee_adress (employee_adress_ID INT(10) PRIMARY KEY NOT NULL, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");
        cout << "employee_adress table has been created" << endl;

        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS customer");
        cout << "customer table removed (if existed)" << endl;
        stmt->execute("CREATE TABLE customer (customer_ID INT(10) PRIMARY KEY NOT NULL, name VARCHAR(50) NOT NULL, registration_date DATE NOT NULL, status_toomuch VARCHAR(25), status_toolong VARCHAR(25));");
        cout << "customer table has been created" << endl;

        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS customer_adress");
        cout << "customer_adress table removed (if existed)" << endl;
        stmt->execute("CREATE TABLE customer_adress (customer_adress_ID INT(10) PRIMARY KEY NOT NULL, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");
        cout << "customer_adress table has been created" << endl;

        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS book");
        cout << "book table removed (if existed)" << endl;
        stmt->execute("CREATE TABLE book (book_ID INT(10) PRIMARY KEY NOT NULL, isbn VARCHAR(25) NOT NULL, title VARCHAR(50) NOT NULL, author VARCHAR(50) NOT NULL, publisher VARCHAR(25) NOT NULL, genre VARCHAR(25) NOT NULL, quantity_available INT(10) NOT NULL, status VARCHAR(25) NOT NULL);");
        cout << "book table has been created" << endl;

        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS rental_status");
        cout << "rental_status table removed (if existed)" << endl;
        stmt->execute("CREATE TABLE rental_status (rental_status_ID INT(10) PRIMARY KEY NOT NULL, rent_customer_ID INT(10) NOT NULL, rent_book_ID INT(10) NOT NULL, rent_date DATE NOT NULL, CONSTRAINT FOREIGN KEY (rent_book_ID) REFERENCES book (book_ID), CONSTRAINT FOREIGN KEY (rent_customer_ID) REFERENCES customer (customer_ID));");
        cout << "rental_status table has been created" << endl;

        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS return_status");
        cout << "return_status table removed (if existed)" << endl;
        stmt->execute("CREATE TABLE return_status (return_status_ID INT(10) PRIMARY KEY NOT NULL, return_customer_ID INT(10) NOT NULL, return_book_ID INT(10) NOT NULL, return_date DATE NOT NULL, CONSTRAINT FOREIGN KEY (return_book_ID) REFERENCES book (book_ID), CONSTRAINT FOREIGN KEY (return_customer_ID) REFERENCES rental_status (rent_customer_ID));");
        cout << "return_status table has been created" << endl;

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