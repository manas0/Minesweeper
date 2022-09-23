#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <string>
#include <queue>
using namespace std;

class Minesweeper
{
    private:
        int length, breadth, numMines, initX, initY, numRevealedCells=0, numCells;
        string gameOverMessage;
        bool isGameOver = false;
        vector<vector<pair<int, int> > > board;
        /*
        board[x][y] is a pair. It stores two things:
        1. board[x][y].first stores the number of mines among the neighboring 8 cells.
        2. board[x][y] shows the status of the cell. These can be:
            0 -> Normal hidden cell
            1 -> Revealed unbombed cell
            2 -> Flag the cell
            3 -> Mark the cell as Question mark
        */

        void printBoard()
        {
            printf("\n");
            for(int i = -1 ; i < breadth ; i++) {
                if(i == -1) {
                    printf("%4s", " ");
                    continue;
                }
                printf("%4d", i);
            }
            printf("\n\n");
            for(int i = 0; i < length; i++)
            {
                printf("%4d", i);
                for(int j = 0; j < breadth; j++)
                {
                    if(board[i][j].second == 0)
                        printf("%4s", "_");
                    else if(board[i][j].second == 1)
                        printf("%4d", board[i][j].first);
                    else if(board[i][j].second == 2)
                        printf("%4s", "F");
                    else if(board[i][j].second == 3)
                        printf("%4s", "Q");
                }
                printf("\n\n");
            }
            printf("\n\n");
        }

        void selectInitialPoint()
        {
            printf("Enter coordinates of Initial Point:");
            scanf("%d %d", &initX, &initY);
            setUpPlayingBoard();
            selectCellAndRevealRelevantCells(initX, initY);
        }

        void setUpMineOnCellAndUpdateNeighboringCells(int x, int y)
        {
            board[x][y].first = -1;
            board[x][y].second = 0;
            if(x>0)
            {
                
                if(board[x-1][y].first != -1) board[x-1][y].first++;
                if(y > 0)
                {
                    if(board[x-1][y-1].first != -1) board[x-1][y-1].first++;
                }
                if(y < breadth-1)
                {
                    if(board[x-1][y+1].first != -1) board[x-1][y+1].first++;
                }
            }
            if(x < length-1)
            {
                if(board[x+1][y].first != -1) board[x+1][y].first++;
                if(y>0)
                {
                    if(board[x+1][y-1].first != -1) board[x+1][y-1].first++;
                }
                if(y < breadth-1)
                {
                    if(board[x+1][y+1].first != -1) board[x+1][y+1].first++;
                }
            }
            if(y > 0)
            {
                if(board[x][y-1].first != -1) board[x][y-1].first++;
            }
            if(y < breadth - 1)
            {
                if(board[x][y+1].first != -1) board[x][y+1].first++;
            }
        }

        void randomlySetUpMines()
        {
            uint countMines = 0;
            int x,y;
            srand((unsigned)time(0)); 
            while(countMines < numMines)
            {
                x = rand()%length;
                y = rand()%breadth;
                if(abs(x-initX) > 1 || abs(y-initY) > 1)
                {
                    if(board[x][y].first != -1)
                    {
                        setUpMineOnCellAndUpdateNeighboringCells(x, y);
                        countMines++;
                    }
                }
            }
        }

        void setUpPlayingBoard()
        {
            randomlySetUpMines();
        }

        void selectCellAndRevealRelevantCells(int x, int y) {
            queue<pair<int,int> > revealCells;
            revealCells.push(make_pair(x,y));
            while(!revealCells.empty()) {
                pair<int,int> cell = revealCells.front();
                revealCells.pop();
                if(board[cell.first][cell.second].second != 1) {
                    board[cell.first][cell.second].second = 1;
                    numRevealedCells++;
                    int newX, newY; 
                    if(board[cell.first][cell.second].first == 0) {
                        for(int i = -1 ; i <= 1 ; i++) {
                            for(int j = -1 ; j <= 1 ; j++) {
                                if(i == 0 && j == 0) {
                                    continue;
                                }
                                newX = cell.first + i;
                                newY = cell.second + j;
                                if(newX < 0 || newX == length || newY < 0 || newY == breadth) {
                                    continue;
                                }
                                revealCells.push(make_pair(newX, newY));
                            }
                        }
                    }
                }

            }
            checkIfGameOver();
        }

        void checkIfGameOver() {
            if(numRevealedCells + numMines == numCells)
            {
                gameOverMessage = "Game Won";
                isGameOver = true;
            }
        }

        void makeMove() 
        {
            printf("Make a move:\n");
            char move;
            uint x, y;
            scanf("%c %u %u", &move, &x, &y);
            if(move == 'S')
            {
                if(board[x][y].first == -1)
                {
                    isGameOver = true;
                    gameOverMessage = "You selected a mine. Game Lost";
                }
                else if(board[x][y].second != 1)
                {
                    selectCellAndRevealRelevantCells(x,y);
                }
            }
            else if(move == 'F' && board[x][y].second != 1)
            {
                board[x][y].second = 2;
            }
            else if(move == 'Q' && board[x][y].second != 1)
            {
                board[x][y].second = 3;
            }
            checkIfGameOver();
        }

    public:
        Minesweeper(uint l=10, uint b=10, uint m=10)
        {
            length = l;
            breadth = b;
            numMines = m;
            numCells = length*breadth;
            for(int i = 0; i < length; i++)
            {
                vector<pair<int,int> > row(breadth);
                for(int j = 0; j < breadth; j++)
                {
                    row[j] = make_pair(0, 0);
                }
                board.push_back(row);
            }
        }

        void play()
        {
            printBoard();
            selectInitialPoint();
            printBoard();
            do
            {
                makeMove();
                printBoard();
            }while(!isGameOver);
            printf("%s\n",gameOverMessage.c_str());
        }
};

int main()
{
    bool playAgain = false;
    do {
        Minesweeper mGame(8, 8, 10);
        mGame.play();
        printf("Press y to play again, else press any other key: ");
        char playAgainInput;
        scanf("%c", &playAgainInput);
        playAgain = (playAgainInput == 'y' || playAgainInput == 'Y');
    } while(playAgain);
}