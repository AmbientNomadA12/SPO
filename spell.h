#ifndef SPELL_H_INCLUDE
#define SPELL_H_INCLUDE

#include <iostream>

const int name_size = 256;

using namespace std;

struct Spell
{
    char name[name_size];
    int mana_cost;
    //0 - Урон, 1 - Бафф
    int type;

    int damage;
    int penetration;

    int health_buff;
    int mana_buff; 
    int defence_buff;

    void print_stats()
    {
        cout << "   \"" << name << "\"" << endl;
        cout << "   Стоимоcть применения: " << mana_cost << endl;
        if(type == 0) 
        {
            cout << "   Урон: " << damage << endl;
            cout << "   Пробитие брони: " << penetration << endl;
        }
        else if(type == 1)
        {
            cout << "   Бонус к здровью: " << health_buff << endl;
            cout << "   Бонус к мане: " << mana_buff << endl;
            cout << "   Бонус к броне: " << defence_buff << endl;
        }
        cout << endl;
    }

};

#endif