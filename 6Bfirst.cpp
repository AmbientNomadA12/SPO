//V56
//первое приложение отсылает 5 цифор второму, второе составляет всевозможные комбинации и высчитывает
//их среднее арифметическое число и отсылает первому
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

using namespace std;

int msgid;

const int N = 5;
const int key = 75;

struct msgdata
{
    long mtype;
    int A[N];
};

void shift(msgdata msg)
{
    int temp = 0;
    for(int i = 0; i < N-1; ++i)
    {
        temp = msg.A[i];
        msg.A[i] = msg.A[i+1];
        msg.A[i+1] = msg.A[i];
    }
}

double task(msgdata msg)
{
    double summa = 0;
    int number = 0;
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N - 1; ++j)
        {
            for(int k = 0; k < N - 2; ++k)
            {
                for(int m = 0; m < N - 3; ++m)
                {
                    for(int l = 0; l < N - 4; ++l)
                    {
                        summa+= msg.A[i]*10000 + msg.A[j]*1000 + msg.A[k]*100 + msg.A[m]*10 + msg.A[l];
                        ++number;
                    }
                }
            }
        }
        shift(msg);
    }
    return(summa/number);
}

int main()
{   
    
setlocale(LC_ALL, "Rus");
srand(time(NULL));

msgid = msgget(key, IPC_CREAT | 0666);
if(msgid < 0)
{
    cout << "Не удалось создать очередь\n";
    return(1);
}

msgdata message;

msgrcv(msgid,  &message, sizeof(msgdata), 0, 0);

double ar_mean = task(message);

cout << "Полученные числа\n";
for(int i = 0; i < N; ++i)
{
    cout << message.A[i] << " ";
}

cout << endl;

cout << "Среднее число: " << ar_mean << endl;

msgsnd(msgid, &ar_mean, sizeof(double), 0);

msgctl(msgid, IPC_RMID, 0);

return(0);

}