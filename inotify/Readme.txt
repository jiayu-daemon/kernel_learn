描述

Inotify API用于检测文件系统变化的机制。Inotify可用于检测单个文件，也可以检测整个目录。当检测的对象是一个目录的时候，目录本身和目录里的内容都会成为检测的对象。

此种机制的出现的目的是当内核空间发生某种事件之后，可以立即通知到用户空间。方便用户做出具体的操作。

Inotify API

inotify_init(void)
用于创建一个inotify的实例，然后返回inotify事件队列的文件描述符。 同样内核也提供了inotify_init1(int flags)接口函数，当flag等于0的时候，该函数等价于inotify_init(void)函数。

inotify_add_watch(int fd, const char* pathname, uint32_t  mask)
该函数用于添加“watch list”，也就是检测列表。 可以是一个新的watch，也可以是一个已经存在的watch。其中fd就是inotify_init的返回值，pathname是要检测目录或者文件的路径，mask就是要检测的事件类型。该函数成功返回的是一个unique的watch描述符。

inotify_rm_watch(int fd, int wd)
用于从watch list种移除检测的对象。

数据结构

内核使用struct inotify_event代表一个文件事件。当检测的文件对象发生变化时，使用read系统调用就会返回一个或者多个inotify_event的文件事件对象。

struct inotify_event {
   int      wd;       /* Watch descriptor */
   uint32_t mask;     /* Mask of events */
   uint32_t cookie;   /* Unique cookie associating related
						 events (for rename(2)) */
   uint32_t len;      /* Size of name field */
   char     name[];   /* Optional null-terminated name */
};
.wd:        就是检测的对象的watch descriptor

.mask:    检测事件的mask

.cookie:  和rename事件相关。

.len:        name字段的长度。

.name:    检测对象的name。

可以看到name字段的长度是0，也就是变长的。因为检测的对象的name不定，使用变长可以方便记录检测对象的name。

 

有关检测的事件类型分为好几种，如下：

IN_ACCESS         File was accessed (read) (*).
IN_ATTRIB         Metadata  changed,  e.g.,  permissions, timestamps, extended
				 attributes, link count (since Linux 2.6.25), UID, GID,  etc.(*).
IN_CLOSE_WRITE    File opened for writing was closed (*).
IN_CLOSE_NOWRITE  File not opened for writing was closed (*).
IN_CREATE         File/directory created in watched directory (*).
IN_DELETE         File/directory deleted from watched directory (*).
IN_DELETE_SELF    Watched file/directory was itself deleted.
IN_MODIFY         File was modified (*).
IN_MOVE_SELF      Watched file/directory was itself moved.
IN_MOVED_FROM     File moved out of watched directory (*).
IN_MOVED_TO       File moved into watched directory (*).
IN_OPEN           File was opened (*)




程序测试举例


1.  编译代码
gcc inotify.c -o inotify

2. 在tmp目录下创建test目录
mkdir /tmp/test

3.  检测/tmp/test目录，使用inotify机制
./inotify /tmp/test &

4.  在/tmp/test下创建1.txt文件
test$ touch /tmp/test/1.txt
create file: 1.txt