#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>
#include <flat_map>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    using flat_map = std::flat_map<std::string, std::size_t, Comparator>;

    flat_map gist{comp};

    for (const auto& it : cont.GetBooks())
        ++gist[it.author];

    return gist;
}

template <BookContainerLike T>
auto calculateGenreRating(const BookDatabase<T> &cont) {
    if (cont.GetBooks().empty()) return 0.;
    auto &books = cont.GetBooks();
    auto rating = std::transform(books.begin(), books.end(), [](auto &b){return b.rating;});
    double sum = std::accumulate(rating.begin(), rating.end(), 0.);
    return sum / books.size();
}


template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.GetBooks().empty()) return 0.;
    auto &books = cont.GetBooks();
    auto rating = std::transform(books.begin(), books.end(), [](auto &b){return b.rating;});
    double sum = std::accumulate(rating.begin(), rating.end(), 0.);
    return sum / books.size();
}

}  // namespace bookdb
