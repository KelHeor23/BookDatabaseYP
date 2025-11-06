#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {
    
    return  s == "Fiction"      ? Genre::Fiction :
            s == "NonFiction"   ? Genre::NonFiction :
            s == "SciFi"        ? Genre::SciFi :
            s == "Biography"    ? Genre::Biography :
            s == "Mystery"      ? Genre::Mystery :
                                  Genre::Unknown;
}

struct Book {
public:
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    Book() = delete;
    Book(std::string t, std::string_view a, Genre g = Genre::Unknown, int y = 0, double r = 0.0, int rc = 0)
        : author(a), title(std::move(t)), year(y), genre(g), rating(r), read_count(rc) {}

    Book(std::string t, std::string_view a, std::string g)
        : author(a), title(std::move(t)), year(0), genre(GenreFromString(g)), rating(0.0), read_count(0) {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book> {
    template <typename FormatContext>
    auto format(const bookdb::Book b, FormatContext &fc) const {
        format_to(fc.out(), "Title = {}, Author = {}, Genre = {}, Year = {}, Rating = {}, ReadCount = {}"
        , b.title, b.author, b.genre, b.year, b.rating, b.read_count);

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
