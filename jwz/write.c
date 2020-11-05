#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
/* 
3	编写两个程序去进行有名管道通信，一个负责读，一个负责写，当输入或者接受到exit，则退出程序，
	这两个文件必须先判断有名管道文件是否存在，如果存在同名的其他类型文件则将其删除，然后创建出一个管道文件出来通信
 */
int main(int argc, char const *argv[])
{
    int of;
    size_t wf;
    char buff[256] = {0};

    /* 创建非匿名管道文件 */
    if (access("test", F_OK) == -1) //不存在
    {
        mkfifo("test", 0766); //创建有名管道文件
    }

    /* 打开管道文件 */
    of = open("test", O_RDWR);
    if (of == -1)
    {
        perror("文件打开失败");
        return -1;
    }

    /* 写入操作 */
    while (1)
    {
        bzero(buff, sizeof(buff));
        scanf("%s", buff);
        if (strcmp(buff, "exit") == 0)
        {
            break;
        }

        wf = write(of, buff, strlen(buff));
        if (wf == -1)
        {
            perror("数据读入失败");
        }
    }
    close(of);
    return 0;
}
