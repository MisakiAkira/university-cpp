#pragma once
#include <string>
#include <iostream>
#include <map>

enum element {
	WATER,
	EARTH,
	AIR,
	FIRE,
	ICE,
	STEEL
};

class Creature {
public:
	/// <summary>
	/// creating creature
	/// </summary>
	/// <param name="name"> name of creature </param>
	/// <param name="element"> element of creature </param>
	/// <param name="maxHp"> creatures max hp </param>
	/// <param name="strength"> creatures strenght </param>
	/// <param name="spAt"> creatures max attack </param>
	/// <param name="expDrop"> how many creature drop exp </param>
	/// <param name="exp"> how many creature xp got </param>
	/// <param name="dodge"> dodge rate o fcreature </param>
	Creature(std::string name, const element element, int maxHp, int strength, int spAt, int expDrop, int exp, double dodge) : name(name), element(element),
		hp(maxHp), strength(strength), spAt(spAt), expDrop(expDrop), exp(exp), dodge(dodge), dead(false), maxHp(maxHp) {};

	/// <summary>
	/// override cout for creature
	/// </summary>
	/// <param name="o"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	friend std::ostream& operator << (std::ostream& o, Creature c) {
		std::string elem;
		switch (c.element) {
		case WATER: {
			elem = "Water";
			break;
		}
		case EARTH: {
			elem = "Earth";
			break;
		}
		case AIR: {
			elem = "Air";
			break;
		}
		case FIRE: {
			elem = "Fire";
			break;
		}
		case ICE: {
			elem = "Ice";
			break;
		}
		case STEEL: {
			elem = "Steel";
			break;
		}
		}
		return o << "Creatures name: " << c.name << "\nElement: " << elem << "\nHp: " << c.hp <<
			"\nStrength: " << c.strength << "\nSpecial attack strength: " << c.spAt << "\nDodge rate: " << c.dodge * 100 << "%\n";
	}

	/// <summary>
	/// constructor without params
	/// </summary>
	Creature() : name(""), element(WATER), hp(0), strength(0), spAt(0), expDrop(0), exp(0), dodge(0.0) {};
	element getElement() {
		return element;
	}

	/// <summary>
	/// override the compair method
	/// </summary>
	/// <param name="rhs"></param>
	/// <returns></returns>
	bool operator==(const Creature& rhs) {
		if (name.compare(rhs.name) == 0 && element == rhs.element)
			return true;
		return false;
	};

	/// <summary>
	/// geting modifer of attack based on elements
	/// </summary>
	/// <param name="a"> creature that we attacking </param>
	/// <returns> modifier </returns>
	float getModifier(Creature& a) {
		switch (this->element) {
		case WATER: {
			if(a.element == WATER || a.element == FIRE || a.element == ICE)
				return 0.5;
			if (a.element == EARTH || a.element == AIR)
				return 1;
			if (a.element == STEEL)
				return 1.5;
			break;
		}
		case EARTH: {
			if (a.element == WATER || a.element == ICE)
				return 1.5;
			if (a.element == EARTH || a.element == STEEL)
				return 1;
			if (a.element == AIR || a.element == FIRE)
				return 0.5;
			break;
		}
		case AIR: {
			if (a.element == WATER || a.element == AIR || 
				a.element == FIRE || a.element == ICE)
				return 1;
			if (a.element == EARTH)
				return 0.5;
			if (a.element == STEEL)
				return 1.5;
			break;
		}
		case FIRE: {
			if (a.element == WATER || a.element == EARTH)
				return 1.5;
			if (a.element == AIR || a.element == FIRE)
				return 1;
			if (a.element == ICE || a.element == STEEL)
				return 0.5;
			break;
		}
		case ICE: {
			if (a.element == WATER || a.element == STEEL)
				return 1;
			if (a.element == EARTH || a.element == FIRE)
				return 1.5;
			if (a.element == AIR)
				return 2;
			if (a.element == ICE)
				return 0.5;
			break;
		}
		case STEEL: {
			if (a.element == WATER || a.element == ICE)
				return 1;
			if (a.element == EARTH || a.element == FIRE)
				return 1.5;
			if (a.element == AIR || a.element == STEEL)
				return 0.5;
			break;
		}
		}
	}

	/// <summary>
	/// attack method
	/// </summary>
	/// <param name="a"> creature that getting damage </param>
	void attack(Creature& a) {
		float modifier = getModifier(a);
		if (rand() % 100 > a.dodge * 100)
			a.hp -= strength * modifier;
		else {
			std::cout << "Missed!\n";
		}
		if (a.hp <= 0) {
			a.hp = 0;
			exp += a.expDrop;
			a.dead = true;
		}
	}

	/// <summary>
	/// special attack
	/// </summary>
	/// <param name="a"> creature that getting damage </param>
	void spAttack(Creature& a) {
		float modifier = getModifier(a);
		if (modifier == 0.5)
			modifier = 0;
		else if (modifier == 1.5)
			modifier = 2;
		else if (modifier == 2)
			modifier = 2.5;
		a.hp -= spAt * modifier;
		if (a.hp <= 0) {
			a.hp = 0;
			exp += a.expDrop;
			a.dead = true;
		}
	}

	/// <summary>
	/// cheking is creature is dead
	/// </summary>
	/// <returns> returning true if creature is dead </returns>
	bool isDead() {
		return dead;
	}

	/// <summary>
	/// healling creature to max hp
	/// </summary>
	void healCreature() {
		hp = maxHp;
	}

	/// <summary>
	/// gettign creatures exp
	/// </summary>
	/// <returns> exp </returns>
	int getExp() {
		return exp;
	}

	/// <summary>
	/// upgrading strength of creature
	/// </summary>
	void upgradeStrenght() {
		strength += 10;
		exp -= 15;
	}

	/// <summary>
	/// upgrading creatures hp
	/// </summary>
	void upgradeHp() {
		maxHp += 20;
		exp -= 15;
	}

	/// <summary>
	/// upgrafing creatures special attack
	/// </summary>
	void upgradeSpecialAttack() {
		spAt += 5;
		exp -= 15;
	}
protected:
	element element;
	std::string name;
	int hp, strength, spAt, expDrop, exp, maxHp;
	double dodge;
	bool dead;
};

