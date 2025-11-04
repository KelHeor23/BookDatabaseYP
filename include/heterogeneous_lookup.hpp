#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
        return lhs < rhs;
    }

    bool operator()(const std::string& lhs, std::string_view rhs) const noexcept {
        return std::string_view(lhs) < rhs;
    }
    bool operator()(std::string_view lhs, const std::string& rhs) const noexcept {
        return lhs < std::string_view(rhs);
    }
    bool operator()(const char* lhs, std::string_view rhs) const noexcept {
        return std::string_view(lhs) < rhs;
    }
    bool operator()(std::string_view lhs, const char* rhs) const noexcept {
        return lhs < std::string_view(rhs);
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(const std::string& lhs, std::string_view rhs) const noexcept {
        return std::string_view(lhs) == rhs;
    }
    bool operator()(std::string_view lhs, const std::string& rhs) const noexcept {
        return lhs == std::string_view(rhs);
    }
    bool operator()(const char* lhs, std::string_view rhs) const noexcept {
        return std::string_view(lhs) == rhs;
    }
    bool operator()(std::string_view lhs, const char* rhs) const noexcept {
        return lhs == std::string_view(rhs);
    }
};

struct TransparentStringHash {
    using is_transparent = void;

    std::size_t operator()(std::string_view s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }

    std::size_t operator()(const std::string& s) const noexcept {
        return (*this)(std::string_view(s));
    }

    std::size_t operator()(const char* s) const noexcept {
        return (*this)(std::string_view(s));
    }
};

}  // namespace bookdb
