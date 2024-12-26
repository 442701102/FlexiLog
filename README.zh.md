

# FlexiLog

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-17%2B-blue.svg)
![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)

**FlexiLog** 是一个轻量级、高性能的 C++ 日志库，旨在促进高效的调试和监控。与其他流行的日志库不同，FlexiLog 提供了编译时日志控制，零开销禁用，丰富的枚举选项以获取详细的调试信息，可自定义的打印限制，并且独立运行，无需任何外部依赖。

## 目录

- [功能](#功能)
- [安装](#安装)
  - [前置条件](#前置条件)
  - [使用 CMake](#使用-cmake)
  - [手动安装](#手动安装)
- [使用方法](#使用方法)
  - [基本日志记录](#基本日志记录)
  - [基于模板的日志控制](#基于模板的日志控制)
  - [丰富的调试信息](#丰富的调试信息)
  - [打印限制](#打印限制)
  - [自定义调试信息](#自定义调试信息)
- [API 参考](#api-参考)
- [示例](#示例)
  - [示例 1：基本日志记录](#示例-1基本日志记录)
  - [示例 2：启用详细的调试信息](#示例-2启用详细的调试信息)
  - [示例 3：限制日志打印](#示例-3限制日志打印)
  - [示例 4：添加自定义调试信息](#示例-4添加自定义调试信息)
- [贡献](#贡献)
  - [如何贡献](#如何贡献)
  - [编码标准](#编码标准)
- [许可](#许可)

## 功能

- **基于模板的日志控制**：通过编译时启用或禁用日志，实现零运行时开销。
- **丰富的枚举选项**：可选择多种调试信息，如行号、文件名、CPU 占用率等。
- **可自定义的打印限制**：限制日志打印次数、控制打印频率，并设置复杂条件以有效管理日志输出。
- **用户定义的调试信息**：通过自定义扩展调试细节，满足项目的特定需求。
- **无外部依赖**：纯 C++ 实现，确保轻松集成到任何项目中，无需额外的库依赖。
- **高性能**：优化设计，确保在禁用日志时对应用性能影响最小。

## 安装

### 前置条件

- **C++17** 或更高版本
- 兼容主要编译器，如 GCC、Clang 和 MSVC

### 使用 CMake

1. **克隆仓库**

   ```bash
   git clone https://github.com/yourusername/FlexiLog.git
   cd FlexiLog
   ```

2. **构建并安装**

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   sudo make install
   ```

### 手动安装

1. **下载源文件**

   从仓库下载 `FlexiLog.hpp` 头文件。

2. **包含到你的项目中**

   将 `FlexiLog.hpp` 文件放入你的项目的包含目录，并在源文件中引入：

   ```cpp
   #include "FlexiLog.hpp"
   ```

## 使用方法

### 基本日志记录

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::log(FlexiLog::LogLevel::INFO, "Application started.");
    FlexiLog::log(FlexiLog::LogLevel::DEBUG, "Debugging value: {}", 42);
    FlexiLog::log(FlexiLog::LogLevel::ERROR, "An error occurred: {}", "Out of memory");
    return 0;
}
```

### 基于模板的日志控制

通过定义或取消定义 `ENABLE_FLEXILOG` 宏，在编译时启用或禁用日志记录。

```cpp
// 启用日志记录
#define ENABLE_FLEXILOG
#include "FlexiLog.hpp"

// 禁用日志记录
// #define ENABLE_FLEXILOG
#include "FlexiLog.hpp"
```

当日志记录被禁用时，所有 `FlexiLog::log` 调用将在编译时被移除，确保没有性能开销。

### 丰富的调试信息

使用枚举选择要包含在日志中的调试信息。

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

### 打印限制

控制日志消息的打印次数和频率。

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::setMaxPrintCount(5); // 最多打印 5 次
    FlexiLog::setPrintFrequency(10); // 每秒最多打印 10 次

    for(int i = 0; i < 100; ++i) {
        FlexiLog::log(FlexiLog::LogLevel::DEBUG, "This will be printed up to 5 times at 10 prints per second.");
    }
    return 0;
}
```

### 自定义调试信息

通过注册自定义函数，添加自己的调试信息。

```cpp
#include "FlexiLog.hpp"

std::string getCustomInfo() {
    // 实现自定义信息的获取
    return "CustomInfo";
}

int main() {
    FlexiLog::addCustomDetail(getCustomInfo);

    FlexiLog::log(FlexiLog::LogLevel::INFO, "Log with custom info.");
    return 0;
}
```

## API 参考

### `FlexiLog::LogLevel`

枚举日志的严重级别。

- `TRACE`
- `DEBUG`
- `INFO`
- `WARN`
- `ERROR`
- `FATAL`

### `FlexiLog::LogDetail`

枚举可用的调试信息类型。

- `LineNumber`
- `FileName`
- `FunctionName`
- `Timestamp`
- `CPUUsage`
- `MemoryUsage`
- `Custom`

### `FlexiLog::log`

使用指定的日志级别记录一条消息。

```cpp
template<typename... Args>
static void log(LogLevel level, const std::string& message, Args&&... args);
```

### `FlexiLog::setDetails`

设置每条日志条目中包含的详细信息。

```cpp
static void setDetails(const std::vector<LogDetail>& details);
```

### `FlexiLog::addCustomDetail`

注册一个自定义函数，以获取额外的调试信息。

```cpp
static void addCustomDetail(std::function<std::string()> customFunc);
```

### `FlexiLog::setMaxPrintCount`

设置特定日志消息的最大打印次数。

```cpp
static void setMaxPrintCount(int count);
```

### `FlexiLog::setPrintFrequency`

设置每秒允许的最大日志打印次数。

```cpp
static void setPrintFrequency(int maxPerSecond);
```

### `FlexiLog::setPrintCondition`

设置一个自定义条件，只有满足该条件时日志才会被打印。

```cpp
static void setPrintCondition(std::function<bool()> condition);
```

## 示例

### 示例 1：基本日志记录

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::log(FlexiLog::LogLevel::INFO, "Starting the application...");
    FlexiLog::log(FlexiLog::LogLevel::DEBUG, "Variable x = {}", 10);
    FlexiLog::log(FlexiLog::LogLevel::ERROR, "Failed to load resource: {}", "config.yaml");
    return 0;
}
```

### 示例 2：启用详细的调试信息

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

### 示例 3：限制日志打印

```cpp
#include "FlexiLog.hpp"

int main() {
    FlexiLog::setMaxPrintCount(3);
    FlexiLog::setPrintFrequency(2); // 每秒 2 次打印

    for(int i = 0; i < 10; ++i) {
        FlexiLog::log(FlexiLog::LogLevel::DEBUG, "This debug message will be limited.");
    }

    return 0;
}
```

### 示例 4：添加自定义调试信息

```cpp
#include "FlexiLog.hpp"

std::string getUserID() {
    // 假设我们从某处获取用户ID
    return "User123";
}

int main() {
    FlexiLog::addCustomDetail(getUserID);

    FlexiLog::log(FlexiLog::LogLevel::INFO, "User action logged.");
    return 0;
}
```

## 贡献

欢迎贡献！无论是报告错误、建议功能还是提交拉取请求，你的贡献都能帮助 FlexiLog 变得更好。

### 如何贡献

1. **Fork 仓库**

   点击仓库页面右上角的 "Fork" 按钮。

2. **克隆你的 Fork**

   ```bash
   git clone https://github.com/yourusername/FlexiLog.git
   cd FlexiLog
   ```

3. **创建新分支**

   ```bash
   git checkout -b feature/YourFeatureName
   ```

4. **进行更改**

   实现你的功能或修复错误。

5. **提交更改**

   ```bash
   git commit -m "Add feature: YourFeatureName"
   ```

6. **推送到你的 Fork**

   ```bash
   git push origin feature/YourFeatureName
   ```

7. **创建拉取请求**

   前往原始仓库，并从你的 Fork 创建一个拉取请求。

### 编码标准

- 遵循 C++ 最佳实践和编码标准。
- 确保代码有良好的文档和可读性。
- 为新功能或错误修复编写单元测试。
- 如有必要，更新 `README.md`。

## 许可

本项目采用 [MIT 许可](LICENSE)。

---

**FlexiLog** 旨在为开发者提供一个强大而简洁的日志解决方案，专为高性能的 C++ 应用程序设计。通过专注于灵活性、效率和易用性，FlexiLog 成为调试和监控项目的可靠工具。

如有任何问题、问题报告或贡献意向，请随时在 [GitHub](https://github.com/yourusername/FlexiLog) 上打开问题或提交拉取请求。

---