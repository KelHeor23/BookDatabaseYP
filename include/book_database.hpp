#pragma once

#include <print>
#include <string>
#include <string_view>
#include <vector>
#include <flat_set>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using AuthorContainer   = std::flat_set<std::string, TransparentStringLess>;
    using book_iterator     = typename BookContainer::iterator;
    using author_iterator   = typename AuthorContainer::iterator;
    using size_type         = typename BookContainer::size_type;
    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> init) {
        for (const auto& b : init) {
            PushBack(b);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    book_iterator begin() noexcept { return books_.begin(); }
    book_iterator end() noexcept { return books_.end(); }

    author_iterator authors_begin() noexcept { return authors_.begin(); }
    author_iterator authors_end() noexcept { return authors_.end(); }

    size_type size() const noexcept { return books_.size(); }
    bool empty() const noexcept { return books_.empty(); }

    const BookContainer& GetBooks() const noexcept {
        return books_;
    }

    const AuthorContainer& GetAuthors() const noexcept {
        return authors_;
    }

    void PushBack(Book book) {
        if (!book.author.empty()) {
            auto [it, inserted] = authors_.emplace(book.author);
            book.author = *it;
        }
        books_.push_back(std::move(book));
    }

    template <typename... Args>
    Book& EmplaceBack(Args&&... args) {
        books_.emplace_back(std::forward<Args>(args)...);
        Book& book = books_.back();

        if (!book.author.empty()) {
            auto [it, inserted] = authors_.emplace(book.author);
            book.author = *it;
        }
        
        return book;
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
