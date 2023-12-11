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


int main()
{   
    
setlocale(LC_ALL, "Rus");
srand(time(NULL));

msgid = msgget(key, IPC_EXCL);
if(msgid < 0)
{
    cout << "Не удалось создать очередь\n";
    return(1);
}

msgdata message;
message.mtype = 1.0;

cout << "Числа: \n";
for(int i = 0; i < N; ++i)
{
    message.A[i] = rand()%10;
    cout << message.A[i] << " ";
}

cout << endl;

msgsnd(msgid, &message, sizeof(msgdata), 0);
double ar_mean = 0;
msgrcv(msgid, &ar_mean, sizeof(double), 0, 0);

cout << "Среднее арифметическое их всевозможных комбинаций: " << ar_mean << endl;

msgctl(msgid, IPC_RMID, NULL);

return(0);

}