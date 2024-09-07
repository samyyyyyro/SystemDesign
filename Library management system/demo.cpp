#include <iostream>
#include "LibraryManager.cpp"
#include "Member.cpp"
#include "Book.cpp"

using namespace std;
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
    std::cout << "Search Results:" << std::endl;
    for (const auto& book : searchResults) {
        std::cout << book.getTitle() << " by " << book.getAuthor() << std::endl;
    }

    return 0;
}
