#pragma once
#include "Location.h"
#include "io.h"
#include <cstdlib>
#include <time.h>

class Enemies
{
public:
	Enemies(int , int );//c-tor

	//Get Functions
	Location getCurrPosEnemy()const; //returns the enemy current position
	Location getStartPosEnemy()const;//returns the enemy start position

	//Set Functions
	void setCurrPosEnemy(int, int); //sets the enemy current position
	void setStartPosEnemy(int, int);//sets the enemy start position

	Location TryToMove(int& whichDirection, int row, int col); //returns the enemy new position 

	char restoreChar() const; //saves the item was in the cell befor enemy moved there
	void pushCharUnder(char CharToRest); //hides the item while enemy on it

private:
	Location m_currPosEnemy;   //enemy current position
	Location m_startPosEnemy;  //enemy start position
	char m_RestoreCharUnder; //the char under the enemy
};