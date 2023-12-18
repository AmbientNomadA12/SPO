// В17 - Игра

#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include <fstream>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// /home/sergey/Desktop/labs/10/
#include </home/sergey/Desktop/labs/10/player.h>
#include </home/sergey/Desktop/labs/10/spell.h>

#define _GNU_SOURCE

#define PORT 1125

using namespace std;

void print_all_spells(Spell *Spells, int N)
{
    for (int i = 0; i < N; ++i)
    {
        cout << "НОМЕР ЗАКЛИНАНИЯ: " << i << endl;
        Spells[i].print_stats();
    }
}

int main()
{
    int sd;

    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = PORT;
    saddr.sin_addr.s_addr = INADDR_ANY;

    sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd < 0)
    {
        cout << "ОШИБКА! НЕВОЗМОЖНО СОЗДАТЬ СОКЕТ!\n";
        return (1);
    }

    if (connect(sd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        cout << "Ошибка! Невозможно установить соединение с сервером!\n";
        return (1);
    }

    Spell *all_spells;
    int total_number_of_spells;

    if (recv(sd, &total_number_of_spells, sizeof(total_number_of_spells), MSG_WAITALL) < 0)
    {
        cout << "Ошибка получения числа заклинаний!\n";
        return (1);
    }
    else
    {
        cout << "Число заклинаний " << total_number_of_spells << endl;

        all_spells = new Spell[total_number_of_spells];

        if (recv(sd, all_spells, sizeof(Spell) * total_number_of_spells, MSG_WAITALL) < 0)
        {
            cout << "Ошибка получения списка заклинаний!\n";
            return (1);
        }
        else
            cout << "Получили список заклинаний\n"
                 << endl;
    }

    cout << "Полученный список заклинаний:\n";
    print_all_spells(all_spells, total_number_of_spells);

    Player gamer_self;
    Player gamer_enemy;

    cout << "Выберите " << deck_sise << " заклинания из списка, указав их номер.\n";

    // Выбор заклинаний
    for (int i = 0; i < deck_sise; ++i)
    {
        int spell_choice;
        cin >> spell_choice;

        if (spell_choice < 0 || spell_choice > total_number_of_spells - 1)
        {
            cout << "Вы заглянули слишком глубоко в тайные знания...\n(такого заклинания нет)\n";
            return (1);
        }

        gamer_self.Deck[i] = all_spells[spell_choice];
    }

    // Отправляем данные игрока, тем самым сообщая готовность серверу
    if (send(sd, &gamer_self, sizeof(Player), 0) < 0)
    {
        cout << "Не удалось сообщить о готовности\n";
        return (1);
    }

    int match = 0;

    while (1)
    {
        int choice;

        system("clear");

        cout << "РАУНД " << ++match << endl;

        if (match > 1)
        {
            cout << "Приняли свой статус " << match << endl;
            recv(sd, &gamer_self, sizeof(Player), MSG_WAITALL);
        }

        // Получение данных противника
        if (recv(sd, &gamer_enemy, sizeof(Player), MSG_WAITALL) < 0)
        {
            cout << "Не удалось получить данные о противнике\n";
            cout << "(Техническое поражение)\n";
            gamer_self.dead = true;
        }

        // Данные на начало раунда
        gamer_self.print_stats_spells();

        if (gamer_self.dead == true)
        {
            // system("clear");
            cout << "Вы погибли\n";
            return (1);
        }

        if (gamer_enemy.dead == true)
        {
            cout << "Вы победили!\n";
            return (0);
        }

        cout << "\nХарактеристики противника\n";
        gamer_enemy.print_stats();

        cout << "\n\nВыберите заклинание:\n";

        cin >> choice;

        // Отправляем выбор заклинания серверу
        send(sd, &choice, sizeof(int), 0);
    }

    return (0);
}
