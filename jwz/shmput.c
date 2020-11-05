#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(int argc, char const *argv[])
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
    printf("%s\n", men_ptr);         //打印内存的数据
    shmdt(men_ptr);                 //取消内存映射
    // shmctl(shm_id, IPC_RMID, NULL); //删除内存映射IPC对象

    return 0;
}
