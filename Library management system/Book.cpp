#include <string>
#include "LibraryManager.cpp"
#include "Member.cpp"

class Book {
private:
    std::string isbn;
    std::string title;
    std::string author;
    int publicationYear;
    bool available;

public:
    Book(std::string isbn, std::string title, std::string author, int publicationYear)
        : isbn(isbn), title(title), author(author), publicationYear(publicationYear), available(true) {}

    std::string getIsbn() const { return isbn; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    bool isAvailable() const { return available; }
    void setAvailable(bool available) { this->available = available; }
};
