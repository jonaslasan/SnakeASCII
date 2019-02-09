#include <list>
#include <vector>
#include <random>
#include <string>
#include <Windows.h>
#include <iostream>
#include "Graphics.h"
#include "InputManager.h"
#include "Game.h"


#define WIDTH 50
#define HEIGHT 25

#define START_X 2
#define START_Y 2

#define START_SIZE 3

#define WALL '#'
#define SNAKE '0'
#define APPLE '@'

#define SCORE_BOARD_WIDTH 30
#define SCORE_BOARD_HEIGHT 5

#define SCORE_LOCATION_Y 3
#define SCORE_LOCATION_X 9

using namespace std;

class Point
{

public:
	int x;
	int y;

	Point(int posX, int posY)
	{
		x = posX;
		y = posY;
	}
};

// --Variables--

int map[WIDTH][HEIGHT];
Point startPoint = Point(START_X, START_Y);

char ScoreBoard[SCORE_BOARD_HEIGHT][SCORE_BOARD_WIDTH]{
	"|===========================|",
	"| Author: Jonas Lasan       |",
	"|===========================|",
	"| Score:                    |",
	"|===========================|",
};

// Head position
int snakeX;
int snakeY;

int score;

// Apple position, first coordinates are random
int appleX;
int appleY;

int lookDirection;
int nextLookDirecton;

bool gameOver;
bool won = false;

vector<Point> body;

// Initialize game state
void Initialize()
{
	snakeX = START_X;
	snakeY = START_Y;

	score = 0;

	newApple();

	lookDirection = 2;
	nextLookDirecton = 2;

	gameOver = false;

	body.clear();

	// Spawn desired number of body parts in same spot
	// Creates an effect that snake is "growing out" of it
	for (int j = 0; j < START_SIZE; j++)
	{
		body.push_back(startPoint);
	}

	ShowMenu();

}

void ShowMenu()
{
	// Initial render
	Render();

	if (won)
	{
		ShowPopup("YOU WON! You deserve a break :)");
	}
	else
	{
		ShowPopup("Press enter to start");
	}

	won = false;

	// Wait for key pressed
	cin.get();

	// Start game loop
	StartGame();
}

void ShowPopup(string text)
{
	// Render pop-up menu
	const int width = 40;
	const int height = 5;
	int corner_x = (WIDTH / 2) - (width / 2);
	int corner_y = (HEIGHT / 2) - height;

	char menu[height][width]{
		"|=====================================|",
		"|                                     |",
		"|                                     |",
		"|                                     |",
		"|=====================================|",
	};

	for (int y = 0; y < height; y++)
	{
		setCursorPosition(corner_x, corner_y + y);

		for (int x = 0; x < width; x++)
		{
			cout << menu[y][x];
		}
	}

	int textOffset = (width - text.length()) / 2;

	setCursorPosition(corner_x+ textOffset, corner_y + (height / 2));
	cout << text;
}

void StartGame()
{
	GameLoop();
	Initialize();
}


void GameLoop()
{
	double startMoveTime, startRenderTime;

	startMoveTime = startRenderTime = GetTickCount();


	while (!gameOver && !won)
	{
		double moveDelta = GetTickCount() - startMoveTime;
		double renderDelta = GetTickCount() - startRenderTime;

		if (moveDelta >= 100)
		{
			Move();
			//Reset the timer.
			startMoveTime = GetTickCount();
		}

		if (renderDelta >= 50) // 0.1 sec
		{
			Render();
			//Reset the timer.
			startRenderTime = GetTickCount();
		}

		using namespace Input;

		// Handle key input
		if (isUpPressed())
			setDirection(0);

		if (isDownPressed())
			setDirection(1);

		if (isRightPressed())
			setDirection(2);

		if (isLeftPressed())
			setDirection(3);
	}

}

void newApple()
{
	appleX = rand() % (WIDTH - 2) + 1;
	appleY = rand() % (HEIGHT - 2) + 1;
}

void setDirection(int newDirection)
{
	// Can't go in reverse
	// We can only rotate right or left relative to current rotation
	if ((lookDirection == 1 && newDirection == 0) || (lookDirection == 0 && newDirection == 1) )
		return;

	if ((lookDirection == 2 && newDirection == 3) || (lookDirection == 3 && newDirection == 2))
		return;

	nextLookDirecton = newDirection;
}

void Move()
{
	lookDirection = nextLookDirecton;
	switch (lookDirection)
	{
	case 0:
		snakeY -= 1;
		break;
	case 1:
		snakeY += 1;
		break;
	case 2:
		snakeX += 1;
		break;
	case 3:
		snakeX -= 1;
		break;
	}

	// We hit an apple
	if (snakeX == appleX && snakeY == appleY)
	{
		newApple();

		score++;
	}
	else // Delete tail
	{
		body.erase(body.begin());
	}

	// We hit a wall
	if (snakeX == 0 || snakeX == (WIDTH - 1) || snakeY == 0 || snakeY == (HEIGHT - 1))
	{
		gameOver = true;
	}

	// We hit ourselves
	for (Point bodyPart : body)
	{
		if (snakeX == bodyPart.x && snakeY == bodyPart.y)
			gameOver = true;
	}

	// If snake size = surface of the map --> i guess you won
	if (body.size() == (HEIGHT - 2) * (WIDTH - 2))
	{
		won = true;
	}

	Point newPoint = Point(snakeX, snakeY);
	body.push_back(newPoint);
}

void Render()
{
	char Canvas[HEIGHT][WIDTH];

	GenerateScoreBoard();

	// Draw background and walls
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			// Border
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
			{
				// Wall
				Canvas[y][x] = WALL;
			}
			else
			{
				Canvas[y][x] = ' ';
			}
		}
	}

	//Draw apple
	Canvas[appleY][appleX] = APPLE;

	// Draw snake
	for (Point body_part : body)
	{
		// Head represented as arrows
		if (body_part.x == snakeX && body_part.y == snakeY)
		{
			switch (lookDirection)
			{
				case 0:
					Canvas[body_part.y][body_part.x] = '^';
					break;
				case 1:
					Canvas[body_part.y][body_part.x] = 'v';
					break;
				case 2:
					Canvas[body_part.y][body_part.x] = '>';
					break;
				case 3:
					Canvas[body_part.y][body_part.x] = '<';
					break;
			}
		}
		else
		{
			Canvas[body_part.y][body_part.x] = SNAKE;
		}
	}

	// Draw to screen
	setCursorPosition(0, 0);
	ShowConsoleCursor(false);

	for (int y = 0; y < HEIGHT; y++)
	{
		// Draw line for game graphics
		for (int x = 0; x < WIDTH; x++)
		{
			cout << Canvas[y][x];
		}

		// Draw line for score board
		if (y < SCORE_BOARD_HEIGHT)
		{
			for (int x = 0; x < SCORE_BOARD_WIDTH; x++)
			{
				cout << ScoreBoard[y][x];
			}
		}

		// Spacing
		cout << "  ";

		// Draw line for 
		cout << endl;


	}
}

Point scoreLocation = Point(-1,-1);

void GenerateScoreBoard()
{
	string scoreLabel = std::to_string(score);

	// Cleanup because digits can be left on from previous games
	for (int index = SCORE_LOCATION_X; index < SCORE_BOARD_WIDTH - 2; index++)
	{
		ScoreBoard[SCORE_LOCATION_Y][index] = ' ';
	}

	for (int index = 0; index < scoreLabel.length(); index++)
	{
		ScoreBoard[SCORE_LOCATION_Y][SCORE_LOCATION_X + index] = scoreLabel.at(index);
	}
}

