// V45 - вывести все двухзначные числа, которые можно составить комбинацией цифр
// из заданного множества

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/msg.h>

#include <signal.h>

#define _GNU_SOURCE
#include <sched.h>

const int NUMSTUCK = 1000000;
int N, *A;
char stack[NUMSTUCK];

using namespace std;

int *create_fill_array(int N)
{
    int *A = new int[N];

    for (int i = 0; i < N; ++i)
    {
        A[i] = rand() % 9 + 1;
    }
    return (A);
}

void print_array(int *A, int N)
{
    for (int i = 0; i < N; ++i)
        cout << A[i] << " ";
    cout << endl;
}

int func(void *param)
{
    int p = *(int *)param;
    int buff = A[0];
    A[0] = A[p];
    A[p] = buff;
    for (int i = 0; i < N; ++i)
    {
        cout << A[0] << A[i] << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));

    cout << "Введите число чисел произвольного множества\n";
    cin >> N;

    if (N <= 0)
    {
        cout << "Ошибка! Неправильно введено число";
        return (1);
    }

    A = create_fill_array(N);
    cout << "Числа произвольного множества: \n";
    print_array(A, N);

    // int param[1];
    int *param = new int[N];
    for (int i = 0; i < N; ++i)
    {
        param[i] = i;
        clone(func, (void *)(stack + 1000 - 1), CLONE_VM | CLONE_VFORK, param + i);
    }

    cout << "Числа могут повторяться, так как в задании не указано \"без повторений\"\n";

    delete[] A;
    delete[] param;

    return (0);
}