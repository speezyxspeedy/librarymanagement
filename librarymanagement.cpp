#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Helper: String ko lowercase me convert karne ke liye (case-insensitive search)
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// 1. Book Class
class Book {
public:
    string bookId;
    string title;
    string author;
    int totalCopies;
    int availableCopies;

    Book() : totalCopies(0), availableCopies(0) {}

    Book(string id, string t, string a, int copies) {
        bookId = id;
        title = t;
        author = a;
        totalCopies = copies;
        availableCopies = copies;
    }

    void display() const {
        cout << "[" << bookId << "] \"" << title << "\" by " << author 
             << " | Available: " << availableCopies << "/" << totalCopies << "\n";
    }
};

// 2. Member Class
class Member {
public:
    string memberId;
    string name;
    vector<string> borrowedBooks; // Borrowed Book IDs list

    Member() {}

    Member(string id, string n) {
        memberId = id;
        name = n;
    }

    bool hasBook(const string& bookId) const {
        return find(borrowedBooks.begin(), borrowedBooks.end(), bookId) != borrowedBooks.end();
    }

    void display() const {
        cout << "Member [" << memberId << "]: " << name << " | Borrowed IDs: ";
        if (borrowedBooks.empty()) {
            cout << "None";
        } else {
            for (const auto& bId : borrowedBooks) cout << bId << " ";
        }
        cout << "\n";
    }
};

// 3. Library Management System Class
class LibrarySystem {
private:
    unordered_map<string, Book> books;       // bookId -> Book
    unordered_map<string, Member> members;   // memberId -> Member

public:
    // Add Book
    void addBook(const string& id, const string& title, const string& author, int copies) {
        if (books.find(id) != books.end()) {
            books[id].totalCopies += copies;
            books[id].availableCopies += copies;
            cout << "Updated copies for book: " << title << "\n";
        } else {
            books[id] = Book(id, title, author, copies);
            cout << "Book \"" << title << "\" added successfully!\n";
        }
    }

    // Register Member
    void registerMember(const string& id, const string& name) {
        if (members.find(id) != members.end()) {
            cout << "Error: Member ID already exists.\n";
        } else {
            members[id] = Member(id, name);
            cout << "Member \"" << name << "\" registered successfully!\n";
        }
    }

    // Issue Book
    void issueBook(const string& bookId, const string& memberId) {
        if (books.find(bookId) == books.end()) {
            cout << "Error: Book ID '" << bookId << "' not found.\n";
            return;
        }
        if (members.find(memberId) == members.end()) {
            cout << "Error: Member ID '" << memberId << "' not found.\n";
            return;
        }

        Book& book = books[bookId];
        Member& member = members[memberId];

        if (member.hasBook(bookId)) {
            cout << "Error: Member already borrowed this book.\n";
            return;
        }
        if (book.availableCopies <= 0) {
            cout << "Error: Book currently out of stock.\n";
            return;
        }

        book.availableCopies--;
        member.borrowedBooks.push_back(bookId);
        cout << "Success: \"" << book.title << "\" issued to " << member.name << ".\n";
    }

    // Return Book
    void returnBook(const string& bookId, const string& memberId) {
        if (books.find(bookId) == books.end() || members.find(memberId) == members.end()) {
            cout << "Error: Invalid Book ID or Member ID.\n";
            return;
        }

        Book& book = books[bookId];
        Member& member = members[memberId];

        auto it = find(member.borrowedBooks.begin(), member.borrowedBooks.end(), bookId);
        if (it == member.borrowedBooks.end()) {
            cout << "Error: Member has not borrowed this book.\n";
            return;
        }

        member.borrowedBooks.erase(it);
        book.availableCopies++;
        cout << "Success: \"" << book.title << "\" returned by " << member.name << ".\n";
    }

    // Search by Title or Author
    void searchBook(const string& query) const {
        string q = toLowerCase(query);
        bool found = false;
        cout << "\n--- Search Results for \"" << query << "\" ---\n";
        for (const auto& pair : books) {
            const Book& b = pair.second;
            if (toLowerCase(b.title).find(q) != string::npos || 
                toLowerCase(b.author).find(q) != string::npos) {
                b.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No matching books found.\n";
        }
        cout << "--------------------------------------\n";
    }

    // Display All Books
    void displayAllBooks() const {
        if (books.empty()) {
            cout << "Inventory is empty.\n";
            return;
        }
        cout << "\n=== Library Catalog ===\n";
        for (const auto& pair : books) {
            pair.second.display();
        }
        cout << "=======================\n";
    }
};

int main() {
    LibrarySystem lib;

    // Sample Data
    lib.addBook("B101", "Let Us C", "Yashavant Kanetkar", 3);
    lib.addBook("B102", "Effective Modern C++", "Scott Meyers", 2);
    lib.registerMember("M01", "Aman Sharma");

    int choice;
    while (true) {
        cout << "\n===== LIBRARY SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. Register Member\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Search Book (Title / Author)\n";
        cout << "6. Display All Books\n";
        cout << "7. Exit\n";
        cout << "Enter choice (1-7): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        cin.ignore(); // flush buffer

        if (choice == 1) {
            string id, title, author;
            int copies;
            cout << "Book ID: "; getline(cin, id);
            cout << "Title: "; getline(cin, title);
            cout << "Author: "; getline(cin, author);
            cout << "Copies: "; cin >> copies;
            cin.ignore();
            lib.addBook(id, title, author, copies);
        } else if (choice == 2) {
            string id, name;
            cout << "Member ID: "; getline(cin, id);
            cout << "Name: "; getline(cin, name);
            lib.registerMember(id, name);
        } else if (choice == 3) {
            string bId, mId;
            cout << "Book ID to issue: "; getline(cin, bId);
            cout << "Member ID: "; getline(cin, mId);
            lib.issueBook(bId, mId);
        } else if (choice == 4) {
            string bId, mId;
            cout << "Book ID to return: "; getline(cin, bId);
            cout << "Member ID: "; getline(cin, mId);
            lib.returnBook(bId, mId);
        } else if (choice == 5) {
            string query;
            cout << "Enter title or author: "; getline(cin, query);
            lib.searchBook(query);
        } else if (choice == 6) {
            lib.displayAllBooks();
        } else if (choice == 7) {
            cout << "Exiting system. Dhanyawad!\n";
            break;
        } else {
            cout << "Invalid choice! Enter 1-7.\n";
        }
    }

    return 0;
}
