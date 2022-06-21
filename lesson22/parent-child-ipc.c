/*
    实现 ps aux | grep xxx 父子进程间通信

    子进程： ps aux, 子进程结束后，将数据发送给父进程
    父进程：获取到数据，过滤
    pipe()
    execlp()
    子进程将标准输出 stdout_fileno 重定向到管道的写端。  dup2
*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
int main()
{
    //创建一个管道
    int pipfd[2];
    int ret = pipe(pipfd);
    if (ret == -1)
    {
        perror("pipe");
        exit(0);
    }

    //创建子进程
    pid_t pid = fork();
    if (pid > 0)
    {
        //父进程

        //关闭写端
        close(pipfd[1]);

        //从管道中读取
        char buf[1024] = {0};

        while (read(pipfd[0], buf, sizeof(buf) - 1) > 0)
        {
            //过滤数据输出
            printf("%s", buf);
            memset(buf, 0, sizeof(buf));
        }
        wait(NULL);
    }
    else if (pid == 0)
    {
        //子进程

        //关闭读端
        close(pipfd[0]);

        //写管道
        //文件描述符的重定向 stdout_fileno -> pipfd[1]
        dup2(pipfd[1], STDOUT_FILENO);

        //执行 ps aux
        execlp("ps", "ps", "aux", NULL);

        perror("execlp");
        exit(0);
    }
    else
    {
        perror("fork");
        exit(0);
    }

    return 0;
}