#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <atomic>
#include <limits>

///////////////////////////////////////////////////////////////////////////////
// 如果在编译时不定义 ENABLE_FLEXILOG，所有日志功能将被禁用（变成空操作）。
///////////////////////////////////////////////////////////////////////////////
//

//#if 0
#ifndef ENABLE_FLEXILOG
namespace FlexiLog
{
    // 日志等级
    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    // 调试信息类型
    enum class LogDetail {
        LineNumber,
        FileName,
        FunctionName,
        Timestamp,
        CPUUsage,
        MemoryUsage,
        Custom
    };

    // 当禁用日志时，这些函数统统变为空操作或空实现
    inline void setDetails(const std::vector<LogDetail>&) {}
    inline void addCustomDetail(std::function<std::string()>) {}
    inline void setMaxPrintCount(int) {}
    inline void setPrintFrequency(int) {}
    inline void setPrintCondition(std::function<bool()>) {}

    template<typename... Args>
    inline void log(LogLevel, const std::string&, Args&&...) {}

} // namespace FlexiLog

#else  // ENABLE_FLEXILOG

namespace FlexiLog
{
    ///////////////////////////////////////////////////////////////////////////
    // 日志等级
    ///////////////////////////////////////////////////////////////////////////
    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    ///////////////////////////////////////////////////////////////////////////
    // 调试信息类型
    ///////////////////////////////////////////////////////////////////////////
    enum class LogDetail {
        LineNumber,
        FileName,
        FunctionName,
        Timestamp,
        CPUUsage,
        MemoryUsage,
        Custom
    };

    ///////////////////////////////////////////////////////////////////////////
    // 全局管理器，用于存储各种日志参数、调试信息配置等
    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        // 保存全局的调试信息选项
        inline std::vector<LogDetail> g_details;

        // 自定义信息函数列表
        inline std::vector<std::function<std::string()>> g_customDetailFuncs;

        // 用于限制打印总次数
        inline int g_maxPrintCount = std::numeric_limits<int>::max();

        // 用于限制打印频率（每秒）
        inline int g_maxPrintsPerSecond = std::numeric_limits<int>::max();

        // 统计日志消息的打印次数（针对相同文本）
        inline std::unordered_map<std::string, int> g_messagePrintCount;

        // 自定义打印条件（默认总是可打印）
        inline std::function<bool()> g_printCondition = [] { return true; };

        // 线程安全锁
        inline std::mutex g_mutex;

        // 频率控制相关变量
        inline std::chrono::steady_clock::time_point g_lastResetTime = std::chrono::steady_clock::now();
        inline int g_printCountThisSecond = 0;

        // 简易替换 "{}" 的格式化函数
        // 会按照出现顺序依次用 args 替换 "{}"
        inline void doFormat(std::string&) {} // 递归终止

        template<typename T, typename... Args>
        void doFormat(std::string& msg, T&& value, Args&&... args)
        {
            // 找到第一个 "{}"
            auto pos = msg.find("{}");
            if (pos != std::string::npos)
            {
                std::ostringstream oss;
                oss << std::forward<T>(value);  
                msg.replace(pos, 2, oss.str());
                doFormat(msg, std::forward<Args>(args)...);
            }
        }

        // 获取当前时间戳（示例：YYYY-MM-DD HH:MM:SS）
        inline std::string getTimestamp()
        {
            using std::chrono::system_clock;
            auto now = system_clock::now();
            std::time_t timeNow = system_clock::to_time_t(now);
            char buf[100];
#if defined(_MSC_VER)
            // Windows 下使用 localtime_s
            tm localTime;
            localtime_s(&localTime, &timeNow);
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &localTime);
#else
            // C++17 及更高，使用线程安全的 localtime_r
            tm localTime;
            localtime_r(&timeNow, &localTime);
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &localTime);
#endif
            return std::string(buf);
        }

        // 获取 CPU 使用率（示例函数，返回一个模拟值）
        inline std::string getCPUUsage()
        {
            // 这里仅做示例，不做实际 CPU 占用获取
            // 真正实现可根据平台不同而有所差异
            return "CPU: 15%";
        }

        // 获取内存使用率（示例函数，返回一个模拟值）
        inline std::string getMemoryUsage()
        {
            // 这里仅做示例，不做实际内存占用获取
            return "Mem: 42MB";
        }

        // 核心打印函数
        template<typename... Args>
        void logImpl(LogLevel level,
                     const std::string& rawMsg,
                     Args&&... args)
        {
            std::lock_guard<std::mutex> lock(g_mutex);

            // 先判断自定义条件是否允许打印
            if (!g_printCondition())
                return;

            // 频率控制：若当前时间与上次重置时间相差 >= 1 秒，则重置计数
            auto now = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastResetTime).count();
            if (diff >= 1)
            {
                g_printCountThisSecond = 0;
                g_lastResetTime = now;
            }

            // 如果当前秒的打印次数已达上限，则不再打印
            if (g_printCountThisSecond >= g_maxPrintsPerSecond)
                return;

            // 先复制一份 message，然后替换占位符
            std::string message = rawMsg;
            doFormat(message, std::forward<Args>(args)...);

            // 如果达到单条消息最大打印次数，则不再打印
            auto it = g_messagePrintCount.find(message);
            if (it == g_messagePrintCount.end())
                it = g_messagePrintCount.emplace(message, 0).first;

            if (it->second >= g_maxPrintCount)
                return;

            // 构建输出内容
            std::ostringstream oss;

            // [等级]
            switch (level)
            {
                case LogLevel::TRACE: oss << "[TRACE] "; break;
                case LogLevel::DEBUG: oss << "[DEBUG] "; break;
                case LogLevel::INFO:  oss << "[INFO ] "; break;
                case LogLevel::WARN:  oss << "[WARN ] "; break;
                case LogLevel::ERROR: oss << "[ERROR] "; break;
                case LogLevel::FATAL: oss << "[FATAL] "; break;
            }

            // 根据配置信息，附加额外的调试信息
            for (auto& d : g_details)
            {
                switch (d)
                {
                case LogDetail::Timestamp:
                    oss << "[" << getTimestamp() << "] ";
                    break;
                case LogDetail::FileName:
                    // 无法自动获取调用处文件名，通常要用宏去做
                    // 此处仅作示例，写个固定值
                    oss << "[File:Unknown] ";
                    break;
                case LogDetail::LineNumber:
                    // 同上，示例写个固定值
                    oss << "[Line:0] ";
                    break;
                case LogDetail::FunctionName:
                    // 同上
                    oss << "[Func:Unknown] ";
                    break;
                case LogDetail::CPUUsage:
                    oss << "[" << getCPUUsage() << "] ";
                    break;
                case LogDetail::MemoryUsage:
                    oss << "[" << getMemoryUsage() << "] ";
                    break;
                case LogDetail::Custom:
                    // 这里把所有自定义函数的结果都加上
                    for (auto& f : g_customDetailFuncs)
                    {
                        oss << "[" << f() << "] ";
                    }
                    break;
                }
            }

            // 最后打印实际消息
            oss << message;

            // 输出到控制台（或根据需求输出到文件/网络等）
            std::cout << oss.str() << std::endl;

            // 更新计数
            g_printCountThisSecond++;
            it->second++;
        }
    } // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    // 以下是对外提供的 API
    ///////////////////////////////////////////////////////////////////////////

    // 设置日志详细信息
    inline void setDetails(const std::vector<LogDetail>& details)
    {
        std::lock_guard<std::mutex> lock(detail::g_mutex);
        detail::g_details = details;
    }

    // 添加自定义调试信息
    inline void addCustomDetail(std::function<std::string()> customFunc)
    {
        std::lock_guard<std::mutex> lock(detail::g_mutex);
        detail::g_customDetailFuncs.push_back(customFunc);
    }

    // 设置最大打印次数（针对同一条消息）
    inline void setMaxPrintCount(int count)
    {
        std::lock_guard<std::mutex> lock(detail::g_mutex);
        detail::g_maxPrintCount = (count <= 0) ? 0 : count;
    }

    // 设置打印频率（每秒最多可打印多少条）
    inline void setPrintFrequency(int maxPerSecond)
    {
        std::lock_guard<std::mutex> lock(detail::g_mutex);
        detail::g_maxPrintsPerSecond = (maxPerSecond <= 0) ? 0 : maxPerSecond;
    }

    // 设置自定义打印条件
    // 只有在此函数返回 true 时才会打印日志
    inline void setPrintCondition(std::function<bool()> condition)
    {
        std::lock_guard<std::mutex> lock(detail::g_mutex);
        detail::g_printCondition = condition;
    }

    // 日志打印函数
    template<typename... Args>
    void log(LogLevel level, const std::string& message, Args&&... args)
    {
        detail::logImpl(level, message, std::forward<Args>(args)...);
    }

} // namespace FlexiLog

#endif // ENABLE_FLEXILOG
