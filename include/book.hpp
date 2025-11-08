#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

namespace detail {
constexpr std::array<std::string_view, 6> kGenreNames{
    "Fiction", "NonFiction", "SciFi", "Biography", "Mystery", "Unknown"
};
}

constexpr Genre GenreFromString(std::string_view s) {
    const auto it = std::find(detail::kGenreNames.begin(),
                              detail::kGenreNames.end(), s);
    return it == detail::kGenreNames.end()
           ? Genre::Unknown
           : static_cast<Genre>(std::distance(detail::kGenreNames.begin(), it));
}

constexpr std::string_view GenreToString(Genre g) {
    const auto idx = static_cast<std::size_t>(g);
    return idx < detail::kGenreNames.size()
           ? detail::kGenreNames[idx]
           : detail::kGenreNames.back();
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
    Book(std::string title_, std::string_view author_, int year_ = 0, Genre genre_ = Genre::Unknown, double raiting_ = 0.0, int read_count_ = 0)
        : author(author_), title(std::move(title_)), year(year_), genre(genre_), rating(raiting_), read_count(read_count_) {}

    Book(std::string title_, std::string_view author_, std::string genre_)
        : author(author_), title(std::move(title_)), year(0), genre(GenreFromString(genre_)), rating(0.0), read_count(0) {}
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
