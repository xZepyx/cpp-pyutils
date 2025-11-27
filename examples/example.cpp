#include "pyutils.cpp"

int main() {
    // 1. print() function - outputs text with newline
    pyutils::print("Hello, World!");

    // 2. input() function - prompts user and reads input
    std::string name = pyutils::input("Enter your name: ");
    pyutils::print("Hello, " + name + "!");

    // 3. Variadic print function - prints multiple arguments separated by spaces
    pyutils::print("This", "is", "a", "test");

    // 4. str() function - converts value to string
    std::string num_str = pyutils::str(123);
    pyutils::print("String representation of number: " + num_str);

    // 5. to_int() and to_double() functions - convert strings to numbers
    auto int_value = pyutils::to_int("42");
    auto double_value = pyutils::to_double("3.14");
    if (int_value) pyutils::print("Converted to int: " + pyutils::str(*int_value));
    if (double_value) pyutils::print("Converted to double: " + pyutils::str(*double_value));

    // 6. len() function - returns length of container/string
    size_t length = pyutils::len("Hello");
    pyutils::print("Length of 'Hello': " + pyutils::str(length));

    // 7. Range class - iterable integer range
    pyutils::print("Range from 0 to 4:");
    for (auto i : pyutils::Range(5)) {
        pyutils::print(pyutils::str(i));
    }

    // 8. enumerate() function - returns vector of (index, value) pairs
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};
    auto enumerated = pyutils::enumerate(fruits);
    pyutils::print("Enumerate example:");
    for (auto [idx, val] : enumerated) {
        pyutils::print("Index: " + pyutils::str(idx) + ", Value: " + val);
    }

    // 9. zip() function - combines two containers into vector of pairs
    std::vector<int> nums = {1, 2, 3};
    std::vector<char> chars = {'a', 'b', 'c'};
    auto zipped = pyutils::zip(nums, chars);
    pyutils::print("Zip example:");
    for (auto [n, c] : zipped) {
        pyutils::print("Num: " + pyutils::str(n) + ", Char: " + pyutils::str(c));
    }

    // 10. map() function - apply function to each element
    auto squared = pyutils::map([](int x) { return x * x; }, std::vector<int>{1, 2, 3, 4});
    pyutils::print("Map (squared) example: ");
    for (auto val : squared) pyutils::print(pyutils::str(val));

    // 11. filter() function - keep elements where predicate is true
    auto filtered = pyutils::filter([](int x) { return x > 2; }, std::vector<int>{1, 2, 3, 4});
    pyutils::print("Filter (x > 2) example:");
    for (auto val : filtered) pyutils::print(pyutils::str(val));

    // 12. sum() function - accumulate container elements
    int total = pyutils::sum(std::vector<int>{1, 2, 3, 4});
    pyutils::print("Sum of [1,2,3,4]: " + pyutils::str(total));

    // 13. max() and min() functions - find maximum and minimum values
    int max_val = pyutils::max(std::vector<int>{1, 5, 3, 9, 2});
    int min_val = pyutils::min(std::vector<int>{1, 5, 3, 9, 2});
    pyutils::print("Max: " + pyutils::str(max_val) + ", Min: " + pyutils::str(min_val));

    // 14. join() function - combine vector of strings with separator
    std::vector<std::string> words = {"Hello", "World"};
    std::string joined = pyutils::join(words, " ");
    pyutils::print("Joined: " + joined);

    // 15. split() function - split string by delimiter
    auto split_result = pyutils::split("Hello World Test", ' ');
    pyutils::print("Split result:");
    for (auto word : split_result) pyutils::print(word);

    // 16. startswith() and endswith() functions - check string prefix/suffix
    bool starts = pyutils::startswith("Hello World", "Hello");
    bool ends = pyutils::endswith("Hello World", "World");
    pyutils::print("Starts with 'Hello': " + pyutils::str(starts));
    pyutils::print("Ends with 'World': " + pyutils::str(ends));

    // 17. strip(), lstrip(), rstrip() functions - remove whitespace
    std::string stripped = pyutils::strip("   Hello World   ");
    std::string lstripped = pyutils::lstrip("   Hello   ");
    std::string rstripped = pyutils::rstrip("   Hello   ");
    pyutils::print("Stripped: '" + stripped + "'");
    pyutils::print("LStripped: '" + lstripped + "'");
    pyutils::print("RStripped: '" + rstripped + "'");

    // 18. isdigit_all() and isalpha_all() functions - check character types
    bool is_digit = pyutils::isdigit_all("12345");
    bool is_alpha = pyutils::isalpha_all("abcdef");
    pyutils::print("Is '12345' all digits? " + pyutils::str(is_digit));
    pyutils::print("Is 'abcdef' all alphabets? " + pyutils::str(is_alpha));

    // 19. reversed() function - return reversed copy of container
    auto reversed_list = pyutils::reversed(std::vector<int>{1, 2, 3, 4});
    pyutils::print("Reversed list:");
    for (auto val : reversed_list) pyutils::print(pyutils::str(val));

    // 20. read_entire_file() and write_text_file() functions - file I/O
    pyutils::write_text_file("test.txt", "Hello, File!");
    auto content = pyutils::read_entire_file("test.txt");
    if (content) {
        pyutils::print("File content: " + *content);
    }

    return 0;
}