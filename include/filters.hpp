#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

inline auto YearBetween(int from, int to) {
    return [from, to](const Book& b) {
        return b.year >= from && b.year <= to;
    };
}

inline auto RatingAbove(double threshold) {
    return [threshold](const Book& b) {
        return b.rating >= threshold;
    };
}

inline auto GenreIs(Genre g) {
    return [g](const Book& b) {
        return b.genre == g;
    };
}

template <BookPredicate... Preds>
inline auto all_of(Preds... preds) {
    return [=](const Book& b) {
        return (preds(b) && ...);
    };
}

template <BookPredicate... Preds>
inline auto any_of(Preds... preds) {
    return [=](const Book& b) {
        return (preds(b) || ...);
    };
}

template <typename It, BookPredicate Pred>
inline std::vector<std::reference_wrapper<const Book>>
filterBooks(It first, It last, Pred pred) {
    std::vector<std::reference_wrapper<const Book>> out;
    for (; first != last; ++first) {
        if (pred(*first)) {
            out.emplace_back(std::cref(*first));
        }
    }
    return out;
}

}  // namespace bookdb