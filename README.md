# cpp-pyutils - A C++ Library for Python-like Functions

cpp-pyutils is a lightweight C++ library that simplifies common tasks by providing Python-like functions such as `print` and `input` and a lot more. This library aims to make C++ programming more accessible and user-friendly, especially for those familiar with Python.


## Features

### I/O

* print(), input()

### Conversions

* str(), to_int(), to_double()

### Container helpers

* len(), Range, enumerate(), zip(), reversed()

### Functional utilities

* map(), filter(), sum(), max(), min()

### String utilities

* join(), split(),
* startswith(), endswith()
* strip(), lstrip(), rstrip()

### Checks

* isdigit_all(), isalpha_all()

### File I/O

* read_entire_file(), write_text_file()


## Installation

To use cpp-pyutils, you can clone the repository and build it using CMake:

```bash
git clone https://github.com/xZepyx/cpp-pyutils.git
cd cpp-pyutils
mkdir build
cd build
cmake ..
make
sudo make install
```

Alternatively, you can use Conan to manage dependencies and build the library.

## Usage

Here is a simple example of how to use the library:

```cpp
#include <pyutils/pyutils.hpp>

int main() {
    print("Hello, World!");
    std::string name = input("Enter your name: ");
    print("Hello, " + name + "!");
    return 0;
}
```

> For more examples read [examples.cpp](examples/example.cpp)

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for any enhancements or bug fixes.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.