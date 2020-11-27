#include "Character.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;
//function gives the index of the defender or attacker in community arrays and return it
int findIndex(vector<Character> *community, string name)
{
    for (int i = 0; i < 5; i++)
    {
        if ((*community)[i].name == name)
        {
            if ((*community)[i].isAlive)
                return i;

            int after = i + 1;
            int before = i - 1;
            while (after < 5)
            {

                if ((*community)[after].isAlive)
                    return after;

                after++;
            }
            while (before >= 0)
            {

                if ((*community)[before].isAlive)
                    return before;

                before--;
            }
        }
    }
    // cout << "olumler boyumuzu asti" << endl;
    return -1;
}
//it prints the results according to wanted order
string print(Character a, int index)
{
    string result = a.name + " ";
    for (int i = 0; i < index + 2; i++)
    {
        string m = to_string(a.healthHistory[i]) + " ";
        result += m;
    }

    return result;
}
vector<Character> reversesort(vector<Character> com, string arr[5])
{
    vector<Character> reversed;
    for (int i = 0; i < 5; i++)
    {
        for (int m = 0; m < 5; m++)
        {
            if (arr[i] == com[m].name)
            {
                reversed.push_back(com[m]);
            }
        }
    }
    return reversed;
}

int main(int argc, char *argv[])
{

    ifstream file;
    ofstream myfile;
    string com1[5];
    string com2[5];
    vector<Character> community1;
    vector<Character> community2;

    file.open(argv[1]);
    myfile.open(argv[2]);
    int numberOfAttack;
    file >> numberOfAttack;
    //this loop creates community arrays
    for (int i = 0; i < 10; i++)
    {
        string name;
        string type;
        int attackPoint;
        int defensePoint;
        int initialHealth;

        file >> name >> type >> attackPoint >> defensePoint >> initialHealth;
        
        if (i < 5)
        {
            com1[i] = name;
            community1.push_back( Character(name, type, attackPoint, defensePoint, initialHealth, numberOfAttack));
        }
        else
        {
            com2[i % 5] = name;
            community2.push_back( Character(name, type, attackPoint, defensePoint, initialHealth, numberOfAttack));
        }
    }
//sorts arrays 
    sort(community1.begin(), community1.end());
    sort(community2.begin(), community2.end());
    int deadNum1 = 0;
    int deadNum2 = 0;
    bool isDeadHobbit1 = false;
    bool isDeadHobbit2 = false;
    Character *dead1 = NULL;
    Character *dead2 = NULL;

    for (int i = 0; i < numberOfAttack; i++)
    {
        //find defender and attacker 
        string attacker;
        string defender;
        string typeOfAttack;
        file >> attacker >> defender >> typeOfAttack;
        vector<Character> *attackerSide = i % 2 == 0 ? &community1 : &community2;
        vector<Character> *defenderSide = i % 2 == 1 ? &community1 : &community2;
        Character *attack = &(*attackerSide)[findIndex(attackerSide, attacker)];
        Character *defend = &(*defenderSide)[findIndex(defenderSide, defender)];
        //find damage
        int damage = attack->attack - defend->defense;
        if (damage < 0)
            damage = 0;

        defend->remainingHealth = defend->remainingHealth - damage > 0 ? defend->remainingHealth - damage : 0;
        if (typeOfAttack == "SPECIAL")
        {

            if (attack->type == "Elves" && i - attack->nRoundsSinceSpecial > 10)
            {

                Character *hobbit;
                for (int m = 0; m < 5; m++)
                {
                    if ((*attackerSide)[m].type == "Hobbit")
                        hobbit = &(*attackerSide)[m];
                }
                hobbit->remainingHealth += attack->remainingHealth / 2;
                attack->remainingHealth /= 2;
                attack->nRoundsSinceSpecial = i;
            }
            if (attack->type == "Dwarfs" && i - attack->nRoundsSinceSpecial > 20 )

            {

                defend->remainingHealth = defend->remainingHealth - damage > 0 ? defend->remainingHealth - damage : 0;
                attack->nRoundsSinceSpecial = i;
            }
            if (attack->type == "Wizards" && i - attack->nRoundsSinceSpecial > 50 )
            {

                Character *lastDead = NULL;
                if (i % 2 == 0 && dead1!=NULL)
                {
                    lastDead = dead1;
                   
                    if(!(dead1->isAlive)){
                    lastDead->nRoundsSinceSpecial=i;
                    deadNum1--;

                    }
                }
                if (i % 2 == 1 && dead2!=NULL)
                {
                    lastDead = dead2;
                   
                    if(!(dead2->isAlive)){
                    lastDead->nRoundsSinceSpecial=i;

                    deadNum2--;
                    }
                }
                if (lastDead!=NULL)
                {

                    lastDead->remainingHealth = lastDead->healthHistory[0];
                    lastDead->isAlive = true;
                    attack->nRoundsSinceSpecial = i;
                }

            }
        }
        defend->isAlive = defend->remainingHealth > 0;

        if (!(defend->isAlive))
        {

            if (i % 2 == 0)
            {
                deadNum2++;
                dead2 = defend;
            }
            if (i % 2 == 1)
            {
                deadNum1++;
                dead1 = defend;
            }
        }
        for (int k = 0; k < 5; k++)
        {
            community1[k].healthHistory[i + 1] = community1[k].remainingHealth;

            if (community1[k].type == "Hobbit" && !community1[k].isAlive)
                isDeadHobbit1 == true;
        }
        for (int k = 0; k < 5; k++)
        {
            community2[k].healthHistory[i + 1] = community2[k].remainingHealth;
            if (community2[k].type == "Hobbit" && !community2[k].isAlive)
                isDeadHobbit2 == true;
        }
        if (deadNum1 >= 4 || isDeadHobbit1 || deadNum2 >= 4 || isDeadHobbit2 || i == numberOfAttack - 1)
        {
            community2 = reversesort(community2, com2);
            community1 = reversesort(community1, com1);
            string winner = "";

            winner = (deadNum1 >= 4 || isDeadHobbit1) ? "Community-2" : " ";
            winner = (deadNum2 >= 4 || isDeadHobbit2) ? "Community-1" : " ";
            if (i == numberOfAttack - 1 && winner == " ")
                winner = "Draw";

            myfile << winner << "\n";
            myfile << i + 1 << "\n";
            myfile << deadNum1 + deadNum2 << "\n";
            for (int k = 0; k < 5; k++)
            {
                myfile << print(community1[k], i) << "\n";
            }
            for (int k = 0; k < 5; k++)
            {
                myfile << print(community2[k], i) << "\n";
            }
            break;
        }
    
    }
    myfile.close();
    file.close();

    return 0;
}