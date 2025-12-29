# High-Performance C++ Web Server

![Platform](https://img.shields.io/badge/Platform-Linux-green.svg)
![Language](https://img.shields.io/badge/Language-C%2B%2B11-blue.svg)
![License](https://img.shields.io/badge/License-MIT-orange.svg)
![Build](https://img.shields.io/badge/Build-CMake-brightgreen.svg)

## 📖 项目简介

这是一个基于 **C++11** 开发的轻量级、高性能 Web 服务器，运行于 Linux 环境。

底层网络库采用了 **One Loop Per Thread** + **Non-blocking I/O** 的 **Reactor** 多线程模型，应用层实现了 **HTTP 1.1** 的状态机解析。项目旨在在高并发场景下提供稳定的连接处理能力，经测试可应对万级并发连接。

该项目适合作为 C++ 后端开发的学习参考，涵盖了 Socket 编程、Epoll 事件驱动、线程池、内存管理等核心技术点。

## ✨ 核心特性

*   **并发模型：** 采用 `Multi-Reactor` 多线程架构（主从 Reactor），主线程仅负责 `accept` 连接，子线程负责处理 IO 读写，彻底解耦连接与业务。
*   **IO 多路复用：** 封装 Linux **Epoll**，使用 **Edge Trigger (ET)** 边缘触发模式，配合非阻塞 IO (Non-blocking IO)，大幅减少系统调用次数。
*   **零拷贝思想：** 核心缓冲区 `Buffer` 设计及 HTTP 解析过程优化了内存拷贝，提升数据吞吐量。
*   **HTTP 解析：** 内部实现高效的 **有限状态机 (FSM)**，支持 HTTP/1.1 GET/POST 请求，支持长连接 (Keep-Alive)。
*   **线程通信：** 使用 `eventfd` 实现线程间的异步唤醒，比管道 (pipe) 更高效。
*   **内存管理：** 遵循 **RAII** 惯例，利用 `std::shared_ptr` 和 `std::unique_ptr` 管理资源，通过 `std::weak_ptr` 解决多线程下的对象生命周期竞态问题。
*   **性能优化：** 利用 `__thread` 关键字实现线程局部存储 (TLS) 缓存线程 ID，减少系统调用开销。

## 🏗️ 系统架构

采用 **Reactor** 模式，架构如下：

```text
+------------------+
|   Main Reactor   |  <--- accept() New Connections
|    (Acceptor)    |
+---------+--------+
          | Dispatch (Round Robin)
          v
+------------------+    +------------------+    +------------------+
|   Sub Reactor 1  |    |   Sub Reactor 2  |    |   Sub Reactor 3  | ...
|  (EventLoop 1)   |    |  (EventLoop 2)   |    |  (EventLoop 3)   |
+---------+--------+    +---------+--------+    +---------+--------+
          |                      |                       |
    +-----+-----+          +-----+-----+           +-----+-----+
    | TcpConn A |          | TcpConn C |           | TcpConn E |
    +-----------+          +-----------+           +-----------+
    | TcpConn B |          | TcpConn D |           ...
    +-----------+          +-----------+
