## FpsServer
---
一个Linux下C++实现的FPS游戏服务器，造轮子学习一下

- 使用epoll进行TCP并发处理（ET模式）
- 半连接半反应堆线程池,信号量+自旋锁保证线程同步（旧版本使用互斥锁）
- 使用libconfig对服务器运行参数高度配置
- 异步日志输出模块，记录服务器运行状态，支持日志信息级别筛选

### 编译环境
- Ubuntu 16.04, CentOS7
- c++11

### 构建工具
- cmake 3.17.1
- ninja 1.5.1

### 库依赖
- libconfig
- uuid

### 编译
首先确保libconfig全局安装

头文件在`usr/local/include`

库文件在`usr/local/lib`

uuid可使用apt安装`sudo apt-get install uuid-dev`

之后新建一个构建目录，并进入
```
mkdir cmake-build-debug
cd cmake-build-debug
```
之后在构建目录执行

```
cmake -G Ninja ..
ninja
```