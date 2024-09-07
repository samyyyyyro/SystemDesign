#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Member.cpp"
#include "Book.cpp"

class LibraryManager {
private:
    std::unordered_map<std::string, Book> catalog;
    std::unordered_map<std::string, Member> members;
    const int MAX_BOOKS_PER_MEMBER = 5;

    LibraryManager() {}

public:
    static LibraryManager& getInstance() {
        static LibraryManager instance;
        return instance;
    }

    void addBook(const Book& book) { catalog[book.getIsbn()] = book; }
    void removeBook(const std::string& isbn) { catalog.erase(isbn); }
    Book getBook(const std::string& isbn) { return catalog[isbn]; }
    void registerMember(const Member& member) { members[member.getMemberId()] = member; }
    void unregisterMember(const std::string& memberId) { members.erase(memberId); }
    Member getMember(const std::string& memberId) { return members[memberId]; }

    void borrowBook(const std::string& memberId, const std::string& isbn) {
        Member& member = members[memberId];
        Book& book = catalog[isbn];

        if (book.isAvailable() && member.getBorrowedBooks().size() < MAX_BOOKS_PER_MEMBER) {
            member.borrowBook(book);
            book.setAvailable(false);
            std::cout << "Book borrowed: " << book.getTitle() << " by " << member.getName() << std::endl;
        } else {
            std::cout << "Cannot borrow book." << std::endl;
        }
    }

    void returnBook(const std::string& memberId, const std::string& isbn) {
        Member& member = members[memberId];
        Book& book = catalog[isbn];

        member.returnBook(book);
        book.setAvailable(true);
        std::cout << "Book returned: " << book.getTitle() << " by " << member.getName() << std::endl;
    }

    std::vector<Book> searchBooks(const std::string& keyword) {
        std::vector<Book> matchingBooks;
        for (const auto& [isbn, book] : catalog) {
            if (book.getTitle().find(keyword) != std::string::npos || book.getAuthor().find(keyword) != std::string::npos) {
                matchingBooks.push_back(book);
            }
        }
        return matchingBooks;
    }
};
