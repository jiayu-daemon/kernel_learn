并发和竟态广泛存在，中断屏蔽、原子操作、自旋锁和互斥体都是解决并发问题的机制。
中断屏蔽很少单独被使用，原子操作只能针对证书进行，因此自旋锁和互斥体应用最为广泛。

自旋锁会导致死循环，锁定期间不允许阻塞，因此要求锁定的临界区小。互斥体允许临界区阻塞
可以适用于临界区大的情况。


