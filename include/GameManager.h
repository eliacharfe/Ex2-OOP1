#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include "io.h"
#include "Location.h"
#include "Player.h"
#include "Enemies.h"

using std::ifstream;
using std::ofstream;
using std::endl;
using std::cout;
using std::vector;
using std::string;

//enum objects_t { // set in "io.h" //
//	PLAYER = '@', ENEMY = '%', WALL = '#', POLE = '-', LADDER = 'H',
//	PLAYER_ON_SCALE = 'S', COIN = '*', EMPTY_CELL = ' '
//}; 

class GameManager
{
public:
	GameManager();//c-tor
	void StartGame(ifstream& BoardFile);
	void SetStartLevelPosition_andCount();
	bool WinCurrLevel();
	void PrintBoard();
	void LOOSE();
	void WIN();


	//Moves Functions
	bool MovePlayer(int row, int col, bool& is_last_coin); //Moves the player
	bool MoveEnemy(int enemy, int row, int col); // mooving the enemie
	void WhenPlayerFalling(int& row, int col,
		bool& enemy, bool& is_last_coin, bool is_scale, bool is_pole);

	//Checks Functions
	bool DoEnemy(); //Doing all the checks befor moving the enemy
	bool MoveOk(int direction, int row, int col)const; //Check if wanted move is legal
	bool NotOnFloor(int row, int col)const; //check if item is on floor
	int getDirectionEnemy(int enemy)const;

private:
	vector<std::string> m_GameBoard; // The game board 
	Player m_player; //Main player
	vector <Enemies> m_enemies;// Enemies Positions

	int m_level; //level number
	int m_score; //counts how many points we got
	int m_size_board; //the NxN board size
	int m_count_coins;//counts how many coins we collect
	int m_count_enemy;
};

//bool isCoin(int row, int col)const;
	//bool WhenEnemyFalling(int enemy, int& row, int col,
	//	 bool is_scale, bool is_pole);
	//bool isEmptyCell(int row, int col);
	//bool isLadder(int row, int col)const;
	//bool isPole(int row, int col)const;
	//bool isEnemy(int row,int col)const;
	//int getSizeBoard()const;
