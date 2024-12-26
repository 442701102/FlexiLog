Certainly! Below is a detailed `README.md` for your C++ logging library, **FlexiLog**. This README includes sections that cover the library's purpose, features, installation instructions, usage examples, API overview, contribution guidelines, and licensing information.

---

# FlexiLog

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-17%2B-blue.svg)
![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)

**FlexiLog** is a lightweight, high-performance C++ logging library designed to facilitate efficient debugging and monitoring. Unlike other popular logging libraries, FlexiLog offers compile-time log control with zero-overhead disabling, rich enumeration options for detailed debug information, customizable print restrictions, and operates independently without any external dependencies.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
  - [Basic Logging](#basic-logging)
  - [Template-Based Log Control](#template-based-log-control)
  - [Rich Debug Information](#rich-debug-information)
  - [Print Restrictions](#print-restrictions)
  - [Custom Debug Information](#custom-debug-information)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)

## Features

- **Template-Based Log Control**: Enable or disable logging at compile time with zero runtime overhead.
- **Rich Enumeration Options**: Choose from a variety of debug information such as line number, filename, CPU usage, and more.
- **Customizable Print Restrictions**: Limit the number of log prints, control print frequency, and set complex conditions to manage log output effectively.
- **User-Defined Debug Information**: Extend the library with custom debug details tailored to your project's needs.
- **No External Dependencies**: Pure C++ implementation ensuring easy integration into any project without additional library requirements.
- **High Performance**: Optimized for minimal impact on application performance, especially when logging is disabled.

## Installation

### Prerequisites

- **C++17** or later
- Compatible with major compilers like GCC, Clang, and MSVC

### Using CMake

1. **Clone the Repository**

   ```bash
   git clone https://github.com/yourusername/FlexiLog.git
   cd FlexiLog
   ```

2. **Build and Install**

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   sudo make install
   ```

### Manual Installation

1. **Download Source Files**

   Download the `FlexiLog.hpp` header file from the repository.

2. **Include in Your Project**

   Place the `FlexiLog.hpp` file into your project's include directory and include it in your source files:

   ```cpp
   #include "FlexiLog.hpp"
   ```

## Usage

### Basic Logging

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::log(FlexiLog::LogLevel::INFO, "Application started.");
    FlexiLog::log(FlexiLog::LogLevel::DEBUG, "Debugging value: {}", 42);
    FlexiLog::log(FlexiLog::LogLevel::ERROR, "An error occurred: {}", "Out of memory");
    return 0;
}
```

### Template-Based Log Control

Enable or disable logging at compile time by defining or undefining the `ENABLE_FLEXILOG` macro.

```cpp
// To enable logging
#define ENABLE_FLEXILOG
#include "FlexiLog.hpp"

// To disable logging
// #define ENABLE_FLEXILOG
#include "FlexiLog.hpp"
```

When logging is disabled, all `FlexiLog::log` calls are removed at compile time, ensuring no performance overhead.

### Rich Debug Information

Choose the debug information to include in your logs using enumerations.

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::setDetails({
        FlexiLog::LogDetail::LineNumber,
        FlexiLog::LogDetail::FileName,
        FlexiLog::LogDetail::CPUUsage
    });

    FlexiLog::log(FlexiLog::LogLevel::INFO, "Detailed log entry.");
    return 0;
}
```

### Print Restrictions

Control the number of times a log message is printed and its frequency.

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::setMaxPrintCount(5); // Maximum 5 prints
    FlexiLog::setPrintFrequency(10); // Maximum 10 prints per second

    for(int i = 0; i < 100; ++i) {
        FlexiLog::log(FlexiLog::LogLevel::DEBUG, "This will be printed up to 5 times at 10 prints per second.");
    }
    return 0;
}
```

### Custom Debug Information

Add your own debug information by registering custom functions.

```cpp
#include "FlexiLog.hpp"

std::string getCustomInfo() {
    // Implement your custom info retrieval
    return "CustomInfo";
}

int main() {
    FlexiLog::addCustomDetail(getCustomInfo);

    FlexiLog::log(FlexiLog::LogLevel::INFO, "Log with custom info.");
    return 0;
}
```

## API Reference

### `FlexiLog::LogLevel`

Enumerates the severity levels of logs.

- `TRACE`
- `DEBUG`
- `INFO`
- `WARN`
- `ERROR`
- `FATAL`

### `FlexiLog::LogDetail`

Enumerates the types of debug information available.

- `LineNumber`
- `FileName`
- `FunctionName`
- `Timestamp`
- `CPUUsage`
- `MemoryUsage`
- `Custom`

### `FlexiLog::log`

Logs a message with the specified log level.

```cpp
template<typename... Args>
static void log(LogLevel level, const std::string& message, Args&&... args);
```

### `FlexiLog::setDetails`

Sets the details to include in each log entry.

```cpp
static void setDetails(const std::vector<LogDetail>& details);
```

### `FlexiLog::addCustomDetail`

Registers a custom function to retrieve additional debug information.

```cpp
static void addCustomDetail(std::function<std::string()> customFunc);
```

### `FlexiLog::setMaxPrintCount`

Sets the maximum number of times a specific log message can be printed.

```cpp
static void setMaxPrintCount(int count);
```

### `FlexiLog::setPrintFrequency`

Sets the maximum number of log prints allowed per second.

```cpp
static void setPrintFrequency(int maxPerSecond);
```

### `FlexiLog::setPrintCondition`

Sets a custom condition that must be met for a log to be printed.

```cpp
static void setPrintCondition(std::function<bool()> condition);
```

## Examples

### Example 1: Basic Logging

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::log(FlexiLog::LogLevel::INFO, "Starting the application...");
    FlexiLog::log(FlexiLog::LogLevel::DEBUG, "Variable x = {}", 10);
    FlexiLog::log(FlexiLog::LogLevel::ERROR, "Failed to load resource: {}", "config.yaml");
    return 0;
}
```

### Example 2: Enabling Detailed Debug Information

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::setDetails({
        FlexiLog::LogDetail::Timestamp,
        FlexiLog::LogDetail::FileName,
        FlexiLog::LogDetail::LineNumber,
        FlexiLog::LogDetail::CPUUsage
    });

    FlexiLog::log(FlexiLog::LogLevel::INFO, "Application initialized with detailed debug info.");
    return 0;
}
```

### Example 3: Limiting Log Prints

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::setMaxPrintCount(3);
    FlexiLog::setPrintFrequency(2); // 2 prints per second

    for(int i = 0; i < 10; ++i) {
        FlexiLog::log(FlexiLog::LogLevel::DEBUG, "This debug message will be limited.");
    }

    return 0;
}
```

### Example 4: Adding Custom Debug Information

```cpp
#include "FlexiLog.hpp"

std::string getUserID() {
    // Assume we retrieve a user ID from somewhere
    return "User123";
}

int main() {
    FlexiLog::addCustomDetail(getUserID);

    FlexiLog::log(FlexiLog::LogLevel::INFO, "User action logged.");
    return 0;
}
```

## Contributing

Contributions are welcome! Whether it's reporting bugs, suggesting features, or submitting pull requests, your input helps make FlexiLog better.

### How to Contribute

1. **Fork the Repository**

   Click the "Fork" button at the top right of the repository page.

2. **Clone Your Fork**

   ```bash
   git clone https://github.com/yourusername/FlexiLog.git
   cd FlexiLog
   ```

3. **Create a New Branch**

   ```bash
   git checkout -b feature/YourFeatureName
   ```

4. **Make Your Changes**

   Implement your feature or fix bugs.

5. **Commit Your Changes**

   ```bash
   git commit -m "Add feature: YourFeatureName"
   ```

6. **Push to Your Fork**

   ```bash
   git push origin feature/YourFeatureName
   ```

7. **Create a Pull Request**

   Go to the original repository and create a pull request from your fork.

### Coding Standards

- Follow C++ best practices and coding standards.
- Ensure code is well-documented and readable.
- Write unit tests for new features or bug fixes.
- Update the `README.md` if necessary.

## License

This project is licensed under the [MIT License](LICENSE).

---

**FlexiLog** aims to provide developers with a powerful yet simple logging solution tailored for high-performance C++ applications. By focusing on flexibility, efficiency, and ease of use, FlexiLog stands out as a reliable tool for debugging and monitoring your projects.

For any questions, issues, or contributions, feel free to open an issue or submit a pull request on [GitHub](https://github.com/yourusername/FlexiLog).

---