#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct semidfo *_buf;
};

int main(int argc, char const *argv[])
{

    key_t ipc_key;
    int sem_id;
    union semun semarg;
    char *buff[256] = {0};

    ipc_key = ftok(".", 1); //建立IPC对象key
    if (ipc_key == -1)
    {
        perror("获取IPC对象key 失败");
        return -1;
    }
    sem_id = semget(ipc_key, 14, IPC_CREAT | 0644); //创建信号量集合，14:创建了14个信号量个数
    if (sem_id == -1)
    {
        perror("创建信号集失败");
        return -1;
    }

    semarg.val = 0;                     //信号集初值
    semctl(ipc_key, 0, SETALL, semarg); //设置第0 号信号量的值为semarg.val = 0;返回所有信号量集的值
    struct sembuf sbuf;
    while (1)
    {
        getchar();
        sbuf.sem_num = 0; //信号量元素序号（数组下标），操作第0个信号量
        sbuf.sem_op = -1; //减一p操作,这里如果填了1.就是加一v操作
        sbuf.sem_flg = 0; //按照正常的信号量的操作

        semop(sem_id, &sbuf, 1); // &sbuf结构体数组，1结构体数组个数
        printf("信号量减一，hello\n");
    }
    semctl(sem_id, 0, IPC_RMID); //删除整个信号量，其中0是随便写的，因为删除信号量的时候是忽略第二个参数的

    return 0;
}
