#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    static std::string_view to_sv(std::string_view v) noexcept { return v; }
    static std::string_view to_sv(const std::string& s) noexcept { return s; }
    static std::string_view to_sv(const char* s) noexcept { return s; }

    template <class L, class R>
    bool operator()(const L& lhs, const R& rhs) const noexcept {
        return to_sv(lhs) < to_sv(rhs);
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    static std::string_view to_sv(std::string_view v) noexcept { return v; }
    static std::string_view to_sv(const std::string& s) noexcept { return s; }
    static std::string_view to_sv(const char* s) noexcept { return s; }

    template <class L, class R>
    bool operator()(const L& lhs, const R& rhs) const noexcept {
        return to_sv(lhs) == to_sv(rhs);
    }
};

struct TransparentStringHash {
    using is_transparent = void;

    static std::string_view to_sv(std::string_view v) noexcept { return v; }
    static std::string_view to_sv(const std::string& s) noexcept { return s; }
    static std::string_view to_sv(const char* s) noexcept { return s; }

    template <class T>
    std::size_t operator()(const T& v) const noexcept {
        return std::hash<std::string_view>{}(to_sv(v));
    }
};

}  // namespace bookdb
