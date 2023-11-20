//V3
//Вывести список процессов указанного пользователя"

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <sys/wait.h>

using namespace std;

void Execute_command()
{
    int status = 0;

    if(fork() == 0)
    {
        cout << getpid() << " - PID процесса\n";
        string command;
        cout << "Пожалуйста введите команду для терминала:\n";
        cin >> command;
        system(command.c_str());
        exit(0);
    }
    wait(&status);
}

void Execute_scenario()
{
    int status = 0;
    
    if(fork() == 0)
    {
        cout << getpid() << " - PID процесса\n";
        cout << "Данная команда запустит сценарий для нахождения ряда Фобиначчи вплоть до N члена и умердаеи его на число A.\n Введите N и A\n";
        string N, A;
        cin >> N >> A;
        string command_String = "~/Desktop/labs/4/laba4_Scenario.sh " + N + " " + A;
        system(command_String.c_str());
        exit(0);
    }
    wait(&status);
}

void Execute_task()
{
    int status = 0;
    
    if(fork() == 0)
    {
        cout << getpid() << " - PID процесса\n";
        cout << "Данная команда запустит выведет процессы указанного пользователя.\n Введите имя пользователя:\n";
        string user;
        cin >> user;
        string command_String = "~/Desktop/labs/4/laba4_Task.sh " + user;
        system(command_String.c_str());
        exit(0);
    }
    wait(&status);
}

int main()
{
    cout << getpid() << " - PID процесса\n";
    int choice_comm = 0;
    cout << "Выберите пункт в меню:\n";
    cout << "1: Выполнить команду\n";
    cout << "2: Запустить сценарий \n";
    cout << "3: Выполнить, действие указанное в варианте\n";
    cout << "4: Выход\n";

    do
    {
        cout << "\nВведите команду\n";
        cin >> choice_comm;

        switch (choice_comm)
        {
        case 1:
            Execute_command();
            break;

        case 2:
            Execute_scenario();
            break;

        case 3:
            Execute_task();
            break;

        case 4:
            cout << "Программа успешно завершена\n";
            return(0);
        }

    } while (true);
    
    return 0;
}



