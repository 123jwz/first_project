#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    key_t ipc_key;
    int msg_id;

    struct msgbuf
    {
        long mtype;        //消息类型
        char mtmext[1024]; //消息数据
    };
    struct msgbuf msg;

    ipc_key = ftok(".", 1); //IPC对象key值
    if (ipc_key == -1)
    {
        perror("获取IPC的key值失败");
        return -1;
    }

    msg_id = msgget(ipc_key, IPC_CREAT | 0644); //创建IPC对象队列
    if (msg_id == -1)
    {
        perror("创建或者获取消息队列失败");
        return -1;
    }

    msg.mtype = atoi(argv[2]); //获取消息队列的类型
    pid = fork();
    if (pid == 0)
    {
        ssize_t rcv_size;
        while (1)
        {
            bzero(&msg, sizeof(msg));

            rcv_size = msgrcv(msg_id, &msg, sizeof(msg.mtmext), atoi(argv[1]), 0);
            if (rcv_size == -1)
            {
                kill(getppid(), SIGQUIT);
            }

            printf("我是500，我跟250聊天\n");
            printf("rcv_text=%s\n", msg.mtmext);
        }

        exit(0);
    }

    while (1)
    {
        scanf("%s", msg.mtmext); //获取输入的信息
        if (strcmp("loginout", msg.mtmext) == 0)
        {
            break;
        }

        msgsnd(msg_id, &msg, strlen(msg.mtmext), 0); //把消息发送到队列中
    }

    msgctl(msg_id, IPC_RMID, NULL); //删除IPC信息对象对列
    // exit(0);
    return 0;
}
