#include "Player.h"

Player::Player() 
	: m_startPosPlayer(1,2), m_currPosPlayer(1, 2) //Set start position
{
	m_life = LIFE; //Set full life
}
//----------------------------------------------------
Location Player::getStartPosPlayer()const
{//Returns player start position
	return m_startPosPlayer;
}
//----------------------------------------------------
Location Player::getCurrPosPlayer()const
{//Returns player current position
	return m_currPosPlayer;
}
//----------------------------------------------------
void Player::setStartPosPlayer(int row, int col)
{//Sets player start position
	m_startPosPlayer.row = row;
	m_startPosPlayer.col = col;
}
//----------------------------------------------------
void Player::setCurrPosPlayer(int row, int col)
{//Sets player current position
	m_currPosPlayer.row = row;
	m_currPosPlayer.col = col;
	m_RestoreCharUnder = NULL;
}
//----------------------------------------------------
void Player::setDecreaseLife()
{//Deacrease life when crossing an enemy
	m_life--;
}
//----------------------------------------------------
int Player::getLife()const
{//Returns amount of lives left
	return m_life;
}
//----------------------------------------------------
char Player::restoreChar() const
{//Restore hidden char under the player
	if (m_RestoreCharUnder == NULL)
		return ' ';
	return m_RestoreCharUnder;
}
//----------------------------------------------------
void Player::pushCharUnder(char CharToRest) 
{//Saves the char under the Player
	m_RestoreCharUnder = CharToRest;
}
//----------------------------------------------------
void Player::MovePlayer(int whichDirection)
{//Returns the player new position after making a move
	Location currPos = getCurrPosPlayer(); //current player position
	int row = currPos.row;
	int col = currPos.col;
	switch (whichDirection)
	{
	case KB_UP: //If Up
		setCurrPosPlayer(row - 1, col);
		break;
	case KB_DOWN: //If Down
		setCurrPosPlayer(row + 1, col);
		break;
	case KB_LEFT: //If Left
		setCurrPosPlayer(row, col - 1);
		break;
	case KB_RIGHT: //If Right
		setCurrPosPlayer(row, col + 1);
		break;
	default:
		break;
	}
}
//----------------------------------------------------
Location Player::TryToMove(int& whichDirection, int row, int col)
{//When player tries to make a move save the new location
	Location SaveLocation(row, col);
	switch (whichDirection)
	{
	case  KB_UP:
		SaveLocation.row = row - 1;
		SaveLocation.col = col;
		break;
	case KB_DOWN:
		SaveLocation.row = row + 1;
		SaveLocation.col = col;
		break;
	case KB_LEFT:
		SaveLocation.row = row;
		SaveLocation.col = col - 1;
		break;
	case  KB_RIGHT: 
		SaveLocation.row = row;
		SaveLocation.col = col + 1;
		break;
	default:
		break;
	}
	return SaveLocation;
}