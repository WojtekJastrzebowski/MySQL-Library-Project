/*
    Title			: C++ & MySQL LinkProject
    Language		: C++
    Author			: Wojciech Jastrzebowski
    Date Created	: 13-08-2022
*/
#include <stdlib.h>
#include <iostream>
#include <iomanip>
//////////////MySQL header files/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

#include "sha256.h"
#include "sha256.cpp"

#include "functions.h"
#include "date.h"

using namespace std;
//////////////Enter your DB data here///////////////////////////////////////////////////////////////////////////////////////////////////
const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "";
//////////////Main function Start///////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
//////////////Attempt to connection with DB/////////////////////////////////////////////////////////////////////////////////////////////
    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* pstmt;
        sql::ResultSet* res;
        SHA256 sha256;

        driver = get_driver_instance();
        con = driver->connect(server, username, password);
//////////////Here, enter your Schema name, which you created earlier in your DB////////////////////////////////////
        con->setSchema("librarydb");
//////////////Create tables, their columns, indexes, links, etc. for our libraryDB//////////////////////////////
        cout << "Creating libraryDB tables...\n\n";
        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS library_adress (library_adress_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(50) NOT NULL, country VARCHAR(50) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS employee (employee_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, name VARCHAR(50) NOT NULL, position VARCHAR(50) NOT NULL, username VARCHAR(50) NOT NULL, password VARCHAR(64) NOT NULL);");        
        stmt->execute("CREATE TABLE IF NOT EXISTS employee_adress (employee_adress_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(50) NOT NULL, country VARCHAR(50) NOT NULL);");  
        stmt->execute("CREATE TABLE IF NOT EXISTS customer (customer_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, name VARCHAR(50) NOT NULL, registration_date VARCHAR(50) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS customer_adress (customer_adress_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, adress VARCHAR(50) NOT NULL, zipcode VARCHAR(25) NOT NULL, city VARCHAR(35) NOT NULL, country VARCHAR(25) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS book (book_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, isbn VARCHAR(50) NOT NULL, title VARCHAR(50) NOT NULL, author VARCHAR(50) NOT NULL, publisher VARCHAR(50) NOT NULL, genre VARCHAR(50) NOT NULL, quantity_available INT(10) NOT NULL);");
        stmt->execute("CREATE TABLE IF NOT EXISTS rental_status (rental_status_ID INT(10) PRIMARY KEY NOT NULL AUTO_INCREMENT, rent_customer_ID INT(10) NOT NULL, rent_book_ID INT(10) NOT NULL, rent_quantity INT(10) NOT NULL DEFAULT 0, rent_date VARCHAR(50), is_returned VARCHAR(25) DEFAULT 'No', return_quantity INT(10) DEFAULT 0, last_return_date VARCHAR(50), CONSTRAINT FOREIGN KEY (rent_book_ID) REFERENCES book (book_ID), CONSTRAINT FOREIGN KEY (rent_customer_ID) REFERENCES customer (customer_ID));");
        delete stmt;

        string optioncheck1, emname, emposition, emadress, emzip, emcity, emcountry, cuname, cuadress, 
               cuzip, cucity, cucountry, username, username1, password, dbookid,
               password1, hashpw, hashpw1, isbn, title, author, publisher, genre, quantity_available, date,
               returnbookid, returnquantity, rentid, rentcustid, rentbookid, rentquantity, bookid4s;

        int rentcustid1, rentbookid1, rentquantity1, returnbookid1, returnquantity1, rentid1, dbookid1;
        int count = 0, option1 = 0, quantity_available1 = 0, bookid3 = 0, custid3 = 0, bookid4 = 0, change2 = 0;
//////////////Main menu with registration and login///////////////////////////////////////////////////////////////////////////////////
    loop1:
        cout << "Welcome to libraryDB. Please logIn or Register as employee:\n\n";
        cout << "*********** 1. LogIn\n"; 
        cout << "*********** 2. Register\n";
        cout << "*********** 0. Exit\n\n";
        cout << "*********** Choose option: ";
        cin >> optioncheck1;
        system("cls");
        if (IntCheck(optioncheck1)) {
            int option1 = stoi(optioncheck1);
            do {
                switch (option1) {
                case 1:
                    cin.ignore();
                    cout << "*********** Your username: ";
                    getline(cin, username1);
                    cout << "*********** Your password: ";
                    getline(cin, password1);
                    hashpw1 = sha256(password1);
                        stmt = con->createStatement();
                        res = stmt->executeQuery("SELECT username, password FROM employee ORDER BY name ASC");
                        while (res->next()) {
                            string usernameDB = res->getString("username");
                            string passwordDB = res->getString("password");
                            if (usernameDB == username1 && passwordDB == hashpw1) {
                                cout << "\n*********** Login successful..!\n\n";
                                system("pause");
                                system("cls");
                                goto libraryDB;
                            }                     
                        }
                        cout << "\nLogin failed..!\n\n";
                        delete res;
                        delete stmt;
                        system("pause");
                        system("cls");
                    goto loop1;
                case 2:
                    cin.ignore();
                    cout << "*********** Registration of a new employee\n\n*********** Name (required): ";
                    getline(cin, emname);
                    if (!IsFilled(emname)) {Message(); goto loop1; }
                    cout << "*********** Your position (required): ";
                    getline(cin, emposition);
                    if (!IsFilled(emposition)) {Message(); goto loop1; }
                    cout << "*********** Adress (required): ";
                    getline(cin, emadress);
                    if (!IsFilled(emadress)) {Message(); goto loop1; }
                    cout << "*********** ZipCode (required): ";
                    getline(cin, emzip);
                    if (!IsFilled(emzip)) {Message(); goto loop1; }
                    cout << "*********** City (required): ";
                    getline(cin, emcity);
                    if (!IsFilled(emcity)) {Message(); goto loop1; }
                    cout << "*********** Country (required): ";
                    getline(cin, emcountry);
                    if (!IsFilled(emcountry)) {Message(); goto loop1; }
                    cout << "*********** Choose your username (8-20)(required): ";
                    getline(cin, username);
                    cout << "*********** Choose your password (8-20)(required): ";
                    getline(cin, password);
                    if(IsCorrect(username, password)){
                        stmt = con->createStatement();
                        res = stmt->executeQuery("SELECT username FROM employee ORDER BY name ASC");
                        while (res->next()) {
                            string usernameDB = res->getString("username"); 
                            if (usernameDB == username) {
                                cout << "\n*********** Registration failed..!\n*********** Username already in database..!\n\n";
                                system("pause");
                                system("cls");
                                goto libraryDB;
                            }
                        }
                        delete res;
                        delete stmt;
                        hashpw = sha256(password);
                        pstmt = con->prepareStatement("INSERT INTO employee(name, position, username, password) VALUES(?,?,?,?)");
                        pstmt->setString(1, emname);
                        pstmt->setString(2, emposition);
                        pstmt->setString(3, username);
                        pstmt->setString(4, hashpw);
                        pstmt->execute();
                        pstmt = con->prepareStatement("INSERT INTO employee_adress(adress, zipcode, city, country) VALUES(?,?,?,?)");
                        pstmt->setString(1, emadress);
                        pstmt->setString(2, emzip);
                        pstmt->setString(3, emcity);
                        pstmt->setString(4, emcountry);
                        pstmt->execute();
                        delete pstmt;
                        cout << "\n*********** Registration successful..!\n\n";
                        system("pause");
                        system("cls");
                        goto loop1;
                    }
                    else {
                        Message();
                        goto loop1;
                    }
                case 0:
                    system("pause");
                    return 0;
                default:
                    Message();
                    goto loop1;
                }
            } while (option1);
        }
        else {
            Message();
            goto loop1;
        }
//////////////Loop for library after succesfull login///////////////////////////////////////////////////////////////////////////////////
    libraryDB:
        cout << "*********** LibraryDB \n\n";
        cout << "*********** 1. Add New Book Rental\n";
        cout << "*********** 2. Add New Book Return\n";
        cout << "*********** 3. Check Book Availability\n";
        cout << "*********** 4. Check Customer Status\n";
        cout << "*********** 5. Check Book Rentals\n";
        cout << "*********** 6. Add Customer\n";
        cout << "*********** 7. Add Book\n";
        cout << "*********** 8. Modify Book\n";
        cout << "*********** 0. Logout\n\n";
        cout << "*********** Choose option: ";
        cin >> optioncheck1;
        system("cls");
        if (IntCheck(optioncheck1)) {
            int option1 = stoi(optioncheck1);
            do {
                switch (option1) {
                case 1:
//////////////New book rental///////////////////////////////////////////////////////////////////////////////////

                    cout << "*********** Registration of a new book rental: \n";
                    cin.ignore();
                    cout << "*********** Book ID (required): ";
                    getline(cin, rentbookid);
                    if (!IsFilled(rentbookid)) { Message(); goto libraryDB; }
                    if (IntCheck(rentbookid)) {
                        rentbookid1 = stoi(rentbookid);
                    }
                    else {
                        Message(); goto libraryDB;
                    }
                    cout << "*********** Quantity (required): ";
                    getline(cin, rentquantity);
                    if (!IsFilled(rentquantity)) { Message(); goto libraryDB; }
                    if (IntCheck(rentquantity)) {
                        rentquantity1 = stoi(rentquantity);
                    }
                    else {
                        Message(); goto libraryDB;
                    }
                    count == 1;
                    stmt = con->createStatement();
                    res = stmt->executeQuery("SELECT book_ID, quantity_available FROM book ORDER BY book_ID ASC");
                    while (res->next() && count == 1) {
                        int rentbookid1DB = res->getInt("book_ID");
                        int rentquantity1DB = res->getInt("quantity_available");
                        if (rentbookid1DB == rentbookid1) {
                            count = 0;
                            if (rentquantity1DB < rentquantity1) {
                                cout << "\n*********** Operation failed..!\n*********** Not enough books..!\n\n";
                                system("pause");
                                system("cls");
                                goto libraryDB;
                            }
                        }
                    }
                    delete stmt;
                    delete res;
                    if (count == 1) {
                        cout << "\n*********** Operation failed..!\n*********** Book ID not in database..!\n\n";
                        system("pause");
                        system("cls");
                        goto libraryDB;
                    }       
                    cout << "*********** Customer ID (required): ";
                    getline(cin, rentcustid);
                    if (!IsFilled(rentcustid)) { Message(); goto libraryDB; }
                    if (IntCheck(rentcustid)) {
                        rentcustid1 = stoi(rentcustid);
                    }
                    else {
                        Message(); goto libraryDB;
                    }
                    count = 1;
                    stmt = con->createStatement();
                    res = stmt->executeQuery("SELECT customer_ID FROM customer ORDER BY customer_ID ASC");
                    while (res->next() && count == 1) {
                        string rentcustidDB = res->getString("customer_ID");
                        if (rentcustidDB == rentcustid) {
                            count = 0;
                            break;
                        }
                    }
                    delete stmt;
                    delete res;
                    if (count == 1) {
                        cout << "\n*********** Operation failed..!\n*********** Customer ID not in database..!\n\n";
                        system("pause");
                        system("cls");
                        goto libraryDB;
                    }
                    date = date::format("%F", chrono::system_clock::now());
                    pstmt = con->prepareStatement("INSERT INTO rental_status(rent_customer_ID, rent_book_ID, rent_date, rent_quantity) VALUES(?,?,?,?)");
                    pstmt->setInt(1, rentcustid1);
                    pstmt->setInt(2, rentbookid1);
                    pstmt->setString(3, date);
                    pstmt->setInt(4, rentquantity1);
                    pstmt->execute();
                    delete pstmt;
                    pstmt = con->prepareStatement("UPDATE book SET quantity_available = quantity_available - ? WHERE book_ID = ?");
                    pstmt->setInt(1, rentquantity1);
                    pstmt->setInt(2, rentbookid1);
                    pstmt->execute();
                    delete pstmt;
                    cout << "\n*********** Book rental registration successful..!\n\n";
                    system("pause");
                    system("cls");
                    goto libraryDB;
//////////////New book return///////////////////////////////////////////////////////////////////////////////////
                case 2:
                    cout << "*********** Registration of a new book return: \n";
                    cin.ignore();
                    cout << "*********** Rent ID (required): ";
                    getline(cin, rentid);
                    if (!IsFilled(rentid)) { Message(); goto libraryDB; }
                    if (IntCheck(rentid)) {
                        rentid1 = stoi(rentid);
                    }
                    else {
                        Message(); goto libraryDB;
                    }
                    cout << "*********** Book ID (required): ";
                    getline(cin, returnbookid);
                    if (!IsFilled(returnbookid)) { Message(); goto libraryDB; }
                    if (IntCheck(returnbookid)) {
                        returnbookid1 = stoi(returnbookid);
                    }
                    else {
                        Message(); goto libraryDB;
                    }
                    cout << "*********** Quantity (required): ";
                    getline(cin, returnquantity);
                    if (!IsFilled(returnquantity)) { Message(); goto libraryDB; }
                    if (IntCheck(returnquantity)) {
                        returnquantity1 = stoi(returnquantity);
                    }
                    else {
                        Message(); goto libraryDB;
                    }
                    count = 1;
                    stmt = con->createStatement();
                    res = stmt->executeQuery("SELECT rental_status_ID, rent_book_ID, rent_quantity, return_quantity FROM rental_status ORDER BY rental_status_ID ASC");
                    while (res->next() && count == 1) {
                        int rentid1DB = res->getInt("rental_status_ID");
                        int returnquantity1DB = res->getInt("rent_quantity");
                        int returnquantity2DB = res->getInt("return_quantity");
                        int returnbookid1DB = res->getInt("rent_book_ID");
                        if (rentid1DB == rentid1) {
                            count = 0;
                            if (returnbookid1DB != returnbookid1) {
                                cout << "\n*********** Operation failed..!\n*********** Book ID not in database..!\n\n";
                                system("pause");
                                system("cls");
                                goto libraryDB;
                            }
                            if (returnquantity1DB - returnquantity2DB > returnquantity1) {
                                count = 2;
                                break;
                            }
                            if (returnquantity1DB - returnquantity2DB < returnquantity1) {
                                cout << "\n*********** Operation failed..!\n*********** That's too many books..!\n\n";
                                system("pause");
                                system("cls");
                                goto libraryDB;
                            }
                            break;
                        }
                    }
                    delete stmt;
                    delete res;
                    if (count == 1) {
                        cout << "\n*********** Operation failed..!\n*********** Rent ID not in database..!\n\n";
                        system("pause");
                        system("cls");
                        goto libraryDB;
                    }         
                    date = date::format("%F", chrono::system_clock::now());
                    pstmt = con->prepareStatement("UPDATE rental_status SET last_return_date = ?, is_returned = ?, return_quantity = return_quantity + ? WHERE rental_status_ID = ?");
                    pstmt->setString(1, date);
                    if (count == 2) { 
                        pstmt->setString(2, "Not all");
                        cout << "\n*********** Only some of the books are returned..!\n\n";
                    }
                    else {
                        pstmt->setString(2, "Yes");
                    }
                    pstmt->setInt(3, returnquantity1);
                    pstmt->setInt(4, rentid1);
                    pstmt->execute();
                    delete pstmt;
                    pstmt = con->prepareStatement("UPDATE book SET quantity_available = quantity_available + ? WHERE book_ID = ?");
                    pstmt->setInt(1, returnquantity1);
                    pstmt->setInt(2, returnbookid1);
                    pstmt->execute();
                    delete pstmt;
                    cout << "\n*********** Book return registration successful..!\n\n";
                    system("pause");
                    system("cls");
                    goto libraryDB;
//////////////All books listing///////////////////////////////////////////////////////////////////////////////////
                case 3:
                    cout << "*********** 1. List all the books\n";
                    cout << "*********** 2. Check exact book\n";
                    cout << "*********** 3. Return\n";
                    cout << "*********** Choose Action: ";
                    cin >> optioncheck1;
                    system("cls");
                    if (IntCheck(optioncheck1)) {
                        int option1 = stoi(optioncheck1);
                        if (option1 == 1) {
                            stmt = con->createStatement();
                            res = stmt->executeQuery("SELECT book_ID, isbn, title, author, publisher, genre, quantity_available FROM book ORDER BY title ASC");
                            cout << endl << "|        Title        |" << "|        Author        |" << "|   ID   |" << "|   ISBN   |" << "|      Publisher      |"
                                << "|     Genre     |" << "|Quantity|" << endl << "-----------------------------------------------------------------------------------------------------------------------";
                            while (res->next()) {
                                int bookid2 = res->getInt("book_ID");
                                string isbn2 = res->getString("isbn");
                                string title2 = res->getString("title");
                                string author2 = res->getString("author");
                                string publisher2 = res->getString("publisher");
                                string genre2 = res->getString("genre");
                                int quantity2 = res->getInt("quantity_available");
                                cout << endl << "|" << setw(21) << title2 << "||" << setw(22) << author2 << "||" << setw(8) << bookid2 << "||" << setw(10) << isbn2
                                    << "||" << setw(21) << publisher2 << "||" << setw(15) << genre2 << "||" << setw(8) << quantity2 << "|" << endl;
                            }
                            delete stmt;
                            delete res;
                            system("pause");
                            system("cls");
                        }
                        if (option1 == 2) {
                            string bookid3s;
                            cout << "*********** Enter book ID: ";
                            cin >> bookid3s;
                            if (IntCheck(bookid3s)) {
                                bookid3 = stoi(bookid3s);
                            }
                            else {
                                Message();
                                goto libraryDB;
                            }
                            stmt = con->createStatement();
                            res = stmt->executeQuery("SELECT book_ID, isbn, title, author, publisher, genre, quantity_available FROM book ORDER BY title ASC");
                            while (res->next()) {
                                int bookid2 = res->getInt("book_ID");
                                if (bookid2 == bookid3) {
                                    string isbn2 = res->getString("isbn");
                                    string title2 = res->getString("title");
                                    string author2 = res->getString("author");
                                    string publisher2 = res->getString("publisher");
                                    string genre2 = res->getString("genre");
                                    int quantity2 = res->getInt("quantity_available");
                                    cout << endl << "|        Title        |" << "|        Author        |" << "|   ID   |" << "|   ISBN   |" << "|      Publisher      |"
                                        << "|     Genre     |" << "|Quantity|" << endl << "-----------------------------------------------------------------------------------------------------------------------";
                                    cout << endl << "|" << setw(21) << title2 << "||" << setw(22) << author2 << "||" << setw(8) << bookid2 << "||" << setw(10) << isbn2
                                        << "||" << setw(21) << publisher2 << "||" << setw(15) << genre2 << "||" << setw(8) << quantity2 << "|" << endl;
                                    system("pause");
                                    system("cls");
                                    break;
                                }
                            }
                            delete stmt;
                            delete res;
                        }
                        if (option1 == 3) {
                            goto libraryDB;
                        }
                    }
                    else {
                        Message();
                        goto libraryDB;
                    }
                    goto libraryDB;
//////////////All customers listing///////////////////////////////////////////////////////////////////////////////////
                case 4:
                    cout << "*********** 1. List all the customers\n";
                    cout << "*********** 2. Check exact customer\n";
                    cout << "*********** 3. Return\n";
                    cout << "*********** Choose Action: ";
                    cin >> optioncheck1;
                    system("cls");
                    if (IntCheck(optioncheck1)) {
                        int option1 = stoi(optioncheck1);
                        if (option1 == 1) {
                            stmt = con->createStatement();
                            res = stmt->executeQuery("SELECT customer_ID, name, registration_date, adress, zipcode, city, country FROM customer, customer_adress ORDER BY name ASC");
                            cout << endl << "|        Name        |" << "|  ID  |" << "|Registration date|"
                                << "|        Adress        |" << "|ZipCode|" << "|     City     |" << "|     Country     |"
                                << endl << "---------------------------------------------------------------------------------------------------------------------";
                            while (res->next()) {
                                int customerid2 = res->getInt("customer_ID");
                                string custname2 = res->getString("name");
                                string registration_date2 = res->getString("registration_date");
                                string custadress2 = res->getString("adress");
                                string zipcode2 = res->getString("zipcode");
                                string custcity2 = res->getString("city");
                                string custcountry2 = res->getString("country");
                                cout << endl << "|" << setw(20) << custname2 << "||" << setw(6) << customerid2 << "||" << setw(17) << registration_date2
                                    << "||" << setw(22) << custadress2 << "||" << setw(7) << zipcode2 << "||" << setw(14) << custcity2
                                    << "||" << setw(17) << custcountry2 << "|" << endl;
                            }
                            delete stmt;
                            delete res;
                            system("pause");
                            system("cls");
                        }
                        if (option1 == 2) {
                            string customerid2s;
                            cout << "*********** Enter customer ID: ";
                            cin >> customerid2s;
                            system("cls");
                            if (IntCheck(customerid2s)) {
                                custid3 = stoi(customerid2s);
                            }
                            else {
                                Message();
                                goto libraryDB;
                            }
                            stmt = con->createStatement();
                            res = stmt->executeQuery("SELECT customer_ID, name, registration_date, adress, zipcode, city, country FROM customer, customer_adress ORDER BY name ASC");
                            while (res->next()) {
                                int customeriddb2 = res->getInt("customer_ID");
                                if (custid3 == customeriddb2) {
                                    string custname2 = res->getString("name");
                                    string registration_date2 = res->getString("registration_date");
                                    string custadress2 = res->getString("adress");
                                    string zipcode2 = res->getString("zipcode");
                                    string custcity2 = res->getString("city");
                                    string custcountry2 = res->getString("country");
                                    cout << endl << "|        Name        |" << "|  ID  |" << "|Registration date|"
                                        << "|        Adress        |" << "|ZipCode|" << "|     City     |" << "|     Country     |"
                                        << endl << "---------------------------------------------------------------------------------------------------------------------";
                                    cout << endl << "|" << setw(20) << custname2 << "||" << setw(6) << custid3 << "||" << setw(17) << registration_date2
                                        << "||" << setw(22) << custadress2 << "||" << setw(7) << zipcode2 << "||" << setw(14) << custcity2
                                        << "||" << setw(17) << custcountry2 << "|" << endl;
                                    system("pause");
                                    system("cls");
                                    break;
                                }
                            }
                            delete stmt;
                            delete res;
                        }
                        if (option1 == 3) {
                            goto libraryDB;
                        }
                    }
                    else {
                        Message();
                        goto libraryDB;
                    }
                    goto libraryDB;
//////////////All rentals listing///////////////////////////////////////////////////////////////////////////////////
                case 5:
                    cout << "*********** All book rentals listed:\n\n";
                    stmt = con->createStatement();
                    res = stmt->executeQuery("SELECT * FROM rental_status ORDER BY rent_date ASC");
                    cout << endl << "|Rent  Date|" << "|Rental ID|" << "|Book ID|" << "|Customer ID|"
                        << "|Rent Quantity|" << "|Is Returned?|" << "|Return Quantity|" << "|Last return date|\n" << endl 
                        << "-------------------------------------------------------------------------------------------------------------\n";
                    while (res->next()) {
                        int rentid3 = res->getInt("rental_status_ID");
                        int rentcust3 = res->getInt("rent_customer_ID");
                        int rentbook3 = res->getInt("rent_book_ID");
                        int rentq3 = res->getInt("rent_quantity");
                        string rentdate3 = res->getString("rent_date");
                        string isreturned3 = res->getString("is_returned");
                        int retquantity3 = res->getInt("return_quantity");
                        string lastretdate3 = res->getString("last_return_date");
                        cout << endl << "|" << setw(10) << rentdate3 << "||" << setw(9) << rentid3 << "||" << setw(7) << rentbook3 << "||" << setw(11) << rentcust3
                            << "||" << setw(13) << rentq3 << "||" << setw(12) << isreturned3 << "||" << setw(15) << retquantity3 << "||" << setw(16) << lastretdate3
                            << "|";
                    }  
                    delete res;
                    delete stmt;
                    cout << endl << endl;
                    system("pause");
                    system("cls");
                    goto libraryDB;
//////////////New Customer Registration///////////////////////////////////////////////////////////////////////////////////
                case 6:
                    cout << "*********** Registration of a new customer: \n";
                    cin.ignore();
                    cout << "*********** Name (required): ";
                    getline(cin, cuname);
                    if (!IsFilled(cuname)) { Message(); goto libraryDB; }
                    cout << "*********** Adress (required): ";
                    getline(cin, cuadress);
                    if (!IsFilled(cuadress)) { Message(); goto libraryDB; }
                    cout << "*********** ZipCode (required): ";
                    getline(cin, cuzip);
                    if (!IsFilled(cuzip)) { Message(); goto libraryDB; }
                    cout << "*********** City (required): ";
                    getline(cin, cucity);
                    if (!IsFilled(cucity)) { Message(); goto libraryDB; }
                    cout << "*********** Country (required): ";
                    getline(cin, cucountry);
                    if (!IsFilled(cucountry)) { Message(); goto libraryDB; }
                    date = date::format("%F", chrono::system_clock::now());     
                    pstmt = con->prepareStatement("INSERT INTO customer(name, registration_date) VALUES(?,?)");
                    pstmt->setString(1, cuname);
                    pstmt->setString(2, date);
                    pstmt->execute();
                    delete pstmt;
                    pstmt = con->prepareStatement("INSERT INTO customer_adress(adress, zipcode, city, country) VALUES(?,?,?,?)");
                    pstmt->setString(1, cuadress);
                    pstmt->setString(2, cuzip);
                    pstmt->setString(3, cucity);
                    pstmt->setString(4, cucountry);
                    pstmt->execute();
                    delete pstmt;
                    cout << "\n*********** Customer registration successful..!\n\n";
                    system("pause");
                    system("cls");
                    goto libraryDB;
//////////////New Book Registration///////////////////////////////////////////////////////////////////////////////////
                case 7:              
                    cout << "*********** Registration of a new book: \n";
                    cin.ignore();
                    cout << "*********** ISBN (required): ";
                    getline(cin, isbn);
                    if (!IsFilled(isbn)) {Message(); goto libraryDB; }
                    cout << "*********** Title (required): ";
                    getline(cin, title);
                    if (!IsFilled(title)) {Message(); goto libraryDB; }
                    cout << "*********** Author (required): ";
                    getline(cin, author);
                    if (!IsFilled(author)) {Message(); goto libraryDB; }
                    cout << "*********** Publisher (required): ";
                    getline(cin, publisher);
                    if (!IsFilled(publisher)) {Message(); goto libraryDB; }
                    cout << "*********** Genre (required): ";
                    getline(cin, genre);
                    if (!IsFilled(genre)) {Message(); goto libraryDB; }
                    cout << "*********** Quantity (required): ";
                    getline(cin, quantity_available);
                    if (!IsFilled(quantity_available)) { Message(); goto libraryDB; }
                    if (IntCheck(quantity_available)) {
                        quantity_available1 = stoi(quantity_available);
                    }
                    else {
                        Message(); goto libraryDB;
                    }
                    stmt = con->createStatement();
                    res = stmt->executeQuery("SELECT isbn, title FROM book ORDER BY isbn ASC");
                    while (res->next()) {
                        string isbnDB = res->getString("isbn");
                        string titleDB = res->getString("title");
                        if (isbnDB == isbn) {
                            cout << "\n*********** Operation failed..!\n*********** ISBN already in database..!\n\n";
                            system("pause");
                            system("cls");
                            goto libraryDB;
                        }
                        if (titleDB == title) {
                            cout << "\n*********** Operation failed..!\n*********** Title already in database..!\n\n";
                            system("pause");
                            system("cls");
                            goto libraryDB;
                        }
                    }
                    delete res;
                    delete stmt;
                    pstmt = con->prepareStatement("INSERT INTO book(isbn, title, author, publisher, genre, quantity_available) VALUES(?,?,?,?,?,?)");
                    pstmt->setString(1, isbn);
                    pstmt->setString(2, title);
                    pstmt->setString(3, author);
                    pstmt->setString(4, publisher);
                    pstmt->setString(5, genre);
                    pstmt->setInt(6, quantity_available1);
                    pstmt->execute();
                    delete pstmt;
                    cout << "\n*********** The addition of the book was successful..!\n\n";
                    system("pause");
                    system("cls");
                    goto libraryDB;
//////////////Book Modification///////////////////////////////////////////////////////////////////////////////////
                case 8:
                    cout << "*********** Enter book ID: ";
                    cin >> bookid4s;
                    if (IntCheck(bookid4s)) {
                        bookid4 = stoi(bookid4s);
                    }
                    else {
                        Message();
                        goto libraryDB;
                    }
                    stmt = con->createStatement();
                    res = stmt->executeQuery("SELECT book_ID, isbn, title, author, publisher, genre, quantity_available FROM book ORDER BY title ASC");
                    while (res->next()) {
                        int bookid4db = res->getInt("book_ID");
                        if (bookid4 == bookid4db) {
                            string isbn2 = res->getString("isbn");
                            string title2 = res->getString("title");
                            string author2 = res->getString("author");
                            string publisher2 = res->getString("publisher");
                            string genre2 = res->getString("genre");
                            int quantity2 = res->getInt("quantity_available");
                            cout << endl << "*********** Your book: " << endl << endl;
                            cout << "*********** Title: " << title2 << endl;
                            cout << "*********** Author: " << author2 << endl;
                            cout << "*********** ID: " << bookid4 << endl;
                            cout << "*********** ISBN: " << isbn2 << endl;
                            cout << "*********** Publisher: " << publisher2 << endl;
                            cout << "*********** Genre: " << genre2 << endl;
                            cout << "*********** Available quantity: " << quantity2 << endl << endl;
                            break;
                        }
                        else {

                        }
                    }
                    delete res;
                    delete stmt;
                    cout << "*********** 1. Change Title\n";
                    cout << "*********** 2. Change Author\n";
                    cout << "*********** 3. Change Publisher\n";
                    cout << "*********** 4. Change Genre\n";
                    cout << "*********** 5. Change Quantity\n";
                    cout << "*********** 6. Return\n";
                    cout << "*********** Choose Action: ";
                    cin >> optioncheck1;
                    system("cls");
                    if (IntCheck(optioncheck1)) {
                        int option1 = stoi(optioncheck1);   
                        if (option1 == 1) {
                            string change1;
                            cin.ignore();
                            cout << endl << "*********** New Title: ";
                            getline(cin, change1);
                            if (!IsFilled(change1)) { Message(); goto libraryDB; }
                            pstmt = con->prepareStatement("UPDATE book SET title = ? WHERE book_ID = ?");
                            pstmt->setString(1, change1);
                            pstmt->setInt(2, bookid4);
                            pstmt->execute();
                            delete pstmt;
                            cout << "\n*********** Operation successful..!\n\n";
                        }
                        if (option1 == 2) {
                            string change1;
                            cin.ignore();
                            cout << endl << "*********** New Author: ";
                            getline(cin, change1);
                            if (!IsFilled(change1)) { Message(); goto libraryDB; }
                            pstmt = con->prepareStatement("UPDATE book SET author = ? WHERE book_ID = ?");
                            pstmt->setString(1, change1);
                            pstmt->setInt(2, bookid4);
                            pstmt->execute();
                            delete pstmt;
                            cout << "\n*********** Operation successful..!\n\n";
                        }
                        if (option1 == 3) {
                            string change1;
                            cin.ignore();
                            cout << endl << "*********** New Publisher: ";
                            getline(cin, change1);
                            if (!IsFilled(change1)) { Message(); goto libraryDB; }
                            pstmt = con->prepareStatement("UPDATE book SET publisher = ? WHERE book_ID = ?");
                            pstmt->setString(1, change1);
                            pstmt->setInt(2, bookid4);
                            pstmt->execute();
                            delete pstmt;
                            cout << "\n*********** Operation successful..!\n\n";
                        }
                        if (option1 == 4) {
                            string change1;
                            cin.ignore();
                            cout << endl << "*********** New Genre: ";
                            getline(cin, change1);
                            if (!IsFilled(change1)) { Message(); goto libraryDB; }
                            pstmt = con->prepareStatement("UPDATE book SET genre = ? WHERE book_ID = ?");
                            pstmt->setString(1, change1);
                            pstmt->setInt(2, bookid4);
                            pstmt->execute();
                            delete pstmt;
                            cout << "\n*********** Operation successful..!\n\n";
                        }
                        if (option1 == 5) {
                            string change1;
                            cin.ignore();
                            cout << endl << "*********** New Quantity: ";
                            cin >> change1;
                            if (IntCheck(change1)) {
                                change2 = stoi(change1);
                            }
                            else {
                                Message();
                                goto libraryDB;
                            }
                            pstmt = con->prepareStatement("UPDATE book SET quantity_available = ? WHERE book_ID = ?");
                            pstmt->setInt(1, change2);
                            pstmt->setInt(2, bookid4);
                            pstmt->execute();
                            delete pstmt;
                            cout << "\n*********** Operation successful..!\n\n";
                        }
                        system("pause");
                        system("cls");
                    }
                    else {
                        goto libraryDB;
                    }
                    goto libraryDB;
                case 0:
                    goto loop1;
                default:
                    Message();
                    goto libraryDB;
                }
            } while (option1);
        }
        else {
            Message();
            goto libraryDB;
        }
        delete con;
    }
    catch (sql::SQLException& e){
        cout << "*********** # ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "*********** # ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    system("pause");
    return 0;
}
