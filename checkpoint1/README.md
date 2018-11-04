# checkpoint1
项目一的第一个检查点是要用select进行IO复用，实现一个echo服务器。

## 各个文件的作用分析
### liso
实现echo服务器的总体框架

### client_pool
实现一个客户池，本质就是select要监听的客户数组，然后对客户数组进行操作，封装成一个结构体。

### io
将send封装成包裹函数

### utilities
一些打印日志、设置文件套接字属性的函数

## 测试
在其中一个终端运行服务器
```shell
$ ./lisod 9999
```
在另外一个终端运行测试程序
```shell
$ ./cp1_checker.py 127.0.0.1 9999 1000 10 2048 500
Success!
```