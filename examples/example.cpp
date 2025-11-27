#include "pyutils.hpp"

int main() {
    // 1. print() — simple print with newline
    pyutils::print("Hello, World!");

    // 2. input() — prompt user
    std::string name = pyutils::input("Enter your name: ");
    pyutils::print("Hello,", name + "!");

    // 3. print() variadic — multiple arguments
    pyutils::print("This", "is", "a", "test");

    // 4. str() — convert to string
    std::string num_str = pyutils::str(123);
    pyutils::print("String representation:", num_str);

    // 5. to_int() / to_double()
    auto int_value = pyutils::to_int("42");
    auto double_value = pyutils::to_double("3.14");

    if (int_value)
        pyutils::print("Converted to int:", pyutils::str(*int_value));

    if (double_value)
        pyutils::print("Converted to double:", pyutils::str(*double_value));

    // 6. len()
    pyutils::print("Length of 'Hello':", pyutils::str(pyutils::len("Hello")));

    // 7. Range
    pyutils::print("Range from 0 to 4:");
    for (int i : pyutils::Range(5))
        pyutils::print(i);

    // 8. enumerate()
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};
    pyutils::print("Enumerate fruits:");
    for (auto [i, v] : pyutils::enumerate(fruits))
        pyutils::print("Index:", pyutils::str(i), "Value:", v);

    // 9. zip()
    std::vector<int> nums = {1, 2, 3};
    std::vector<char> chars = {'a', 'b', 'c'};
    pyutils::print("Zip example:");
    for (auto [n, c] : pyutils::zip(nums, chars))
        pyutils::print("Num:", pyutils::str(n), "Char:", std::string(1, c));

    // 10. map()
    auto squared = pyutils::map([](int x) { return x * x; }, nums);
    pyutils::print("Map (square):");
    for (int v : squared)
        pyutils::print(v);

    // 11. filter()
    auto filtered = pyutils::filter([](int x) { return x > 1; }, nums);
    pyutils::print("Filter (>1):");
    for (int v : filtered)
        pyutils::print(v);

    // 12. sum()
    pyutils::print("Sum of nums:", pyutils::str(pyutils::sum(nums)));

    // 13. max() / min()
    std::vector<int> values = {1, 5, 3, 9, 2};
    pyutils::print("Max:", pyutils::str(pyutils::max(values)));
    pyutils::print("Min:", pyutils::str(pyutils::min(values)));

    // 14. join()
    std::vector<std::string> words = {"Hello", "World"};
    pyutils::print("Joined:", pyutils::join(words, " "));

    // 15. split()
    auto split_res = pyutils::split("Hello World Test", ' ');
    pyutils::print("Split result:");
    for (auto& w : split_res)
        pyutils::print(w);

    // 16. startswith / endswith
    pyutils::print("Starts with 'Hello':",
                   pyutils::str(pyutils::startswith("Hello World", "Hello")));
    pyutils::print("Ends with 'World':",
                   pyutils::str(pyutils::endswith("Hello World", "World")));

    // 17. strip / lstrip / rstrip
    pyutils::print("Strip:'" + pyutils::strip("   Hi   ") + "'");
    pyutils::print("LStrip:'" + pyutils::lstrip("   Hi") + "'");
    pyutils::print("RStrip:'" + pyutils::rstrip("Hi   ") + "'");

    // 18. isdigit_all / isalpha_all
    pyutils::print("Is '12345' digit:", pyutils::str(pyutils::isdigit_all("12345")));
    pyutils::print("Is 'abcdef' alpha:", pyutils::str(pyutils::isalpha_all("abcdef")));

    // 19. reversed()
    auto rev = pyutils::reversed(std::vector<int>{1, 2, 3});
    pyutils::print("Reversed list:");
    for (int x : rev)
        pyutils::print(x);

    // 20. read_entire_file / write_text_file
    pyutils::write_text_file("test.txt", "Hello, File!");
    auto content = pyutils::read_entire_file("test.txt");

    if (content)
        pyutils::print("File content:", *content);

    return 0;
}
