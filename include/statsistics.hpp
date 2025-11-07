#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>
#include <flat_map>
#include <map>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    using flat_map = std::flat_map<std::string, std::size_t, Comparator>;

    flat_map gist{comp};

    for (const auto& b : cont.GetBooks()) {
        auto [it, inserted] = gist.try_emplace(std::string(b.author), 0);
        ++it->second;
    }
    std::string out;
    for (const auto& [author, count] : gist)
        out += std::format("{}: {}\n", author, count);

    return out;
}

template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &cont) {
    std::map<Genre, double> res;
    std::string out = "";
    if (cont.GetBooks().empty()) return out;
    constexpr auto genre_count = std::to_underlying(Genre::Unknown) + 1;

    std::array<double, genre_count> sum{};
    std::array<std::size_t, genre_count> cnt{};
    
    for (const auto& b : cont.GetBooks()) {
        auto i = std::to_underlying(b.genre);
        sum[i] += b.rating;
        ++cnt[i];
    }
    
    for (std::size_t i = 0; i < genre_count; ++i)
        if (cnt[i])
            res.emplace(static_cast<Genre>(i), sum[i] / cnt[i]);

    for (const auto& [genre, raiting] : res)
        out += std::format("{}: {}\n", genre, raiting);

    return out;
}


template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T> &cont) {
    const auto& books = cont.GetBooks();
    if (books.empty()) return 0.;

    double sum = std::accumulate(
        books.begin(), books.end(), 0.,
        [](double acc, const bookdb::Book& b) { return acc + b.rating; }
    );
    return sum / books.size();
}

using resultBookVec = std::vector<std::reference_wrapper<const Book>>;

template <BookContainerLike T>
resultBookVec sampleRandomBooks(const BookDatabase<T> &cont, std::size_t count) {
    resultBookVec result;
    const auto& books = cont.GetBooks();
    if (books.empty() || count == 0) return result;

    count = std::min<std::size_t>(count, books.size());    
    result.reserve(count);

    static thread_local std::mt19937_64 rng{std::random_device{}()};
    std::sample(books.begin(), books.end(),
                std::back_inserter(result),
                count,
                rng);

    return result;
}

template <BookContainerLike T, typename Comp>
resultBookVec getTopNBy(BookDatabase<T> &cont, std::size_t count, Comp comp) {
    resultBookVec result;
    auto& books = cont.GetBooks();
    if (books.empty() || count == 0) return result;

    count = std::min<std::size_t>(count, books.size());
    result.reserve(count);

    std::nth_element(books.begin(), books.begin() + count, books.end(), comp);
    for (std::size_t i = 0; i < count; i++) 
        result.emplace_back(std::cref(books[i]));

    return result;
}

}  // namespace bookdb
