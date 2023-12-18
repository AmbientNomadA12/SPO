// В17 - Игра

#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include <fstream>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include </home/sergey/Desktop/labs/10/player.h>
#include </home/sergey/Desktop/labs/10/spell.h>

#define _GNU_SOURCE

#define PORT 1125

int default_mana_regen = 2;

Spell *load_spell_list(int &N)
{
    int st;
    ifstream spells("spells.txt");

    if (!spells.is_open())
    {
        cout << "Ошибка загрузки заклинаний\n";
        exit(st);
    }

    spells >> N;

    Spell *Spell_list = new Spell[N];

    for (int i = 0; i < N; ++i)
    {
        spells >> Spell_list[i].name;
        spells >> Spell_list[i].mana_cost;
        spells >> Spell_list[i].type;

        if (Spell_list[i].type == 0)
        {
            spells >> Spell_list[i].damage;
            spells >> Spell_list[i].penetration;
        }
        else if (Spell_list[i].type == 1)
        {
            spells >> Spell_list[i].health_buff;
            spells >> Spell_list[i].mana_buff;
            spells >> Spell_list[i].defence_buff;
        }
    }

    spells.close();

    return (Spell_list);
}

void print_all_spells(Spell *Spells, int N)
{
    for (int i = 0; i < N; ++i)
    {
        Spells[i].print_stats();
    }
}

int send_spells(int player, int N, Spell *all_sp)
{
    if (send(player, &N, sizeof(N), 0) < 0)
    {
        cout << "Отправка игроку ID:" << player << " не удалась\n";
        return (1);
    }
    else
    {
        if (send(player, all_sp, sizeof(Spell) * N, 0) > 0)
            cout << "Отправка игроку ID:" << player << " удалась\n";
        else
        {
            cout << "Отправка игроку ID:" << player << " не удалась\n";
            return (1);
        }
    }

    return (0);
}

int main()
{
    setlocale(LC_ALL, "Russian");

    int total_number_of_spells;
    Spell *all_spells = load_spell_list(total_number_of_spells);

    print_all_spells(all_spells, total_number_of_spells);

    int sd;
    struct sockaddr_in saddr, caddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd < 0)
    {
        cout << "ОШИБКА! НЕВОЗМОЖНО СОЗДАТЬ СОКЕТ!\n";
        return (1);
    }

    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = PORT;
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        cout << "Ошибка! Невозможно связать сокет с адрессом!\n";
        return (1);
    }

    if (listen(sd, 3) < 0)
    {
        cout << "Ошибка вызова listen()\n";
        return (1);
    }
    else
        cout << "Сервер запущен\n";

    while (1)
    {
        int sd_player1, sd_player2;
        bzero(&caddr, sizeof(caddr));
        socklen_t addrlen;

        sd_player1 = accept(sd, (struct sockaddr *)&caddr, &addrlen);
        sd_player2 = accept(sd, (struct sockaddr *)&caddr, &addrlen);

        // Клиенты подключились
        if (sd_player1 > 0 && sd_player2 > 0)
        {
            cout << "Оба клиента подключились\n";
            // Отправляем заклинания
            send_spells(sd_player1, total_number_of_spells, all_spells);
            send_spells(sd_player2, total_number_of_spells, all_spells);

            Player gamer_1;
            Player gamer_2;

            // Получаем данные об игроках
            if (
                recv(sd_player1, &gamer_1, sizeof(Player), MSG_WAITALL) < 0 ||
                recv(sd_player2, &gamer_2, sizeof(Player), MSG_WAITALL) < 0)
            {
                cout << "Не удалось получить данные о игроках\n";
                return (1);
            }

            cout << "Игроки готовы к битве!\n";

            int match = 0;
            // Начинаем игру
            while (1)
            {
                cout << "\n\nРАУНД " << ++match << endl;
                // Отправляем игрокам данные об их противнике
                if (
                    send(sd_player1, &gamer_2, sizeof(Player), 0) < 0 ||
                    send(sd_player2, &gamer_1, sizeof(Player), 0) < 0)
                {
                    cout << "Ошибка отправки данных о противнике\n";
                    return (1);
                }

                if (gamer_1.dead || gamer_2.dead)
                {
                    close(sd);
                    cout << "Игра окончена\n";
                    return (1);
                }

                int choice_p_1, choice_p_2;
                cout << "Принимаем заклинания\n";
                // Принимаем выбранные заклинания от игроков
                recv(sd_player1, &choice_p_1, sizeof(int), MSG_WAITALL);
                recv(sd_player2, &choice_p_2, sizeof(int), MSG_WAITALL);
                cout << "Приняли заклинания\n";

                // Обрабатываем данные сигналы
                gamer_1.spell_choice_checker(choice_p_1);
                gamer_2.spell_choice_checker(choice_p_2);

                gamer_1.health_check();
                gamer_2.health_check();

                if (gamer_1.dead != true)
                {
                    if (gamer_1.Deck[choice_p_1].type == 1)
                    {
                        gamer_1.got_buffed(choice_p_1);
                    }
                    else if (gamer_1.Deck[choice_p_1].type == 0)
                    {
                        gamer_2.got_damaged(gamer_1.Deck[choice_p_1]);
                    }
                }

                if (gamer_2.dead != true)
                {
                    if (gamer_2.Deck[choice_p_2].type == 1)
                    {
                        gamer_2.got_buffed(choice_p_2);
                    }
                    else if (gamer_2.Deck[choice_p_2].type == 0)
                    {
                        gamer_1.got_damaged(gamer_2.Deck[choice_p_1]);
                    }
                }

                if(!gamer_1.dead)
                    gamer_1.mana -= gamer_1.Deck[choice_p_1].mana_cost;
                
                if(!gamer_2.dead)
                    gamer_2.mana -= gamer_2.Deck[choice_p_2].mana_cost;

                gamer_1.mana += default_mana_regen;
                gamer_2.mana += default_mana_regen;

                gamer_1.print_stats();
                cout << "======================================================\n";
                gamer_2.print_stats();

                // Отправляем состояние игроков им
                send(sd_player1, &gamer_1, sizeof(Player), 0);
                send(sd_player2, &gamer_2, sizeof(Player), 0);
            }
        }
    }

    delete[] all_spells;
    close(sd);
    return (0);
}