#include <string>
#include <vector>
#include "LibraryManager.cpp"
#include "Book.cpp"

class Member {
private:
    std::string memberId;
    std::string name;
    std::string contactInfo;
    std::vector<Book> borrowedBooks;

public:
    Member(std::string memberId, std::string name, std::string contactInfo)
        : memberId(memberId), name(name), contactInfo(contactInfo) {}

    void borrowBook(const Book& book) { borrowedBooks.push_back(book); }
    void returnBook(const Book& book) {
        borrowedBooks.erase(std::remove(borrowedBooks.begin(), borrowedBooks.end(), book), borrowedBooks.end());
    }

    std::string getMemberId() const { return memberId; }
    std::string getName() const { return name; }
    std::vector<Book> getBorrowedBooks() const { return borrowedBooks; }
};
