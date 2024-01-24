#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const string DATABASE_FILE = "library_database.txt";
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin123";

class Library {
private:
    int bookId;
    string bookTitle;
    string authorName;
    int quantity;

public:
    void displayMenu();
    void handleAdministrator();
    void handleUser();
    void addBook();
    void editBook();
    void removeBook();
    void listBooks();
};

bool authenticateAdministrator(const string& username, const string& password) {
    return (username == ADMIN_USERNAME && password == ADMIN_PASSWORD);
}

void Library::displayMenu() {
    int userChoice;
    string username, password;

    cout << "--------------- Library Management System ---------------" << endl;
    cout << "1. Administrator Login" << endl;
    cout << "2. User Access" << endl;
    cout << "3. Exit" << endl;
    cout << "Please select an option: ";
    cin >> userChoice;

    switch (userChoice) {
    case 1:
        cout << "Enter Administrator Username: ";
        cin >> username;
        cout << "Enter Administrator Password: ";
        cin >> password;

        if (authenticateAdministrator(username, password)) {
            handleAdministrator();
        } else {
            cout << "Invalid username or password." << endl;
        }
        break;

    case 2:
        handleUser();
        break;

    case 3:
        cout << "Exiting the program..." << endl;
        exit(0);

    default:
        cout << "Invalid choice. Please try again." << endl;
    }

    displayMenu();
}

void Library::handleAdministrator() {
    int adminChoice;

    cout << "\n\nAdministrator Menu:" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Edit Book" << endl;
    cout << "3. Remove Book" << endl;
    cout << "4. List Books" << endl;
    cout << "5. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    cin >> adminChoice;

    switch (adminChoice) {
    case 1:
        addBook();
        break;

    case 2:
        editBook();
        break;

    case 3:
        removeBook();
        break;

    case 4:
        listBooks();
        break;

    case 5:
        displayMenu();
        break;

    default:
        cout << "Invalid choice. Please try again." << endl;
    }

    handleAdministrator();
}

void Library::handleUser() {
    int userChoice;

    cout << "\n\nUser Menu:" << endl;
    cout << "1. List Books" << endl;
    cout << "2. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> userChoice;

    switch (userChoice) {
    case 1:
        listBooks();
        break;

    case 2:
        cout << "Exiting the program..." << endl;
        exit(0);

    default:
        cout << "Invalid choice. Please try again." << endl;
    }

    handleUser();
}

void Library::addBook() {
    fstream bookData;
    int existingBookId;
    int token = 0;

    cout << "\n\nAdd New Book:" << endl;
    cout << "Book ID: ";
    cin >> bookId;
    cout << "Book Title: ";
    cin.ignore();
    getline(cin, bookTitle);
    cout << "Author Name: ";
    getline(cin, authorName);
    cout << "Quantity: ";
    cin >> quantity;

    bookData.open(DATABASE_FILE, ios::in);

    if (!bookData) {
        bookData.open(DATABASE_FILE, ios::app | ios::out);
        bookData << bookId << " " << bookTitle << " " << authorName << " " << quantity << "\n";
        bookData.close();
    } else {
        // Clear error flags and reset file pointer to the beginning
        bookData.clear();
        bookData.seekg(0, ios::beg);

        bookData >> existingBookId >> bookTitle >> authorName >> quantity;

        while (!bookData.eof()) {
            if (existingBookId == bookId) {
                token++;
            }
            bookData >> existingBookId >> bookTitle >> authorName >> quantity;
        }
        bookData.close();

        if (token == 1) {
            cout << "Book with the same ID already exists. Please try again." << endl;
            addBook();
        } else {

            bookData.open(DATABASE_FILE, ios::app | ios::out | ios::in);
            bookData.seekp(0, ios::end);

            bookData << bookId << " " << bookTitle << " " << authorName << " " << quantity << "\n";
            bookData.close();

            cout << "Book added successfully!" << endl;
        }
    }
}


void Library::editBook() {
    fstream data, data1;
    int bookKey;
    int token = 0;
    int id;
    string title, author;
    int qty;

    cout << "\n\nEdit Book Record:" << endl;
    cout << "Enter Book ID: ";
    cin >> bookKey;

    data.open(DATABASE_FILE, ios::in);
    if (!data) {
        cout << "File doesn't exist." << endl;
    } else {
        data1.open("library_database_temp.txt", ios::app | ios::out);

        data >> id >> title >> author >> qty;
        while (!data.eof()) {
            if (bookKey == id) {
                cout << "Enter New Book ID: ";
                cin >> id;
                cout << "Enter New Book Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter New Author Name: ";
                getline(cin, author);
                cout << "Enter New Quantity: ";
                cin >> qty;
                data1 << id << " " << title << " " << author << " " << qty << "\n";
                cout << "Record edited successfully." << endl;
                token++;
            } else {
                data1 << id << " " << title << " " << author << " " << qty << "\n";
            }
            data >> id >> title >> author >> qty;
        }
        data.close();
        data1.close();

        remove(DATABASE_FILE.c_str());
        rename("library_database_temp.txt", DATABASE_FILE.c_str());

        if (token == 0) {
            cout << "Record not found." << endl;
        }
    }
}

void Library::removeBook() {
    fstream data, data1;
    int bookKey;
    int token = 0;

    cout << "\n\nRemove Book:" << endl;
    cout << "Enter Book ID: ";
    cin >> bookKey;

    data.open(DATABASE_FILE, ios::in);
    if (!data) {
        cout << "File doesn't exist." << endl;
    } else {
        data1.open("library_database_temp.txt", ios::app | ios::out);

        data >> bookId >> bookTitle >> authorName >> quantity;
        while (!data.eof()) {
            if (bookKey == bookId) {
                cout << "Book deleted successfully." << endl;
                token++;
            } else {
                data1 << bookId << " " << bookTitle << " " << authorName << " " << quantity << "\n";
            }
            data >> bookId >> bookTitle >> authorName >> quantity;
        }
        data.close();
        data1.close();

        remove(DATABASE_FILE.c_str());
        rename("library_database_temp.txt", DATABASE_FILE.c_str());

        if (token == 0) {
            cout << "Record not found." << endl;
        }
    }
}

void Library::listBooks() {
    fstream data;
    data.open(DATABASE_FILE, ios::in);

    if (!data) {
        cout << "Database is empty." << endl;
    } else {
        cout << "\nList of Books in the Library:" << endl;
        cout << setw(15) << "Book ID" << setw(30) << "Book Title" << setw(25) << "Author" << setw(15) << "Quantity" << endl;
        cout << setfill('-') << setw(85) << "-" << setfill(' ') << endl;


        data.clear();
        data.seekg(0, ios::beg);

        data >> bookId >> bookTitle >> authorName >> quantity;
        while (!data.eof()) {
            cout << setw(15) << bookId << setw(30) << bookTitle << setw(25) << authorName << setw(15) << quantity << endl;
            data >> bookId >> bookTitle >> authorName >> quantity;
        }

        data.close();
    }
}


int main() {
    Library library;
    library.displayMenu();
    return 0;
}
