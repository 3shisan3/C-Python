鉴于Python默认会锁住GIL在当运行C++或者C代码的时候
导致当前执行的线程持有GIL，其他线程得不到执行机会的情况出现无法解决
额外进行一层代码封装