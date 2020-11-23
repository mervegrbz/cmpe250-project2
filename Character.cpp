#include "Character.h"
#include<iostream>
Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) {
    name=_name;
    type=_type;
    attack=_attack;
    defense=_defense;
    remainingHealth=_remainingHealth;
    nMaxRounds=_nMaxRounds;
    healthHistory=new int(nMaxRounds+1);
    healthHistory[0]=remainingHealth;
    nRoundsSinceSpecial=0;


}

Character::Character(const Character& character) {
    name=character.name;
    type=character.type;
    attack=character.attack;
    defense=character.defense;
    remainingHealth=character.remainingHealth;
    nMaxRounds=character.nMaxRounds;
    nRoundsSinceSpecial=character.nRoundsSinceSpecial;
     healthHistory=new int[nMaxRounds+1];
     for(int i=0;i<nMaxRounds+1;i++){
         healthHistory[i]=character.healthHistory[i];
     }




}

Character& Character::operator=(const Character& character) {
    name=character.name;
    type=character.type;
    attack=character.attack;
    defense=character.defense;
    remainingHealth=character.remainingHealth;
    nMaxRounds=character.nMaxRounds;
    nRoundsSinceSpecial=character.nRoundsSinceSpecial;
    if(healthHistory!=NULL)
        delete[] healthHistory;
    healthHistory=new int[nMaxRounds+1];
    for(int i=0;i<nMaxRounds+1;i++){
        healthHistory[i]=character.healthHistory[i];
    }
    cout<<"heee";
    return *this;

}

bool Character::operator<(const Character& other) {
    return this->name<other.name;


}

Character::~Character() {

}