/*
    int shmget(key_t key, size_t size, int shmflg);
        - 功能：创建一个新的共享内存段，或者获取一个既有的共享内存段的标识。
            新创建的内存段中的数据都会被初始化为0
        - 参数：
            - key : key_t类型是一个整形，通过这个找到或者创建一个共享内存。
                    一般使用16进制表示，非0值
            - size: 共享内存的大小
            - shmflg: 属性
                - 访问权限
                - 附加属性：创建/判断共享内存是不是存在
                    - 创建：IPC_CREAT
                    - 判断共享内存是否存在： IPC_EXCL , 需要和IPC_CREAT一起使用
                        IPC_CREAT | IPC_EXCL | 0664
            - 返回值：
                失败：-1 并设置错误号
                成功：>0 返回共享内存的引用的ID，后面操作共享内存都是通过这个值。


    void *shmat(int shmid, const void *shmaddr, int shmflg);
        - 功能：和当前的进程进行关联
        - 参数：
            - shmid : 共享内存的标识（ID）,由shmget返回值获取
            - shmaddr: 申请的共享内存的起始地址，指定NULL，内核指定
            - shmflg : 对共享内存的操作
                - 读 ： SHM_RDONLY, 必须要有读权限
                - 读写： 0
        - 返回值：
            成功：返回共享内存的首（起始）地址。  失败(void *) -1


    int shmdt(const void *shmaddr);
        - 功能：解除当前进程和共享内存的关联
        - 参数：
            shmaddr：共享内存的首地址
        - 返回值：成功 0， 失败 -1

    int shmctl(int shmid, int cmd, struct shmid_ds *buf);
        - 功能：对共享内存进行操作。删除共享内存，共享内存要删除才会消失，创建共享内存的进行被销毁了对共享内存是没有任何影响。
        - 参数：
            - shmid: 共享内存的ID
            - cmd : 要做的操作
                - IPC_STAT : 获取共享内存的当前的状态
                - IPC_SET : 设置共享内存的状态
                - IPC_RMID: 标记共享内存被销毁
            - buf：需要设置或者获取的共享内存的属性信息
                - IPC_STAT : buf存储数据
                - IPC_SET : buf中需要初始化数据，设置到内核中
                - IPC_RMID : 没有用，NULL

    key_t ftok(const char *pathname, int proj_id);
        - 功能：根据指定的路径名，和int值，生成一个共享内存的key
        - 参数：
            - pathname:指定一个存在的路径
                /home/nowcoder/Linux/a.txt
                /
            - proj_id: int类型的值，但是这系统调用只会使用其中的1个字节
                    范围 ： 0-255  一般指定一个字符 'a'
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
    // 1.创建一个共享内存
    int shmid = shmget(100, 4096, IPC_CREAT | 0664);
    printf("shmid : %d\n", shmid);

    // 2.和当前进程进行关联
    void *ptr = shmat(shmid, NULL, 0);

    char *str = "helloworld";

    // 3.写数据
    memcpy(ptr, str, strlen(str) + 1);

    printf("按任意键继续\n");
    getchar();

    // 4.解除关联
    shmdt(ptr);

    // 5.删除共享内存
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}