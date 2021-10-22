#include "GameManager.h"

//--------------------------------------------------------------------------
GameManager::GameManager() //Sets Default Values
{
	m_level = 1; m_score = 0; m_size_board = 0;
	m_count_coins = 0; m_count_enemy = 0;
}
//--------------------------------------------------------------------------
void GameManager::StartGame(ifstream& BoardFile)
{
	while (true)
	{
		auto restartLevel = BoardFile.tellg(); //when crossing enemy - restart the level
		string str;
		getline(BoardFile, str);  // Gets the Size of screen
		m_size_board = stoi(str);

		m_GameBoard.clear();  m_enemies.clear(); //clear the board //clear the enemy vec
		for (int i = 0; i < m_size_board; i++)
		{	//build the updated game board from the rellevent screen
			string str;
			getline(BoardFile, str);
			m_GameBoard.push_back(str);
		}
		cout << "Press up, down, left or right to move the player" << endl << endl;
		PrintBoard(); //print the game board

		if (!WinCurrLevel())// run game // false if crossed by an enemy
		{
			LOOSE(); // cautch by an enemy
			if (m_player.getLife() == 0)
				return;
			BoardFile.seekg(restartLevel); // print start of level board 
		}
		else // if won the level
			WIN();

		/* if got here is because win or loose */
		if (m_count_coins == 0)
			BoardFile.get(); // if win go next level
	}
}
//--------------------------------------------------------------------------
bool GameManager::WinCurrLevel()
{
	SetStartLevelPosition_andCount(); //count level datas and set positions
	bool is_ladder = false;		bool is_pole = false;	 
	bool is_enemy = false;		bool is_last_coin = false;
	while (auto Direction = Keyboard::getch()) // get new key from user
	{
		if (Direction == 224)	continue;
		Location CurrPlayerPos = m_player.getCurrPosPlayer();//save player position for check
		Location WantedEnemyPos = m_player.TryToMove(Direction, CurrPlayerPos.row, CurrPlayerPos.col);
		int PlayerRow = WantedEnemyPos.row;
		int PlayerCol = WantedEnemyPos.col;

		if (!MoveOk(Direction, PlayerRow, PlayerCol))// if invalid input //check if is wall  or end of screen
			continue;// go get new key from user

		if (NotOnFloor(PlayerRow, PlayerCol))
		{//if falling
			WhenPlayerFalling(PlayerRow, PlayerCol, is_enemy, is_last_coin, is_ladder, is_pole);
			if (is_enemy)// if cross an enemy
				return false;//restart level
			if (is_last_coin)// if when fall eat last coin = WIN!
				return true;//next level

			if (!DoEnemy())//move enemies
				return false;
			continue;
		}
		if (!MovePlayer(PlayerRow, PlayerCol, is_last_coin))
			return false;
		if (is_last_coin)// if when falling eat last coin = WIN!
			return true;//next level

		if (!DoEnemy())//move enemies
			return false;
		PrintBoard();
	}
	return true;
}
//--------------------------------------------------------------------------
bool GameManager::MovePlayer(int PlayerRow, int PlayerCol, bool& is_last_coin) 
{//Moving the Enemy to the wanted location
	char charToResrore = m_GameBoard[PlayerRow][PlayerCol]; //the item the player is stepping on
	m_GameBoard[PlayerRow][PlayerCol] = PLAYER;//update player new position on board
	m_GameBoard[m_player.getCurrPosPlayer().row][m_player.getCurrPosPlayer().col]
		= m_player.restoreChar();//restore char
	m_player.setCurrPosPlayer(PlayerRow, PlayerCol);//set player position

	//*** enum in "io.h" file ***
	switch (charToResrore) //which char we need to restore
	{	
	case EMPTY_CELL: //if steps in empty cell
		m_player.pushCharUnder(EMPTY_CELL);
		break;
	case ENEMY:	 //if crossing an enemy
		m_player.setDecreaseLife(); 
		return false;
	case COIN:	//if eating a coin
		m_player.pushCharUnder(EMPTY_CELL);  
		m_count_coins--;	//decrease coin
		m_score += 2 * m_level;
		if (m_count_coins == 0)	//if eaten all the coins
			is_last_coin = true;// win // go next level
		break;
	case POLE: //if stepping on Pole
		m_player.pushCharUnder(POLE); 
		break;
	case LADDER:  //if stepping on ladder
		m_player.pushCharUnder(LADDER);
		m_GameBoard[PlayerRow][PlayerCol] = PLAYER_ON_SCALE;//update
		break;
	case PLAYER:
		m_player.pushCharUnder(PLAYER); 
		break;
	default: break;
	}
	return true;
}
//--------------------------------------------------------------------------
bool GameManager::DoEnemy() 
{//Moving the enemy after checking the move is legal
	bool is_ladder = false, is_pole = false;

	for (int enemy = 0; enemy < m_count_enemy; enemy++)
	{
		int Direction = getDirectionEnemy(enemy); //the wanted direction to move
		Location CurrEnemyPos = m_enemies[enemy].getCurrPosEnemy();//save in case move is illigal
		Location WantedEnemyPos = m_enemies[enemy].TryToMove(Direction, CurrEnemyPos.row, CurrEnemyPos.col);
		int EnemyRow = WantedEnemyPos.row;
		int EnemyCol = WantedEnemyPos.col;

		if (!MoveOk(Direction, EnemyRow, EnemyCol))//check if is wall or end of screen 
			continue;

		if (!MoveEnemy(enemy, EnemyRow, EnemyCol))
			return false;
		PrintBoard();
	}
	return true;
}
//--------------------------------------------------------------------------
bool GameManager::MoveEnemy(int enemy, int row, int col) 
{//Mooving the enemie
	char charToRestore = m_GameBoard[row][col];
	m_GameBoard[row][col] = ENEMY;
	m_GameBoard[m_enemies[enemy].getCurrPosEnemy().row][m_enemies[enemy].getCurrPosEnemy().col] 
		= m_enemies[enemy].restoreChar();//restore
	m_enemies[enemy].setCurrPosEnemy(row, col);

	switch (charToRestore) //which char we need to restore
	{
	case EMPTY_CELL: //if stepping on emoty cell
		m_enemies[enemy].pushCharUnder(EMPTY_CELL);
		break;
	case PLAYER: //if crossing the player
		m_player.setDecreaseLife(); //deacrse player life
		return false;
	case POLE: //if stepping on pole
		m_enemies[enemy].pushCharUnder(POLE);
		break;
	case LADDER: //if stepping a ladder
		m_enemies[enemy].pushCharUnder(LADDER);
		break;
	case COIN: //if stepping on a coin
		m_enemies[enemy].pushCharUnder(COIN);
		break;
	default: break;
	}
	return true;
}
//--------------------------------------------------------------------------
bool GameManager::MoveOk(int Direction, int Row, int Col) const
{//This function check if the wanted move is legal and returns boolian value
	switch (Direction)
	{
	case KB_UP: if (!Row) //out of screen
		return false; break;
	case KB_DOWN: if (Row == (m_size_board - 1)) //out of screen
		return false; break;
	case KB_RIGHT: if (Col == (m_size_board - 1)) //out of screen
		return false; break;
	case KB_LEFT: if (!Col) //out of screen
		return false; break;
	default: return false;
	}
	if (m_GameBoard[Row][Col] == WALL) // if is wall or floor ('#')
		return false;
	return true;
}
//--------------------------------------------------------------------------
bool GameManager::NotOnFloor(int row, int col)const
{
	return (m_GameBoard[++row][col] == EMPTY_CELL || m_GameBoard[row][col] == LADDER 
	|| m_GameBoard[row][col] == POLE || m_GameBoard[row][col] == PLAYER || m_GameBoard[row][col] == ENEMY);
}
//--------------------------------------------------------------------------
void GameManager::WhenPlayerFalling(int& row, int col,
	bool& is_enemy, bool& is_last_coin, bool is_ladder, bool is_pole)
{// when falling //When player not stepping on a floor
	for (; row < m_size_board; row++)//until floor
	{
		char charToResrore = m_GameBoard[row][col];//save for later
		if (m_GameBoard[row][col] == WALL)
		{
			m_GameBoard[--row][col] = PLAYER;
			PrintBoard();
			return;
		}
		m_GameBoard[row][col] = PLAYER;//update the player location in the board
		m_GameBoard[m_player.getCurrPosPlayer().row][m_player.getCurrPosPlayer().col] = m_player.restoreChar(); 
		//restore char
		m_player.setCurrPosPlayer(row, col);//set
		switch (charToResrore) //which char we need to restore
		{
		case EMPTY_CELL: //if fell on an empty cell
			m_player.pushCharUnder(EMPTY_CELL);
			break;
		case ENEMY: //if fell on an enemy
			m_player.setDecreaseLife();
			is_enemy = true;
			break;
		case COIN: //if fell on a coin
			m_player.pushCharUnder(EMPTY_CELL);
			m_count_coins--;
			m_score += 2 * m_level;
			if (m_count_coins == 0)
				is_last_coin = true;
			break;
		case POLE: //if fell on a pole
			m_player.pushCharUnder(POLE);
		     is_pole = true; 
			 break;
		case LADDER: //if fell on a ladder
			m_player.pushCharUnder(LADDER);
			m_GameBoard[row][col] = PLAYER_ON_SCALE;
			is_ladder = true; 
			break;
		case PLAYER:
			m_player.pushCharUnder(PLAYER);break;
		default: break;
		}
		if (is_pole) break;
		if (is_ladder) break;
		PrintBoard();
	}
}
//--------------------------------------------------------------------------
void GameManager::PrintBoard()
{//Prints the game board
	std::system("cls"); //clean screen
	for (int i = 0; i < m_size_board; i++)
		cout << m_GameBoard[i] << endl;//Print board
	cout << "LEVEL:     \t" << m_level << endl;
	cout << "LIFE:      \t" << m_player.getLife() << endl;
	cout << "SCORE:     \t" << m_score << endl;
	cout << "SIZE BOARD:\t" << m_size_board << endl;
	cout << "Coins left:\t" << m_count_coins << endl;
}
//--------------------------------------------------------------------------
void GameManager::SetStartLevelPosition_andCount()
{//count coins and enemies, find player start position
	m_count_coins = m_count_enemy = 0; //initiate again, in case of restore the level
	for (int row = 0; row < m_size_board; row++)
	{
		for (int col = 0; col < m_size_board; col++)
		{
			if (m_GameBoard[row][col] == COIN)
				m_count_coins++;
			else if (m_GameBoard[row][col] == PLAYER)
			{
				m_player.setStartPosPlayer(row, col);
				m_player.setCurrPosPlayer(row, col);
			}
			else if (m_GameBoard[row][col] == ENEMY)
			{
				Enemies enemy(row, col);
				m_enemies.push_back(enemy);
				m_enemies[m_count_enemy].setStartPosEnemy(row, col);
				m_enemies[m_count_enemy].setCurrPosEnemy(row, col);
				m_count_enemy++;
			}
		}
	}
}
//--------------------------------------------------------------------------
int GameManager::getDirectionEnemy(int enemy)const
{
	int RowP = m_player.getCurrPosPlayer().row;
	int ColP = m_player.getCurrPosPlayer().col;
	int RowE = m_enemies[enemy].getCurrPosEnemy().row;
	int ColE = m_enemies[enemy].getCurrPosEnemy().col;

	if (m_level == 1 && (enemy % 2 == 0))
		return ((rand() % 4) + 1);
	
	if (ColP <= ColE && RowP <= RowE)
	{
		if (MoveOk(KB_UP, RowE-1, ColE))//if wall
			return 1;// up
		else  return 3; //left
	}
	else if (ColP <= ColE && RowP >= RowE)
	{
		if (MoveOk(KB_DOWN, RowE+1, ColE))//if wall
			return 2;//down
		else  return 3;//left
	}
	else if (ColP >= ColE && RowP <= RowE)
	{
		if (MoveOk(KB_RIGHT, RowE, ColE+1))//if wall
			return 4;//right
		else  return 1;//up
	}
	else if (ColP >= ColE && RowP >= RowE)
	{
		if (MoveOk(KB_RIGHT, RowE, ColE+1))//if wall
			return 4;//right
		else  return 2;//down
	}
	else return ((rand() % 4) + 1);
}
//--------------------------------------------------------------------------
void GameManager::LOOSE()
{
	std::system("cls"); //clear screan
	if (m_player.getLife() == 0)// if no life
	{
		cout << endl<<endl<<endl<<"\t \t \t \t \t ------------------------------\n " << endl;
		cout << "\t \t \t   -------------------  YOU LOST!  ---------------------" << endl;
		cout << "\t \t \t ---------------------------------------------------------" << endl;
		cout << "\t \t \t ----------------------GAME OVER!!! ------------------------" << endl;
		return;
	}
	else // if still have life - restart level
		cout << endl << endl << "            Retry Level..." << endl;   getchar();
}
//--------------------------------------------------------------------------
void GameManager::WIN()
{
	std::system("cls"); //clean screen
	cout << endl << endl<< "\t \t \t -----------------------------------------------------" << endl
		<< "\t \t \t ---------------YOU WIN THE LEVEL!!--------------------" << endl
		<< "\t \t \t ------------------------------------------------------" << endl
		<< "\t \t \t                  CONGRATULATION!" << endl << endl << endl
		<< "\t \t \t             Press Enter to next level: " << endl; getchar();
	m_score += 50 * m_level;
	m_level++;
}
//--------------------------------------------------------------------------


















//bool GameManager::WhenEnemyFalling(int enemy, int& row, int col,

	//	bool is_ladder, bool is_pole)
	//{
	//	for (; row < m_size_board; row++)//until floor
	//	{
	//		char charToResrore = m_GameBoard[row][col];
	//		if (m_GameBoard[row][col] == WALL)
	//		{
	//			row--;
	//			m_GameBoard[row][col] = ENEMY;
	//			PrintBoard();
	//			return true;
	//		}
	//		m_GameBoard[row][col] = ENEMY;
	//		//PrintBoard();
	//		m_GameBoard[m_enemies[enemy].getCurrPosEnemy().row][m_enemies[enemy].getCurrPosEnemy().col]
	//			= m_enemies[enemy].restoreChar(); //restore char
	//		m_enemies[enemy].setCurrPosEnemy(row, col);
	//		switch (charToResrore) //which char we need to restore
	//		{
	//		case EMPTY_CELL:
	//			m_enemies[enemy].pushCharUnder(EMPTY_CELL);
	//			break;
	//		case ENEMY:
	//			//is_enemy = true;
	//			break;
	//		case COIN:
	//			m_player.pushCharUnder(COIN);
	//			break;
	//		case POLE:
	//			//m_GameBoard[row][col] = ENEMY;
	//			m_enemies[enemy].pushCharUnder(POLE);
	//			is_pole = true;
	//			break;
	//		case LADDER:
	//			m_enemies[enemy].pushCharUnder(LADDER);
	//			is_ladder = true;
	//			break;
	//		case PLAYER:
	//			m_player.setDecreaseLife();
	//			return false;
	//		default:
	//			break;
	//		}
	//		if (is_pole)
	//		{
	//			//PrintBoard();
	//			break;
	//		}
	//		if (is_ladder)
	//		{
	//			//PrintBoard();
	//			break;
	//		}
	//		PrintBoard();
	//	}
	//	PrintBoard();
	//
	//}

//----------------------------------------------
/*bool GameManager::isLadder(int row, int col)const
{
	return (m_GameBoard[row][col] == LADDER);
}
//--------------------------------------------
bool GameManager::isPole(int row, int col)const
{
	return (m_GameBoard[row][col] == POLE);
}*/
//---------------------------------------------------
/*bool GameManager::isEnemy(int row, int col)const
{
	return (m_GameBoard[row][col] == ENEMY);
}*/
/*bool GameManager::isEmptyCell(int row, int col)
{ //This function checks id the given cell is empty
	return (m_GameBoard[row][col] == EMPTY_CELL); //if cell is empty
}
//-------------------------------------------------
bool GameManager::isCoin(int row, int col)const
{ //This function checks id the given cell has a coin in it
	return (m_GameBoard[row][col] == COIN); // if there is a coin
}*/
////---------------------------------------------------
//int GameManager::getSizeBoard()const
//{
//	return m_size_board;
//}
////--------