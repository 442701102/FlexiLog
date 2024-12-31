#include <iostream>
#include <string>
// main.cpp

// 通过定义 ENABLE_FLEXILOG 来启用日志功能
#define ENABLE_FLEXILOG

#include "flexilog.hpp"

int main()
{
    // 基本日志
    FlexiLog::log(FlexiLog::LogLevel::INFO, "Application started.");
    FlexiLog::log(FlexiLog::LogLevel::DEBUG, "Debugging value: {}", 42);
    FlexiLog::log(FlexiLog::LogLevel::ERROR, "An error occurred: {}", "Out of memory");

    // 设置详细信息
    FlexiLog::setDetails({
        FlexiLog::LogDetail::LineNumber,
        FlexiLog::LogDetail::FileName,
        FlexiLog::LogDetail::CPUUsage
    });
    FlexiLog::log(FlexiLog::LogLevel::INFO, "Detailed log entry.");

    // 打印限制：最多打印 5 次，每秒最多打印 10 条
    FlexiLog::setMaxPrintCount(5);
    FlexiLog::setPrintFrequency(10);

    for(int i = 0; i < 100; ++i) {
        FlexiLog::log(FlexiLog::LogLevel::DEBUG,
                      "This will be printed up to 5 times at 10 prints per second.");
    }

    // 自定义调试信息
    auto getCustomInfo = []() {
        return std::string("MyCustomInfo");
    };
    FlexiLog::addCustomDetail(getCustomInfo);

    FlexiLog::log(FlexiLog::LogLevel::INFO, "Log with custom info.");

    return 0;
}
