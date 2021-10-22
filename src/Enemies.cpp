#include "Enemies.h"

Enemies::Enemies(int row, int col)	
	: m_startPosEnemy(row, col), m_currPosEnemy(0, 0) //sets start position for enemy
{
	m_currPosEnemy = m_startPosEnemy; // current position is the start position
	m_RestoreCharUnder = NULL; //there is no item currently under the enemy
}
//----------------------------------------------------
Location Enemies::getStartPosEnemy()const
{//Return the start position of the enemy
	return m_startPosEnemy;
}
//----------------------------------------------------
Location Enemies::getCurrPosEnemy()const
{//Return the current position of the enemy
	return m_currPosEnemy;
}
//----------------------------------------------------
void Enemies::setStartPosEnemy(int row, int col)
{//Sets the start position of the enemy
	m_startPosEnemy.row = row;
	m_startPosEnemy.col = col;
}
//----------------------------------------------------
void Enemies::setCurrPosEnemy(int row, int col)
{//Sets the current position of the enemy
	m_currPosEnemy.row = row;
	m_currPosEnemy.col = col;
}
//----------------------------------------------------
char Enemies::restoreChar() const
{//Restore hidden char
	if (m_RestoreCharUnder == NULL)
		return EMPTY_CELL;
	return m_RestoreCharUnder;
}
//----------------------------------------------------
void Enemies::pushCharUnder(char CharToRest) 
{//Item under the enemy
	m_RestoreCharUnder = CharToRest;
}
//----------------------------------------------------
Location Enemies::TryToMove(int& Direction, int row, int col)
{//Returns enemy new position wanted
	Location SaveLocation(row, col);
	switch (Direction)
	{
	case 1: // KB_UP
		SaveLocation.row = row - 1;
		SaveLocation.col = col;
		Direction = KB_UP;
		break;
	case 2: // KB_DOWN
		SaveLocation.row = row + 1;
		SaveLocation.col = col;
		Direction = KB_DOWN;
		break;
	case 3: //  KB_LEFT
		SaveLocation.row = row ;
		SaveLocation.col = col - 1;
		Direction = KB_LEFT;
		break;
	case 4: // KB_RIGHT
		Direction = KB_RIGHT;
		SaveLocation.row = row ;
		SaveLocation.col = col + 1;
		break;
	default:
		break;
	}
	return SaveLocation;
}

