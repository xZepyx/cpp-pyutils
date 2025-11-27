#pragma once
#ifndef PYUTILS_HPP
#define PYUTILS_HPP

// pyutils.hpp — single-header, lightweight "Python-like" utilities for C++
// A cleaned, organized, header-only version of your library.
// Usage: #include "pyutils.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <cctype>
#include <optional>
#include <type_traits>
#include <iterator>
#include <stdexcept>
#include <initializer_list>
#include <map>
#include <set>
#include <filesystem>

namespace pyutils {

// -----------------------------
// Basic I/O
// -----------------------------

inline void print() {
    std::cout << std::endl;
}

template<typename T>
inline void print_one(const T& v) {
    std::cout << v;
}

template<typename T, typename... Ts>
inline void print_one(const T& v, const Ts&... vs) {
    std::cout << v << ' ';
    print_one(vs...);
}

template<typename... Args>
inline void print(const Args&... args) {
    print_one(args...);
    std::cout << std::endl;
}

inline std::string input(const std::string& prompt = "") {
    if (!prompt.empty()) std::cout << prompt;
    std::string res;
    std::getline(std::cin, res);
    return res;
}

// -----------------------------
// String conversions
// -----------------------------

template<typename T>
inline std::string str(const T& v) {
    std::ostringstream os;
    os << v;
    return os.str();
}
inline std::string str(const std::string& s) { return s; }
inline std::string str(const char* s) { return std::string(s); }

// -----------------------------
// Parsing helpers
// -----------------------------

inline std::optional<long long> to_int(const std::string& s, int base = 10) {
    try {
        size_t idx = 0;
        long long v = std::stoll(s, &idx, base);
        if (idx != s.size()) return std::nullopt;
        return v;
    } catch (...) {
        return std::nullopt;
    }
}

inline std::optional<double> to_double(const std::string& s) {
    try {
        size_t idx = 0;
        double v = std::stod(s, &idx);
        if (idx != s.size()) return std::nullopt;
        return v;
    } catch (...) {
        return std::nullopt;
    }
}

inline std::optional<bool> to_bool(const std::string& s) {
    std::string t;
    t.reserve(s.size());
    std::transform(s.begin(), s.end(), std::back_inserter(t), [](unsigned char c){ return std::tolower(c); });
    if (t == "true" || t == "1" || t == "yes" || t == "y") return true;
    if (t == "false" || t == "0" || t == "no" || t == "n") return false;
    return std::nullopt;
}

// -----------------------------
// Length helpers
// -----------------------------

template<typename C>
inline auto len(const C& c) -> decltype(std::distance(std::begin(c), std::end(c))) {
    return static_cast<decltype(std::distance(std::begin(c), std::end(c)))>(std::distance(std::begin(c), std::end(c)));
}
inline size_t len(const std::string& s) { return s.size(); }

// -----------------------------
// Range (simple)
// -----------------------------

class Range {
public:
    struct iterator {
        long long cur;
        long long step;
        bool operator!=(const iterator& o) const { return step > 0 ? cur < o.cur : cur > o.cur; }
        long long operator*() const { return cur; }
        iterator& operator++() { cur += step; return *this; }
    };

    Range(long long stop) : start_(0), stop_(stop), step_(1) {}
    Range(long long start, long long stop, long long step = 1) : start_(start), stop_(stop), step_(step) {}
    iterator begin() const { return { start_, step_ }; }
    iterator end() const { return { stop_, step_ }; }
private:
    long long start_;
    long long stop_;
    long long step_;
};

// -----------------------------
// Itertools-like helpers
// -----------------------------

template<typename C>
inline auto enumerate(const C& c) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::vector<std::pair<size_t, T>> out;
    size_t i = 0;
    for (auto&& el : c) out.emplace_back(i++, el);
    return out;
}

template<typename C1, typename C2>
inline auto zip(const C1& a, const C2& b) {
    using T1 = typename std::decay<decltype(*std::begin(a))>::type;
    using T2 = typename std::decay<decltype(*std::begin(b))>::type;
    std::vector<std::pair<T1, T2>> out;
    auto it1 = std::begin(a);
    auto it2 = std::begin(b);
    for (; it1 != std::end(a) && it2 != std::end(b); ++it1, ++it2) out.emplace_back(*it1, *it2);
    return out;
}

template<typename F, typename C>
inline auto map(F f, const C& c) {
    using R = typename std::decay<decltype(f(*std::begin(c)))>::type;
    std::vector<R> out;
    out.reserve(std::distance(std::begin(c), std::end(c)));
    for (auto&& el : c) out.push_back(f(el));
    return out;
}

template<typename P, typename C>
inline auto filter(P pred, const C& c) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::vector<T> out;
    for (auto&& el : c) if (pred(el)) out.push_back(el);
    return out;
}

// chain two containers
template<typename C1, typename C2>
inline auto chain(const C1& a, const C2& b) {
    using V1 = typename std::decay<decltype(*std::begin(a))>::type;
    std::vector<V1> out;
    out.reserve(std::distance(std::begin(a), std::end(a)) + std::distance(std::begin(b), std::end(b)));
    out.insert(out.end(), std::begin(a), std::end(a));
    out.insert(out.end(), std::begin(b), std::end(b));
    return out;
}

// product of two containers
template<typename C1, typename C2>
inline auto product(const C1& a, const C2& b) {
    using T1 = typename std::decay<decltype(*std::begin(a))>::type;
    using T2 = typename std::decay<decltype(*std::begin(b))>::type;
    std::vector<std::pair<T1, T2>> out;
    for (auto&& x : a) for (auto&& y : b) out.emplace_back(x, y);
    return out;
}

// accumulate (prefix sums)
template<typename C>
inline auto accumulate_prefix(const C& c) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::vector<T> out;
    T acc{};
    for (auto&& v : c) { acc = acc + v; out.push_back(acc); }
    return out;
}

// chunking
template<typename C>
inline auto chunk(const C& c, size_t size) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::vector<std::vector<T>> out;
    auto it = std::begin(c);
    auto end = std::end(c);
    while (it != end) {
        std::vector<T> part;
        for (size_t i = 0; i < size && it != end; ++i, ++it) part.push_back(*it);
        out.push_back(std::move(part));
    }
    return out;
}

// take / drop
template<typename C>
inline C take(const C& c, size_t n) {
    auto it = std::begin(c);
    C out;
    for (size_t i = 0; i < n && it != std::end(c); ++i, ++it) out.insert(out.end(), *it);
    return out;
}

template<typename C>
inline C drop(const C& c, size_t n) {
    auto it = std::begin(c);
    for (size_t i = 0; i < n && it != std::end(c); ++i) ++it;
    C out;
    for (; it != std::end(c); ++it) out.insert(out.end(), *it);
    return out;
}

// any / all
template<typename C>
inline bool any(const C& c) {
    for (auto&& v : c) if (v) return true;
    return false;
}

template<typename C>
inline bool all(const C& c) {
    for (auto&& v : c) if (!v) return false;
    return true;
}

// find_index / contains
template<typename C, typename T>
inline std::optional<size_t> find_index(const C& c, const T& value) {
    size_t i = 0;
    for (auto&& v : c) { if (v == value) return i; ++i; }
    return std::nullopt;
}

template<typename C, typename T>
inline bool contains(const C& c, const T& value) {
    for (auto&& v : c) if (v == value) return true;
    return false;
}

// unique (keep order)
template<typename C>
inline C unique(const C& c) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::set<T> seen;
    C out;
    for (auto&& v : c) if (seen.insert(v).second) out.insert(out.end(), v);
    return out;
}

// sorted copy
template<typename C>
inline C sorted(const C& c) {
    C out(std::begin(c), std::end(c));
    std::sort(out.begin(), out.end());
    return out;
}

// clamp
template<typename T>
inline T clamp(T v, T lo, T hi) {
    return std::min(hi, std::max(lo, v));
}

// -----------------------------
// String utilities
// -----------------------------

inline std::string join(const std::vector<std::string>& parts, const std::string& sep = "") {
    std::ostringstream os;
    bool first = true;
    for (const auto& p : parts) {
        if (!first) os << sep;
        os << p;
        first = false;
    }
    return os.str();
}

inline std::vector<std::string> split(const std::string& s, char delim = ' ') {
    std::vector<std::string> out;
    std::string token;
    std::istringstream is(s);
    while (std::getline(is, token, delim)) out.push_back(token);
    return out;
}

inline bool startswith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && std::equal(p.begin(), p.end(), s.begin());
}
inline bool endswith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && std::equal(p.rbegin(), p.rbegin() + p.size(), s.rbegin());
}

inline std::string strip(const std::string& s, const std::string& chars = " \t\n\r") {
    auto left = s.find_first_not_of(chars);
    if (left == std::string::npos) return "";
    auto right = s.find_last_not_of(chars);
    return s.substr(left, right - left + 1);
}
inline std::string lstrip(const std::string& s, const std::string& chars = " \t\n\r") {
    auto left = s.find_first_not_of(chars);
    if (left == std::string::npos) return "";
    return s.substr(left);
}
inline std::string rstrip(const std::string& s, const std::string& chars = " \t\n\r") {
    auto right = s.find_last_not_of(chars);
    if (right == std::string::npos) return "";
    return s.substr(0, right + 1);
}

inline std::string replace_all(std::string s, const std::string& from, const std::string& to) {
    if (from.empty()) return s;
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
    return s;
}

inline std::string lower(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    std::transform(s.begin(), s.end(), std::back_inserter(out), [](unsigned char c){ return std::tolower(c); });
    return out;
}
inline std::string upper(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    std::transform(s.begin(), s.end(), std::back_inserter(out), [](unsigned char c){ return std::toupper(c); });
    return out;
}

inline bool isdigit_all(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isdigit(c); });
}
inline bool isalpha_all(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isalpha(c); });
}
inline bool isalnum_all(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isalnum(c); });
}
inline bool isspace_all(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isspace(c); });
}

// -----------------------------
// Containers
// -----------------------------

template<typename C>
inline auto reversed(const C& c) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::vector<T> out(std::begin(c), std::end(c));
    std::reverse(out.begin(), out.end());
    return out;
}

// reversed view (pair of reverse iterators)
template<typename C>
inline auto reversed_view(const C& c) {
    return std::make_pair(c.rbegin(), c.rend());
}

// sum, max, min
template<typename C, typename T = typename std::decay<decltype(*std::begin(std::declval<C>()))>::type>
inline auto sum(const C& c, T init = T{}) {
    return std::accumulate(std::begin(c), std::end(c), init);
}

template<typename C>
inline auto max(const C& c) {
    if (std::begin(c) == std::end(c)) throw std::runtime_error("max() arg is an empty container");
    return *std::max_element(std::begin(c), std::end(c));
}

template<typename C>
inline auto min(const C& c) {
    if (std::begin(c) == std::end(c)) throw std::runtime_error("min() arg is an empty container");
    return *std::min_element(std::begin(c), std::end(c));
}

// unique, sorted are above

// -----------------------------
// Map helpers
// -----------------------------

template<typename K, typename V>
inline std::optional<V> get(const std::map<K,V>& m, const K& key) {
    auto it = m.find(key);
    if (it == m.end()) return std::nullopt;
    return it->second;
}

inline std::string join_map(const std::map<std::string,std::string>& m, const std::string& sep = ",", const std::string& eq = "=") {
    std::vector<std::string> parts;
    parts.reserve(m.size());
    for (auto&& kv : m) parts.push_back(kv.first + eq + kv.second);
    return join(parts, sep);
}

// -----------------------------
// File helpers
// -----------------------------

inline bool file_exists(const std::string& path) {
    return std::filesystem::exists(path);
}

inline std::optional<std::string> read_entire_file(const std::string& path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs) return std::nullopt;
    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

inline bool write_text_file(const std::string& path, const std::string& content) {
    std::ofstream ofs(path);
    if (!ofs) return false;
    ofs << content;
    return true;
}

// -----------------------------
// Misc
// -----------------------------

inline std::string replace(std::string s, const std::string& from, const std::string& to, bool all=true) {
    if (from.empty()) return s;
    size_t pos = 0;
    if (all) {
        while ((pos = s.find(from, pos)) != std::string::npos) {
            s.replace(pos, from.length(), to);
            pos += to.length();
        }
    } else {
        pos = s.find(from);
        if (pos != std::string::npos) s.replace(pos, from.length(), to);
    }
    return s;
}

} // namespace pyutils

#endif // PYUTILS_HPP
