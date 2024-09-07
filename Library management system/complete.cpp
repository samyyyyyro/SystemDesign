#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Book {
private:
    string isbn;
    string title;
    string author;
    int publicationYear;
    bool available;

public:
     Book() : isbn(""), title(""), author(""), publicationYear(0), available(true) {}
    Book(string isbn, string title, string author, int publicationYear)
        : isbn(isbn), title(title), author(author), publicationYear(publicationYear), available(true) {}

    string getIsbn() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isAvailable() const { return available; }
    void setAvailable(bool available) { this->available = available; }

    bool operator==(const Book& other) const {
        return isbn == other.isbn;
    }
};

class Member {
private:
    string memberId;
    string name;
    string contactInfo;
    vector<Book> borrowedBooks;

public:
      Member() : memberId(""), name(""), contactInfo("") {}
    Member(string memberId, string name, string contactInfo)
        : memberId(memberId), name(name), contactInfo(contactInfo) {}

    void borrowBook(const Book& book) { borrowedBooks.push_back(book); }
    void returnBook(const Book& book) {
        auto it = remove_if(borrowedBooks.begin(), borrowedBooks.end(), 
                            [&](const Book& b) { return b.getIsbn() == book.getIsbn(); });
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it, borrowedBooks.end());
        }
    }

    string getMemberId() const { return memberId; }
    string getName() const { return name; }
    vector<Book> getBorrowedBooks() const { return borrowedBooks; }
};

class LibraryManager {
private:
    unordered_map<string, Book> catalog;
    unordered_map<string, Member> members;
    const int MAX_BOOKS_PER_MEMBER = 5;

    LibraryManager() {}

public:
    static LibraryManager& getInstance() {
        static LibraryManager instance;
        return instance;
    }

    void addBook(const Book& book) { catalog[book.getIsbn()] = book; }
    void removeBook(const string& isbn) { catalog.erase(isbn); }
    Book getBook(const string& isbn) {
        auto it = catalog.find(isbn);
        if (it != catalog.end()) {
            return it->second;
        } else {
            throw runtime_error("Book not found");
        }
    }
    void registerMember(const Member& member) { members[member.getMemberId()] = member; }
    void unregisterMember(const string& memberId) { members.erase(memberId); }
    Member getMember(const string& memberId) {
        auto it = members.find(memberId);
        if (it != members.end()) {
            return it->second;
        } else {
            throw runtime_error("Member not found");
        }
    }

    void borrowBook(const string& memberId, const string& isbn) {
        auto memberIt = members.find(memberId);
        auto bookIt = catalog.find(isbn);

        if (memberIt == members.end()) {
            cout << "Member not found." << endl;
            return;
        }
        if (bookIt == catalog.end()) {
            cout << "Book not found." << endl;
            return;
        }

        Member& member = memberIt->second;
        Book& book = bookIt->second;

        if (book.isAvailable() && member.getBorrowedBooks().size() < MAX_BOOKS_PER_MEMBER) {
            member.borrowBook(book);
            book.setAvailable(false);
            cout << "Book borrowed: " << book.getTitle() << " by " << member.getName() << endl;
        } else {
            cout << "Cannot borrow book." << endl;
        }
    }

    void returnBook(const string& memberId, const string& isbn) {
        auto memberIt = members.find(memberId);
        auto bookIt = catalog.find(isbn);

        if (memberIt == members.end()) {
            cout << "Member not found." << endl;
            return;
        }
        if (bookIt == catalog.end()) {
            cout << "Book not found." << endl;
            return;
        }

        Member& member = memberIt->second;
        Book& book = bookIt->second;

        member.returnBook(book);
        book.setAvailable(true);
        cout << "Book returned: " << book.getTitle() << " by " << member.getName() << endl;
    }

    vector<Book> searchBooks(const string& keyword) {
        vector<Book> matchingBooks;
        for (const auto& [isbn, book] : catalog) {
            if (book.getTitle().find(keyword) != string::npos || book.getAuthor().find(keyword) != string::npos) {
                matchingBooks.push_back(book);
            }
        }
        return matchingBooks;
    }
};

int main() {
    LibraryManager& libraryManager = LibraryManager::getInstance();

    // Add books to the catalog
    libraryManager.addBook(Book("ISBN1", "Book 1", "Author 1", 2020));
    libraryManager.addBook(Book("ISBN2", "Book 2", "Author 2", 2019));
    libraryManager.addBook(Book("ISBN3", "Book 3", "Author 3", 2021));

    // Register members
    libraryManager.registerMember(Member("M1", "John Doe", "john@example.com"));
    libraryManager.registerMember(Member("M2", "Jane Smith", "jane@example.com"));

    // Borrow books
    libraryManager.borrowBook("M1", "ISBN1");
    libraryManager.borrowBook("M2", "ISBN2");

    // Return books
    libraryManager.returnBook("M1", "ISBN1");

    // Search books
    auto searchResults = libraryManager.searchBooks("Book");
    cout << "Search Results:" << endl;
    for (const auto& book : searchResults) {
        cout << book.getTitle() << " by " << book.getAuthor() << endl;
    }

    return 0;
}
