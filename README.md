# Ex2-OOP1
2nd year sem 1

OOP, ex2
-----------------------------
Students:
Eliachar Feig - 302278338
-----------------------------
General Explanation:
in this excrecise we will make a version of the "Load Runner" game.
the game will be played in the CMD terminal
and it will be static, turn after turn, player, than the enemies.

% - are enemies
@ - is the Player
# - are walls and cant be passed
-- - Pole : player can only move left and right ot fall down
H - ladder: player can only move up and down
* - coins: the player collecting them, when done you win

if the enemie eats the player - The level reset , and life deacrese by one.
if lives are finished, you lose.

if the player ate all the coins, you will pass to the next level

movement with the arrows keys.

Have Fun !!
-----------------------------
Files created by the students:

GameManager.cpp
GameManager.h

Player.cpp
Player.h

Board.cpp
Board.h

Enemies.cpp
Enemies.h
-----------------------------
Data Structures used:
Vectors of structures. 
-----------------------------
Notable Algorithms:
Enemie movement :
first level : 
partly randomal and partly twoards the player ,
for balanced gaming experianced.
second level :
only twoards the player,
to upgrade the difficult level.
-----------------------------
Known bugs:
no known bugs
-----------------------------
Other comments:
board class holds static objects and everything related to the board Representation -> vector of strings

gameManager - holds anything for managing the game, from starting it, building it, restarting. and using all of the other calsses

Player class: holds Player data and setting of location and his charictaristics

Enemies class: holds Enemies data and setting of location and his charictaristics
