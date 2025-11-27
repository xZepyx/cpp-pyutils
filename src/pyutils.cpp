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

namespace pyutils {

void print(const std::string& message) {
    std::cout << message << std::endl;
}

std::string input(const std::string& prompt) {
    std::string userInput;
    std::cout << prompt;
    std::getline(std::cin, userInput);
    return userInput;
}

// convenience overload with no prompt
std::string input() {
    return input("");
}

// variadic print (simple)
template<typename T>
static void print_one(const T& v) {
    std::cout << v;
}
template<typename T, typename... Ts>
static void print_one(const T& v, const Ts&... vs) {
    std::cout << v << ' ';
    print_one(vs...);
}

template<typename... Args>
void print(const Args&... args) {
    print_one(args...);
    std::cout << std::endl;
}

// str() - stringify
template<typename T>
std::string str(const T& v) {
    std::ostringstream os;
    os << v;
    return os.str();
}
inline std::string str(const std::string& s) { return s; }
inline std::string str(const char* s) { return std::string(s); }

// to_int / to_double helpers
std::optional<long long> to_int(const std::string& s, int base = 10) {
    try {
        size_t idx = 0;
        long long v = std::stoll(s, &idx, base);
        if (idx != s.size()) return std::nullopt;
        return v;
    } catch (...) {
        return std::nullopt;
    }
}
std::optional<double> to_double(const std::string& s) {
    try {
        size_t idx = 0;
        double v = std::stod(s, &idx);
        if (idx != s.size()) return std::nullopt;
        return v;
    } catch (...) {
        return std::nullopt;
    }
}

// len() for containers / strings
template<typename C>
auto len(const C& c) -> decltype(std::distance(std::begin(c), std::end(c))) {
    return static_cast<decltype(std::distance(std::begin(c), std::end(c)))>(std::distance(std::begin(c), std::end(c)));
}
inline size_t len(const std::string& s) { return s.size(); }

// range - simple integer range iterable
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

// enumerate -> vector<pair<size_t, T>>
template<typename C>
auto enumerate(const C& c) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::vector<std::pair<size_t, T>> out;
    size_t i = 0;
    for (auto&& el : c) out.emplace_back(i++, el);
    return out;
}

// zip for two containers -> vector<pair<T1,T2>>
template<typename C1, typename C2>
auto zip(const C1& a, const C2& b) {
    using T1 = typename std::decay<decltype(*std::begin(a))>::type;
    using T2 = typename std::decay<decltype(*std::begin(b))>::type;
    std::vector<std::pair<T1, T2>> out;
    auto it1 = std::begin(a);
    auto it2 = std::begin(b);
    for (; it1 != std::end(a) && it2 != std::end(b); ++it1, ++it2) out.emplace_back(*it1, *it2);
    return out;
}

// map: apply f to each element, return vector
template<typename F, typename C>
auto map(F f, const C& c) {
    using R = typename std::decay<decltype(f(*std::begin(c)))>::type;
    std::vector<R> out;
    out.reserve(std::distance(std::begin(c), std::end(c)));
    for (auto&& el : c) out.push_back(f(el));
    return out;
}

// filter: keep elements where pred is true
template<typename P, typename C>
auto filter(P pred, const C& c) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::vector<T> out;
    for (auto&& el : c) if (pred(el)) out.push_back(el);
    return out;
}

// sum - numeric accumulate
template<typename C, typename T = typename std::decay<decltype(*std::begin(std::declval<C>()))>::type>
auto sum(const C& c, T init = T{}) {
    return std::accumulate(std::begin(c), std::end(c), init);
}

// max/min for container (throws if empty)
template<typename C>
auto max(const C& c) {
    if (std::begin(c) == std::end(c)) throw std::runtime_error("max() arg is an empty container");
    return *std::max_element(std::begin(c), std::end(c));
}
template<typename C>
auto min(const C& c) {
    if (std::begin(c) == std::end(c)) throw std::runtime_error("min() arg is an empty container");
    return *std::min_element(std::begin(c), std::end(c));
}

// join / split
std::string join(const std::vector<std::string>& parts, const std::string& sep = "") {
    std::ostringstream os;
    bool first = true;
    for (const auto& p : parts) {
        if (!first) os << sep;
        os << p;
        first = false;
    }
    return os.str();
}
std::vector<std::string> split(const std::string& s, char delim = ' ') {
    std::vector<std::string> out;
    std::string token;
    std::istringstream is(s);
    while (std::getline(is, token, delim)) out.push_back(token);
    return out;
}

// startswith / endswith
bool startswith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && std::equal(p.begin(), p.end(), s.begin());
}
bool endswith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && std::equal(p.rbegin(), p.rbegin() + p.size(), s.rbegin());
}

// strip / lstrip / rstrip
std::string strip(const std::string& s, const std::string& chars = " \t\n\r") {
    auto left = s.find_first_not_of(chars);
    if (left == std::string::npos) return "";
    auto right = s.find_last_not_of(chars);
    return s.substr(left, right - left + 1);
}
std::string lstrip(const std::string& s, const std::string& chars = " \t\n\r") {
    auto left = s.find_first_not_of(chars);
    if (left == std::string::npos) return "";
    return s.substr(left);
}
std::string rstrip(const std::string& s, const std::string& chars = " \t\n\r") {
    auto right = s.find_last_not_of(chars);
    if (right == std::string::npos) return "";
    return s.substr(0, right + 1);
}

// isdigit / isalpha for strings
bool isdigit_all(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isdigit(c); });
}
bool isalpha_all(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isalpha(c); });
}

// reversed -> returns vector of elements in reverse order
template<typename C>
auto reversed(const C& c) {
    using T = typename std::decay<decltype(*std::begin(c))>::type;
    std::vector<T> out(std::begin(c), std::end(c));
    std::reverse(out.begin(), out.end());
    return out;
}

// trivial file helpers (read_entire_file / write_text) - minimal
std::optional<std::string> read_entire_file(const std::string& path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs) return std::nullopt;
    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}
bool write_text_file(const std::string& path, const std::string& content) {
    std::ofstream ofs(path);
    if (!ofs) return false;
    ofs << content;
    return true;
}

}