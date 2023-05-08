#include <iostream>
#include <Windows.h>
#include <vector>
#include "Creature.h"
#include <string>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

std::vector<Creature> creatures;
std::vector<Creature> playersCreatures;
std::vector<Creature> bot1;
int coolDownPlayer;
int coolDownBot;

/// upgrading method
void upgrade() {
	for (auto& c : playersCreatures) {
		if (c.getExp() >= 15) {
			std::cout << c << std::endl;
			int choose;
			std::cout << "Choose what to upgrade:\n1.Strenght\n2.Hp\n3.Special attack\n";
			std::cin >> choose;
			switch (choose) {
			case 1: {
				c.upgradeStrenght();
				break;
			}
			case 2: {
				c.upgradeHp();
				break;
			}
			case 3: {
				c.upgradeSpecialAttack();
				break;
			}
			default: {
				c.upgradeHp();
				break;
			}
			}
		}
	}
}

/// enums with colors
enum color {
	GREEN,
	RED,
	BLUE,
	WHITE,
	CYAN
};

/// checking if every creature in vector is dead
bool allDead(std::vector<Creature>& cr) {
	for (auto& c : cr)
		if (!c.isDead())
			return false;
	return true;
}

/// healing all players creatures
void heal() {
	for (auto& c : playersCreatures) {
		c.healCreature();
	}
}

/// changing color of consol based on given enum color
void changeColor(const color& color) {
	switch (color) {
	case color::GREEN: {
		SetConsoleTextAttribute(hConsole, 10);
		break;
	}
	case color::RED: {
		SetConsoleTextAttribute(hConsole, 12);
		break;
	}
	case color::BLUE: {
		SetConsoleTextAttribute(hConsole, 9);
		break;
	}
	case color::WHITE: {
		SetConsoleTextAttribute(hConsole, 15);
		break;
	}
	case color::CYAN: {
		SetConsoleTextAttribute(hConsole, 11);
		break;
	}
	}
}

/// generating vector of creatures
void makePoolOfCreatures() {
	creatures.push_back(Creature(std::string("Kikko"), WATER, 100, 20, 30, 15, 0, 0.3));
	creatures.push_back(Creature(std::string("Mokki"), WATER, 60, 30, 35, 15, 0, 0.5));
	creatures.push_back(Creature(std::string("Urashi"), WATER, 150, 10, 15, 15, 0, 0.15));
	creatures.push_back(Creature(std::string("Takiou"), WATER, 100, 15, 20, 15, 0, 0.32));
	creatures.push_back(Creature(std::string("Kumiko"), EARTH, 100, 20, 30, 15, 0, 0.3));
	creatures.push_back(Creature(std::string("Ashiko"), EARTH, 60, 30, 35, 15, 0, 0.5));
	creatures.push_back(Creature(std::string("Richini"), EARTH, 150, 10, 15, 15, 0, 0.15));
	creatures.push_back(Creature(std::string("Yuu"), EARTH, 100, 15, 20, 15, 0, 0.32));	
	creatures.push_back(Creature(std::string("Kunoichi"), AIR, 100, 20, 30, 15, 0, 0.3));
	creatures.push_back(Creature(std::string("Jyukari"), AIR, 60, 30, 35, 15, 0, 0.5));
	creatures.push_back(Creature(std::string("Keiunkai"), AIR, 150, 10, 15, 15, 0, 0.15));
	creatures.push_back(Creature(std::string("Sora"), AIR, 100, 15, 20, 15, 0, 0.32));	
	creatures.push_back(Creature(std::string("Moshiro"), FIRE, 100, 20, 30, 15, 0, 0.3));
	creatures.push_back(Creature(std::string("Minami"), FIRE, 60, 30, 35, 15, 0, 0.5));
	creatures.push_back(Creature(std::string("Kokorou"), FIRE, 150, 10, 15, 15, 0, 0.15));
	creatures.push_back(Creature(std::string("Hi"), FIRE, 100, 15, 20, 15, 0, 0.32));
	creatures.push_back(Creature(std::string("Jinjyuu"), ICE, 100, 20, 30, 15, 0, 0.3));
	creatures.push_back(Creature(std::string("Rokoto"), ICE, 60, 30, 35, 15, 0, 0.5));
	creatures.push_back(Creature(std::string("Hidashi"), ICE, 150, 10, 15, 15, 0, 0.15));
	creatures.push_back(Creature(std::string("Kuriru"), ICE, 100, 15, 20, 15, 0, 0.32));
	creatures.push_back(Creature(std::string("Huji"), STEEL, 100, 20, 30, 15, 0, 0.3));
	creatures.push_back(Creature(std::string("Fukakku"), STEEL, 60, 30, 35, 15, 0, 0.5));
	creatures.push_back(Creature(std::string("Fakku"), STEEL, 150, 10, 15, 15, 0, 0.15));
	creatures.push_back(Creature(std::string("Migashi"), STEEL, 100, 15, 20, 15, 0, 0.32));
}

/// adding creature with choosen element to players vector of creatures
bool chooseCreaturesWithThisElement(element& ele) {
	int choose;
	bool chosen = false;
	int count = 1;
	for (auto& c : creatures) {
		if (ele == c.getElement()) {
			std::cout << count++ << ". " << c << std::endl;
		}
	}
	count = 0;
	std::cin >> choose;
	Creature tmp;
	for (auto& c : creatures) {
		if (ele == c.getElement())
			count++;
		if (count == choose && ele == c.getElement()) {
			playersCreatures.push_back(c);
			chosen = true;
			tmp = c;
		}
	}
	if (chosen == true) {
		changeColor(color::GREEN);
		std::cout << "Creature added to your list)\n";
		changeColor(color::CYAN);
		creatures.erase(std::find(creatures.begin(), creatures.end(), tmp));
	}
	else {
		changeColor(color::RED);
		std::cout << "No such creature, going back to choosing element\n";
		changeColor(color::CYAN);
		return false;
	}
	return true;
}

/// randomly choosing creatures to bot
void randomChooseBot1() {
	//bot1.clear();
	bot1.erase(bot1.begin(), bot1.end());
	bot1.push_back(creatures.at(rand() % creatures.size()));
	std::vector<Creature>::iterator position = std::find(creatures.begin(), creatures.end(), bot1.at(0));
	creatures.erase(position);
	bot1.push_back(creatures.at(rand() % creatures.size()));
	position = std::find(creatures.begin(), creatures.end(), bot1.at(1));
	creatures.erase(position);
	bot1.push_back(creatures.at(rand() % creatures.size()));
	position = std::find(creatures.begin(), creatures.end(), bot1.at(2));
	creatures.erase(position);
	bot1.push_back(creatures.at(rand() % creatures.size()));
	position = std::find(creatures.begin(), creatures.end(), bot1.at(3));
	creatures.erase(position);
}

/// choosing element for choosing creature for player
void chooseCreatures() {
	changeColor(CYAN);
	std::cout << "Choose element:\n1.Water\n2.Earth\n3.Air\n4.Fire\n5.Ice\n6.Steel\n";
	int* choose = new int();
	std::cin >> *choose;
	element* ele = new element();
	switch (*choose) {
	case 1: {
		*ele = WATER;
		break;
	}
	case 2: {
		*ele = EARTH;
		break;
	}
	case 3: {
		*ele = AIR;
		break;
	}
	case 4: {
		*ele = FIRE;
		break;
	}
	case 5: {
		*ele = ICE;
		break;
	}
	case 6: {
		*ele = STEEL;
		break;
	}
	default: {
		*ele = WATER;
		break;
	}
	}
	chooseCreaturesWithThisElement(*ele);
	if (playersCreatures.size() < 4)
		chooseCreatures();
}

/// change the creature that fights right now
int change() {
	int count = 1;
	for (auto& c : playersCreatures) {
		std::cout << count++ << ". " << c << std::endl;
	}
	std::cin >> count;
	return --count;
}

/// fighting method
bool tour(int fline, std::vector<Creature>& bot, int botCreature) {
	if (fline >= 4 || fline < 0)
		fline = 0;
	if (coolDownBot > 0) 
		coolDownBot--;
	if (coolDownPlayer > 0) 
		coolDownPlayer--;
	botCreature;
	changeColor(color::GREEN);
	std::cout << "Your creature:\n" << playersCreatures[fline] << std::endl;
	changeColor(color::RED);
	std::cout << "Oponents creature:\n" << bot[botCreature] << std::endl;
	changeColor(color::CYAN);
	std::cout << "Action:\n1.Attack\n2.Special attack\n3.Change creature\n";
	int choose;
	std::cin >> choose;
	switch (choose) {
	case 1: {
		playersCreatures[fline].attack(bot[botCreature]);
		break;
	}
	case 2: {
		if (coolDownPlayer == 0) {
			playersCreatures[fline].spAttack(bot[botCreature]);
			coolDownPlayer = 4;
		}
		else {
			changeColor(color::RED);
			std::cout << "Your special attack is still on cool down: " << coolDownPlayer << std::endl;
			changeColor(color::CYAN);
		}
		break;
	}
	case 3: {
		fline = change();
		if (fline >= 4 || playersCreatures[fline].isDead()) {
			int i = 0;
			while (playersCreatures[i].isDead() && i < playersCreatures.size())
				i++;
			fline = i;
		}
		break;
	}
	default: {
		break;
	}
	}
	if (bot[botCreature].isDead() && !allDead(bot)) {
		botCreature++;
		return tour(fline, bot, botCreature);
	}
	if (allDead(bot)) 
		return true;
	if (rand() % 100 > 90 && coolDownBot == 0) {
		bot[botCreature].spAttack(playersCreatures[fline]);
		coolDownBot = 4;
	}
	else 
		bot[botCreature].attack(playersCreatures[fline]);
	if (allDead(playersCreatures))
		return false;
	else
		if (playersCreatures[fline].isDead()) {
			fline = change();
			if (fline >= 4 || playersCreatures[fline].isDead()) {
				int i = 0;
				while (playersCreatures[i].isDead() && i < playersCreatures.size())
					i++;
				fline = i;
			}
			return tour(fline, bot, botCreature);
		}
		else
			return tour(fline, bot, botCreature);
}

/// startign tours
void startGame() {
	std::cout << "Choose your first creature\n";
	int count = 1;
	int choose;
	bool won;
	changeColor(color::BLUE);
	for (auto& c : playersCreatures) {
		std::cout << count++ << ". " << c << std::endl;
	}
	changeColor(color::CYAN);
	std::cin >> choose;
	randomChooseBot1();
	coolDownPlayer = 0;
	coolDownBot = 0;
	won = tour(--choose, bot1, 0);
	if (!won) {
		changeColor(color::RED);
		std::cout << "You lose\n";
		changeColor(color::CYAN);
		return;
	}
	else {
		changeColor(color::GREEN);
		std::cout << "You won, starting next round!\n";
		changeColor(color::CYAN);
	}
	upgrade();
	heal();

	randomChooseBot1();
	changeColor(color::BLUE);
	for (auto& c : playersCreatures) {
		std::cout << count++ << ". " << c << std::endl;
	}
	changeColor(color::CYAN);
	std::cin >> choose;
	coolDownPlayer = 0;
	coolDownBot = 0;
	won = tour(--choose, bot1, 0);
	if (!won) {
		changeColor(color::RED);
		std::cout << "You lose\n";
		changeColor(color::CYAN);
		return;
	}
	else {
		changeColor(color::GREEN);
		std::cout << "You won, starting next round!\n";
		changeColor(color::CYAN);
	}
	upgrade();
	heal();

	randomChooseBot1();
	changeColor(color::BLUE);
	for (auto& c : playersCreatures) {
		std::cout << count++ << ". " << c << std::endl;
	}
	changeColor(color::CYAN);
	std::cin >> choose;
	coolDownPlayer = 0;
	coolDownBot = 0;
	won = tour(--choose, bot1, 0);
	if (!won) {
		changeColor(color::RED);
		std::cout << "You lose\n";
		changeColor(color::CYAN);
		return;
	}
	else {
		changeColor(color::GREEN);
		std::cout << "You won, starting next round!\n";
		changeColor(color::CYAN);
	}
	upgrade();
	heal();

	randomChooseBot1();
	changeColor(color::BLUE);
	for (auto& c : playersCreatures) {
		std::cout << count++ << ". " << c << std::endl;
	}
	changeColor(color::CYAN);
	std::cin >> choose;
	coolDownPlayer = 0;
	coolDownBot = 0;
	won = tour(--choose, bot1, 0);
	if (!won) {
		changeColor(color::RED);
		std::cout << "You lose\n";
		changeColor(color::CYAN);
		return;
	}
	else {
		changeColor(color::GREEN);
		std::cout << "You won the game!\n";
		changeColor(color::CYAN);
	}
	upgrade();
	heal();
}

/// startign new game
void newGame() {
	makePoolOfCreatures();
	chooseCreatures();
	startGame();
}

///start menu
void start() {
	changeColor(color::CYAN);
	std::cout << "Welcome, please choose option:\n" <<
		"1) New game\n" <<
		"2) Exit\n";
	changeColor(WHITE);
	int* option = new int();
	std::cin >> *option;
	switch (*option) {
	case 1: {
		newGame();
		break;
	}
	case 2: {
		exit(0);
		break;
	}
	default: {
		std::cout << "No such option\n";
		return start();
	}
	}
	return start();
}


/// main methods
int main() {
	start();
}