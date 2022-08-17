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
        sql::ResultSet* res;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);
//////////////Here, enter your Schema name, which you created earlier in your DB////////////////////////////////////
        con->setSchema("librarydb");
//////////////Create tables, their columns, indexes, links, etc. for our libraryDB (if not existed)//////////////////////////////
        cout << "Creating libraryDB tables...\n\n";
        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS library_adress (library_adress_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS employee (employee_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, name VARCHAR(50) NOT NULL, position VARCHAR(50) NOT NULL, username VARCHAR(50) NOT NULL, password VARCHAR(64) NOT NULL);");        
        stmt->execute("CREATE TABLE IF NOT EXISTS employee_adress (employee_adress_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");  
        stmt->execute("CREATE TABLE IF NOT EXISTS customer (customer_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, name VARCHAR(50) NOT NULL, registration_date DATE NOT NULL, status_toomuch VARCHAR(25), status_toolong VARCHAR(25));");
        stmt->execute("CREATE TABLE IF NOT EXISTS customer_adress (customer_adress_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS book (book_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, isbn VARCHAR(25) NOT NULL, title VARCHAR(50) NOT NULL, author VARCHAR(50) NOT NULL, publisher VARCHAR(25) NOT NULL, genre VARCHAR(25) NOT NULL, quantity_available INT(10) NOT NULL, status VARCHAR(25));");
        stmt->execute("CREATE TABLE IF NOT EXISTS rental_status (rental_status_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, rent_customer_ID INT(10) NOT NULL, rent_book_ID INT(10) NOT NULL, rent_date DATE NOT NULL, CONSTRAINT FOREIGN KEY (rent_book_ID) REFERENCES book (book_ID), CONSTRAINT FOREIGN KEY (rent_customer_ID) REFERENCES customer (customer_ID));");
        stmt->execute("CREATE TABLE IF NOT EXISTS return_status (return_status_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, return_customer_ID INT(10) NOT NULL, return_book_ID INT(10) NOT NULL, return_date DATE NOT NULL, CONSTRAINT FOREIGN KEY (return_book_ID) REFERENCES book (book_ID), CONSTRAINT FOREIGN KEY (return_customer_ID) REFERENCES rental_status (rent_customer_ID));");
        delete stmt;

        string optioncheck1, emname, emposition, emadress, emzip, emcity, emcountry, username, username1, password, password1;
        int count = 0, option1;
        string check1;
        string isbn, title, author, publisher, genre;
        int quantity_available;
        
    loop1:

        cout << "Welcome to libraryDB. Please logIn or Register as employee:\n\n";
        cout << "1. LogIn\n"; 
        cout << "2. Register\n";
        cout << "0. Exit\n\n";
        cout << "Choose option: ";
        cin >> optioncheck1;
        for (int i = 0; i < optioncheck1.length(); i++)
        {
            if (isdigit(optioncheck1[i]) == false)
            {
                count = 1;
                break;
            }
            else
                count = 0;
        }
        if (count == 0) {
            int option1 = stoi(optioncheck1);
            do {
                switch (option1) {
                case 1:
                    cout << "Your username: ";
                    cin >> username1;
                    cout << "Your password: ";
                    cin >> password1;
                        stmt = con->createStatement();
                        res = stmt->executeQuery("SELECT username, password FROM employee ORDER BY name ASC");
                        while (res->next()) {
                            string usernameDB = res->getString("username");
                            string passwordDB = res->getString("password");
                            if (usernameDB == username1 && passwordDB == password1) {
                                cout << "\nLogin successful..!\n\n";
                                goto libraryDB;
                            }                     
                        }
                        cout << "\nLogin failed..!\n\n";
                        delete res;
                        delete stmt;
                    goto loop1;
                case 2:
                    cout << "Name: ";
                    cin >> emname;
                    cout << "Your position: ";
                    cin >> emposition;
                    cout << "Adress: ";
                    cin >> emadress;
                    cout << "ZipCode: ";
                    cin >> emzip;
                    cout << "City: ";
                    cin >> emcity;
                    cout << "Country: ";
                    cin >> emcountry;
                    cout << "Choose your username: ";
                    cin >> username;
                    cout << "Choose your password: ";
                    cin >> password;

                    pstmt = con->prepareStatement("INSERT INTO employee(name, position, username, password) VALUES(?,?,?,?)");
                    pstmt->setString(1, emname);
                    pstmt->setString(2, emposition);
                    pstmt->setString(3, username);
                    pstmt->setString(4, password);
                    pstmt->execute();

                    pstmt = con->prepareStatement("INSERT INTO employee_adress(adress, zipcode, city, country) VALUES(?,?,?,?)");
                    pstmt->setString(1, emadress);
                    pstmt->setString(2, emzip);
                    pstmt->setString(3, emcity);
                    pstmt->setString(4, emcountry);
                    pstmt->execute();

                    cout << "\nRegistration successful..!\n\n";
                    goto loop1;
                case 0:
                    system("pause");
                    return 0;
                default:
                    cout << "The wrong parameter has been specified!\n\n";      
                    goto loop1;
                }
            } while (option1);
        }
        else {
            cout << "The wrong parameter has been specified!\n\n";
            goto loop1;
        }
    libraryDB:
        cout << "LibraryDB. Please choose action:\n\n";
        cout << "1. Add New Book Rental\n";
        cout << "2. Add New Book Return\n";
        cout << "3. Check Book Availability\n";
        cout << "4. Check Customer Status\n";
        cout << "5. Check Book Return\n";
        cout << "6. Check Book Rentals\n";
        cout << "7. Add Book\n";
        cout << "8. Modify Book\n";
        cout << "9. Remove Book\n";
        cout << "0. Logout\n\n";
        cout << "Choose option: ";
        cin >> optioncheck1;
        for (int i = 0; i < optioncheck1.length(); i++)
        {
            if (isdigit(optioncheck1[i]) == false)
            {
                count = 1;
                break;
            }
            else
                count = 0;
        }
        if (count == 0) {
            int option1 = stoi(optioncheck1);
            do {
                switch (option1) {
                case 1:
                    //add data
                    //pstmt exec
                    //done
                    goto libraryDB;
                case 2:
                    //add data
                    //pstmt exec
                    //done
                    goto libraryDB;
                case 3:
                    //1.list all data
                    //2.choose book
                    goto libraryDB;
                case 4:
                    //1.list all data
                    //2.search?
                    goto libraryDB;
                case 5:
                    //1.list all data
                    //2.search?
                    goto libraryDB;
                case 6:
                    //1.list all data
                    //2.search?
                    goto libraryDB;
                case 7:              
                    cout << "Add new book: \n";
                    cin.ignore();
                    cout << "ISBN: ";
                    getline(cin, isbn);
                    cout << "Title: ";
                    getline(cin, title);
                    cout << "Author: ";
                    getline(cin, author);
                    cout << "Publisher: ";
                    getline(cin, publisher);
                    cout << "Genre: ";
                    getline(cin, genre);
                    
                    cout << "Quantity: ";
                    cin >> quantity_available;
                    
                    pstmt = con->prepareStatement("INSERT INTO book(isbn, title, author, publisher, genre, quantity_available) VALUES(?,?,?,?,?,?)");
                    pstmt->setString(1, isbn);
                    pstmt->setString(2, title);
                    pstmt->setString(3, author);
                    pstmt->setString(4, publisher);
                    pstmt->setString(5, genre);
                    pstmt->setInt(6, quantity_available);
                    pstmt->execute();

                    cout << "\nThe addition of the book was successful..!\n";
                    goto libraryDB;
                case 8:
                    //output all data (name etc)
                    //input name of book
                    //modify alt->input new, for quantity +/- int
                    //confirm
                    goto libraryDB; 
                case 9:
                    //output all data (name etc)
                    //input name of book
                    //delete
                    //confirm
                        goto loop1;
                case 0:
                    goto loop1;
                default:
                    cout << "The wrong parameter has been specified!\n\n";
                    goto libraryDB;
                }
            } while (option1);
        }
        else {
            cout << "The wrong parameter has been specified!\n\n";
            goto libraryDB;
        }
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