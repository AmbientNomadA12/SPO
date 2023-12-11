// V45 - вычислить сколько раз встречается число К во всей матрице и в каждой строке матрицы

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <sys/wait.h>
#include <ctime>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <sys/msg.h>

#include <signal.h>

#define _GNU_SOURCE
#include <sched.h>

const int NUMSTUCK = 1000000;

char stack[NUMSTUCK];
int **A, *B, N, K;

using namespace std;

int **create_array(int N, int K)
{
    int **A = new int *[N];
    for (int i = 0; i < N; ++i)
    {
        A[i] = new int[K];

        for (int j = 0; j < K; j++)
            A[i][j] = rand() % 2;
    }

    return (A);
}

void print_array(int **A, int N, int K)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < K; ++j)
            cout << A[i][j] << " ";
        cout << endl;
    }
}

int find_K(int row)
{
    int count = 0;
    for (int i = 0; i < N; ++i)
    {
        if (A[row][i] == K)
            ++count;
    }
    return (count);
}

int func(void *param)
{
    int i = *(int *)param;
    B[i] = find_K(i);
    // return(0);
}

int main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));

    cout << "Введите размер N квадратной матрицы А:\n";
    cin >> N;

    if (N <= 0)
    {
        cout << "Невозможный размер матрицы!\n";
        return (1);
    }

    A = create_array(N, N);
    B = new int[N];

    print_array(A, N, N);

    cout << "Введите число K:\n";
    cin >> K;

    int *param = new int[N];
    // int param[N];
    for (int i = 0; i < N; ++i)
    {
        param[i] = i;
        clone(func, (void *)(stack + 1000 - 1), CLONE_VM | CLONE_VFORK, param + i);
    }

    int count = 0;
    for (int i = 0; i < N; ++i)
    {
        cout << "Число чисел " << K << " в строке " << i << " равно: " << B[i] << endl;
        count += B[i];
    }

    cout << "Всего число встречается " << count << " раза" << endl;

    for (int i = 0; i < N; i++)
    {
        delete[] A[i];
    }
    delete[] A;
    delete[] B;

    return (0);
}