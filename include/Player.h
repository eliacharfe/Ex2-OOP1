#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include "io.h"
#include "Location.h"

using std::ifstream;
//-----------------------
const int LIFE = 3;
//---------------------
class GameManager;
//-------------------------
class Player
{
public:
	Player(); // c-tor

	//Get Functions
	Location getCurrPosPlayer() const; //Returns Player current position
	Location getStartPosPlayer()const; //Returns Playr start position
	int getLife()const; //Returns the amount of lives left

	//Set Functions
	void setCurrPosPlayer(int, int); //Sets Player current position
	void setStartPosPlayer(int, int);//Sets Playr start position
	void setDecreaseLife(); //Sets Player life after crossing an enemy

	//Player moves Functions
	void MovePlayer(int); //Move Player to new position
	Location TryToMove(int& whichDirection, int row, int col); //Saves the Player new position

	char restoreChar() const; //Restore the item under the player
	void pushCharUnder(char CharToRest); //Hide the Item the player is on

private:
	Location m_currPosPlayer; //The player current position
	Location m_startPosPlayer; //The player start position
	int m_life; //Amount of lives left
	char m_RestoreCharUnder; //The char the Player is on 
};