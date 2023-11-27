//V28
//Найти максимальный элемент в матрице А. Размерность NxK. Число процессов либо N либо K

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <sys/wait.h>
#include <ctime>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

using namespace std;

int shmid; //для хранения дескриптора изменяемой памяти
int semid; //для хранения дескриптора группы семафоров

sembuf Plus1 = {0 , 1, 0};
sembuf Minus1 = {0, -1, 0};

int** create_Array(int N, int K)
{
    int **A = new int*[N];
    for(int i = 0; i < N; ++i)
    {
        A[i] = new int[K];

        for(int j = 0; j < K; j++)
            A[i][j] = rand()%100;
    }

    return(A);

}

void print_Array(int** A, int N, int K)
{
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < K; ++j)
            cout << A[i][j] << " ";
        cout << endl;
    }
}

int find_Max(int **A, int N, int K)
{
    int Max = 0;
    for(int i = 0; i < K; ++i)
    {
        if(A[N][i] > Max)
            Max = A[N][i];
    }
    return(Max);
}

int main()
{

setlocale(LC_ALL, "Rus");
srand(time(NULL));

int N, K; //N - строки, K - столбцы 

cout << "Введите размерность матрицы: N и K\n";
cin >> N >> K;

if(N <= 0 || K <= 0)
{
    cout << "Ошибка! Отрицательные или нулевые размеры матрицы\n";
    return(1);
}

int **A = create_Array(N, K);

print_Array(A, N, K);

int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
if(semid < 0)
{
    cout << "Не удалось создать семафор\n";
    return(1);
}

int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
if(shmid < 0)
{
    cout << "Не удалось создать разделенную память\n";
    return(1);
}

int* shared_memory = (int*)shmat(shmid, NULL, 0);

if(shared_memory == (int*) -1)
{
    cout << "Не удалось присоединить разделенную память\n";
    return(1);
}

*shared_memory = 0;

semop(semid, &Plus1, 1);

for(int i = 0; i < N; ++i)
{
    if(fork() == 0)
    {
        semop(semid, &Minus1, 1);
        int Max = find_Max(A, i, K);
        if(Max > *shared_memory)
            *shared_memory = Max;
        semop(semid, &Plus1, 1);
        //system("ipcs -s");
        exit(0);
    }
    
}

for(int i = 0; i < N; ++i)
{
    wait(NULL);
}

cout << "Максимальное число в массиве А это " << *shared_memory << endl;

//Удаление всего
shmdt(shared_memory);
semctl(shmid, IPC_RMID, NULL);
semctl(semid, 0, IPC_RMID);

for(int i = 0; i < N; i++)
{
    delete [] A[i];
}
delete [] A;

return(0);
}
