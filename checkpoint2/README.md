# checkpoint-2
该项目的第二部分是在第一部分的基础上继续完善Web服务器。

第二部分主要是完成两个功能：记录日志和HTTP响应。

## 记录日志
该部分比较简单，只要简单地将一些服务器的信息添加到指定日志文件即可，
也不需要完成很复杂的功能，有了日志功能也方便调试。

由于要在启动服务器时将日志文件传给main函数，所以我将日志文件名声明为
全局变量，方便在整个项目中往日志文件中添加日志信息。

日志文件的操作放在`src/log`文件夹中。

## HTTP响应
要处理HTTP请求，那必须能解析HTTP请求。

### HTTP解析
完成一个HTTP解析器并不是一件简单的事，感觉难度已经超过了本次项目，
所以我用开源工具`flex`和`bison`来完成对HTTP请求的词法分析器和语法分析器，
所以要先学习使用这两个开源工具。[入门](https://liu-jianhao.github.io/2018/09/flex--bsion/)。

该部分放在`src/parse`文件夹中。

### GET、HEAD、POST
我只完成这三个最常用的请求方法，主要是了解在各种情况下要对请求响应
正确的格式。

该部分放在`src/response`文件夹中。

## 其它
与项目第一部分不同的还有在`src/io`中添加了一些文件操作的函数。

## 测试
根据课程提供的测试脚本，通过了所有的测试。
```shell
test_using_select (__main__.project1cp2tester) ... Simple checker to tell if you are using select().         We will check it manually later.
ok
start_server (__main__.project1cp2tester) ... Try to start server!
./lisod 7471 9722 ./lisod.log ../tmp/lisod.lock ../tmp/www ../tmp/cgi/cgi_script.py ../tmp/grader.key ../tmp/grader.crt
Wait 2 seconds.
Server is running
ok
test_HEAD_headers (__main__.project1cp2tester) ... ----- Testing Headers -----
ok
test_HEAD (__main__.project1cp2tester) ... ----- Testing HEAD -----
ok
test_GET (__main__.project1cp2tester) ... ----- Testing GET -----
ok
test_POST (__main__.project1cp2tester) ... ----- Testing POST -----
ok
test_bad (__main__.project1cp2tester) ... ----- Testing Bad Requests-----
ok
test_big (__main__.project1cp2tester) ... ----- Testing Big file -----
ok
test_kill (__main__.project1cp2tester) ... kill it
ok

----------------------------------------------------------------------
Ran 11 tests in 19.567s

OK
{"scores": {"test_make": 1, "test_HEAD_headers": 1, "test_POST": 1, "server_start": 1, "test_git": 1, "use_select": 1, "test_big": 1, "test_bad": 1, "test_GET": 1, "test_HEAD": 1}}
```

## 效果
我们可以实际看看加上静态html有什么效果。

先启动服务器：
```shell
./lisod 7471 9722 ./lisod.log ../tmp/lisod.lock ./src/static_site/ ../tmp/cgi/cgi_script.py ../tmp/grader.key ../tmp/grader.crt
```
在浏览器输入；
```shell
localhost:7471
```
就可以看到下面的画面：
![](https://img-blog.csdnimg.cn/20181104095801666.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlc3Ricm9va2xpdQ==,size_16,color_FFFFFF,t_70)
