#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>
/* 
    实现利用信号量及共享内存实现以下效果：
	当我们进程A往共享内存写入数据的时候进程B才能够读取打印出来，不然进程B不能够读取共享内存的数据
 */
/* 写入监听 */

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct semidfo *_buf;
};

/* 往内存中写入数据 */
int write_shm()
{
    key_t ipc_key;
    int shm_id;
    char *men_ptr;

    ipc_key = ftok(".", 1); //获取ipcd对象key值
    if (ipc_key == -1)
    {
        perror("获取ipc对象key值失败");
        return -1;
    }

    shm_id = shmget(ipc_key, 4096, IPC_CREAT | 0644); //获取共享内存的ID
    if (shm_id == -1)
    {
        perror("获取共享内存ID失败");
        return -1;
    }

    men_ptr = shmat(shm_id, NULL, 0); //映射虚拟内存
    if (men_ptr == (void *)-1)
    {
        perror("内存映射失败");
        return -1;
    }
    scanf("%s", men_ptr); //往内存中输入数据
    shmdt(men_ptr);       //取消内存映射
    return 0;
}

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
        // getchar();        //监听这个动作，有这个动作就执行pv操作
        write_shm();             //写入操作
        sbuf.sem_num = 0;        //信号量元素序号（数组下标），操作第0个信号量
        sbuf.sem_op = 1;         //加一v操作,这里如果填了-1.就是减一p操作
        sbuf.sem_flg = 0;        //按照正常的信号量的操作
        semop(sem_id, &sbuf, 1); // &sbuf结构体数组，1结构体数组个数

        printf("信号量加一，hello\n");
    }
    semctl(sem_id, 0, IPC_RMID); //删除整个信号量，其中0是随便写的，因为删除信号量的时候是忽略第二个参数的

    return 0;
}
