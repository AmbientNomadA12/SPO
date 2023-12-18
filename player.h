#ifndef PLAYER_H_INCLUDE
#define PLAYER_H_INCLUDE

#include <iostream>
#include </home/sergey/Desktop/labs/10/spell.h>

const int deck_sise = 3;

int default_health = 15;
int default_mana = 20;
int default_defence = 10;

using namespace std;

struct Player
{
    int health = default_health;
    int mana = default_mana;
    int defence = default_defence;

    Spell Deck[deck_sise];

    bool dead = false;

    void got_buffed(int choosen_spell)
    {
        health += Deck[choosen_spell].health_buff;
        defence += Deck[choosen_spell].defence_buff;
        mana += Deck[choosen_spell].mana_buff;
    }

    void got_damaged(Spell enemy_spell)
    {
        if(defence > 0)
        {
            defence -= enemy_spell.penetration;   
        }

        if(defence <= 0)
        {
            health -= enemy_spell.damage;
            defence = 0;
        }
    }
    
    void health_check()
    {
        if(health <= 0 || mana <= 0)
            dead = true;
    }

    void spell_choice_checker(int choosen_spell)
    {
        if (choosen_spell < 0 || choosen_spell >= deck_sise)
        {
            dead = true;
        }
        else if (Deck[choosen_spell].mana_cost > mana)
        {
            dead = true;
        }
    }

    void print_spells()
    {
        cout << "Ваши заклинания:\n";
        for (int i = 0; i < deck_sise; ++i)
        {
            cout << "НОМЕР ЗАКЛИНАНИЯ: " << i << endl;
            Deck[i].print_stats();
        }
    }

    void print_stats()
    {
        cout << "   Здоровье: " << health << endl;
        cout << "   Мана: " << mana << endl;
        cout << "   Броня: " << defence << endl;
    }

    void print_stats_spells()
    {
        cout << "Ваши характеристики:\n";
        cout << "   Здоровье: " << health << endl;
        cout << "   Мана: " << mana << endl;
        cout << "   Броня: " << defence << endl;
        if(dead) 
            cout << "  Мерт\n";
        else 
            cout << "  Жив\n";

        print_spells();
    }
};

#endif