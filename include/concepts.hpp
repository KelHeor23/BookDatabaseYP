#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = 
    std::ranges::range<T> &&
    std::convertible_to<std::ranges::range_reference_t<T>, const Book&>;;

template <typename T>
concept BookIterator = 
    std::input_iterator<T> &&
    std::convertible_to<std::iter_reference_t<T>, const Book&>;

template <typename S, typename I>
concept BookSentinel = 
    std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = 
    std::predicate<P, const Book&>;

template <typename C>
concept BookComparator =
    std::strict_weak_order<C, const Book&, const Book&>;

}  // namespace bookdb