#include <iostream>    // For input/output operations
#include <conio.h>     // For _getch() and _kbhit() functions to handle keyboard input
#include <windows.h>   // For Windows-specific functions like Sleep() and SetConsoleCursorPosition()
#include <vector>      // For using the vector container to manage dynamic arrays
#include <fstream>     // For file handling operations like reading/writing to files


using namespace std;

// Function to set the cursor position in the console
void gotoxy(int x, int y) {
    COORD coord;  // Coordinate structure
    coord.X = x;  // X-coordinate (column)
    coord.Y = y;  // Y-coordinate (row)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);  // Move cursor to specified position
}

// Class to handle login and user-related operations
class loginPage {
public:
    string username, password, id, pass;
    int userHighScore;

    // Function to display the main menu
    void menu() {
        system("cls");  // Clear the screen
        int c;
        gotoxy(60, 15);  // Set cursor position
        cout << "1. Login with your existing ID\n";
        gotoxy(60, 17);
        cout << "2. Create your new ID\n";
        gotoxy(60, 19);
        cout << "3. Exit\n";
        gotoxy(60, 21);
        cout << "Enter your choice: ";
        cin >> c;
        switch (c) {
            case 1:
                login();
                break;
            case 2:
                createId();
                break;
            case 3:
                exitProgram();
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                menu();
                break;
        }
    }

    // Function to handle user login
    void login() {
        int count = 0;
        system("cls");  // Clear the screen
        gotoxy(60, 15);
        cout << "Enter your ID: ";
        cin >> username;

        gotoxy(60, 17);
        cout << "Enter your password: ";
        cin >> password;

        // Open file to verify credentials
        ifstream input("record.txt");
        while (input >> id >> pass >> userHighScore) {
            if (id == username && pass == password) {
                system("cls");
                gotoxy(60, 19);
                cout << "Login successful....";
                Sleep(1000);
                system("cls");
                gotoxy(60, 19);
                cout << "Game is starting...please wait";
                Sleep(3000);
                input.close();
                count = 1;
                break;
            }
        }
        input.close();
        if (count == 0) {
            gotoxy(60, 19);
            cout << "Invalid ID or password.";
            getch();  // Wait for user to press a key
            menu();
        }
    }

    // Function to create a new user ID
    void createId() {
        bool idExists = false;

        do {
            system("cls");  // Clear the screen
            idExists = false;
            gotoxy(60, 15);
            cout << "Enter your new username: ";
            cin >> username;
            gotoxy(60, 17);
            cout << "Enter your new password: ";
            cin >> password;

            // Check if ID already exists
            ifstream input("record.txt");
            while (input >> id >> pass >> userHighScore) {
                if (id == username) {
                    gotoxy(60, 19);
                    cout << "This ID already exists...please create another ID\n";
                    getch();  // Wait for user to press a key
                    idExists = true;
                    break;
                }
            }
            input.close();

            // Create new ID if it doesn't exist
            if (!idExists) {
                ofstream f1("record.txt", ios::app);
                userHighScore = 0;
                f1 << username << " " << password << " " << userHighScore << endl;
                f1.close();
                gotoxy(60, 19);
                cout << "Your ID is created successfully";
                Sleep(1500);
            }

        } while (idExists);

        menu();  // Return to the menu after ID creation
    }

    // Function to exit the program
    void exitProgram() {
        system("cls");  // Clear the screen
        gotoxy(60, 19);
        cout << "Thank you for playing our game :)";
        gotoxy(60, 25);
        cout << "Created by: ";
        gotoxy(60, 26);
        cout << "1.Homraj K.C. (PUL080BCT032)";
        gotoxy(60, 27);
        cout << "2.Avinash Yadav (PUL080BCT018)";
        gotoxy(60, 28);
        cout << "3.Abhishek Tharu (PUL080BCT008)";
        gotoxy(60, 29);
        cout << "4.Bibek Gautam (PUL080BCT020)";
        getch();  // Wait for user to press a key
        exit(0);  // Exit the program
    }
};

// Class to handle the game logic
class Game {
private:
    int t, choice;
    bool gameOver;
    const int width;
    const int height;
    int x, y, fruitX, fruitY;
    vector<int> tailX, tailY;  // To store tail coordinates
    int nTail;
    int score;
    string globalHighScorer;
    int globalHighScore;
    loginPage *lp;  // Pointer to loginPage object
    loginPage lp2;  // Another loginPage object
    enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };  // Enum for direction
    eDirection dir;

public:
    int m = 0;

    // Constructor to initialize the game
    Game(int w, int h, loginPage* lp) : width(w), height(h), gameOver(false), nTail(0), score(0), dir(STOP), lp(lp) {
        globalHighScore = 0;
        globalHighScorer = "";
        loadGlobalHighScore();
    }

    // Function to select difficulty level
    void difficulty() {
        system("cls");  // Clear the screen
        gotoxy(60, 15);
        cout << "Select your difficulty\n\n";
        gotoxy(60, 18);
        cout << "1. Easy\n";
        gotoxy(60, 20);
        cout << "2. Medium\n";
        gotoxy(60, 22);
        cout << "3. Hard\n";
        gotoxy(60, 24);
        cout << "Your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                t = 300;  // Slow speed
                break;
            case 2:
                t = 100;  // Medium speed
                break;
            case 3:
                t = 10;  // Fast speed
                break;
        }
    }

    // Function to set up the initial game state
    void Setup() {
        gameOver = false;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        fruitX = rand() % width;
        fruitY = rand() % height;
        score = 0;
        nTail = 0;
        tailX.clear();
        tailY.clear();
    }

    // Function to draw the game elements on the screen
    void Draw() {
        system("cls");  // Clear the screen
        for (int i = 0; i < width; i++) {
            cout << "#";  // Top border
        }
        cout << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0 || j == width - 1) {
                    cout << "#";  // Side borders
                } else if (i == y && j == x) {
                    cout << "O";  // Snake head
                } else if (i == fruitY && j == fruitX) {
                    cout << "F";  // Fruit
                } else {
                    bool print = false;
                    for (int k = 0; k < nTail; k++) {
                        if (i == tailY[k] && j == tailX[k]) {
                            cout << "O";  // Snake tail
                            print = true;
                        }
                    }
                    if (!print) {
                        cout << " ";  // Empty space
                    }
                }
            }
            cout << endl;
        }

        for (int i = 0; i < width; i++) {
            cout << "#";  // Bottom border
        }
        cout << endl;

        cout << "\t\t\tScore: " << score << endl;
        gotoxy(60, 30);
        cout << "Current User: " << lp->username << " | High Score: " << lp->userHighScore << endl;
        gotoxy(60, 32);
        cout << "Global High Score: " << globalHighScore << " by " << globalHighScorer << endl;
    }

    // Function to handle user input
    void Input() {
        if (_kbhit()) {  // Check if a key is pressed
            switch (_getch()) {
                case 'w':
                    dir = UP;
                    break;
                case 'a':
                    dir = LEFT;
                    break;
                case 'd':
                    dir = RIGHT;
                    break;
                case 's':
                    dir = DOWN;
                    break;
                default:
                    break;
            }
        }
    }

    // Function to update the game logic
    void Logic() {
        int prevX = x;
        int prevY = y;

        switch (dir) {
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            default:
                break;
        }

        // Check if snake hits the wall
        if (x <= 0 || x >= width - 1 || y < 0 || y > height) {
            gameOver = true;
        }

        // Move the tail
        for (int i = nTail - 1; i > 0; i--) {
            tailX[i] = tailX[i - 1];
            tailY[i] = tailY[i - 1];
        }

        if (nTail > 0) {
            tailX[0] = prevX;
            tailY[0] = prevY;
        }

        // Check if snake hits its own tail
        for (int i = 0; i < nTail; i++) {
            if (x == tailX[i] && y == tailY[i]) {
                gameOver = true;
            }
        }

        // Check if snake eats the fruit
        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % (width - 2) + 1;
            fruitY = rand() % (height - 2) + 1;
            nTail++;
            tailX.push_back(0);
            tailY.push_back(0);
        }
    }

    // Function to check if the game is over
    bool isGameOver() const {
        return gameOver;
    }

    // Function to load the global high score from a file
    void loadGlobalHighScore() {
        ifstream file("highscore.txt");
        if (file.is_open()) {
            file >> globalHighScorer >> globalHighScore;
            file.close();
        }
    }

    // Function to update the high scores
    void updateHighScores() {
        if (score > lp->userHighScore) {
            lp->userHighScore = score;
            updateUserHighScore();
        }

        if (score > globalHighScore) {
            globalHighScore = score;
            globalHighScorer = lp->username;
            saveGlobalHighScore();
        }
    }

    // Function to update the user's high score in the record file
    void updateUserHighScore() {
        ifstream input("record.txt");
        ofstream output("temp.txt");
        string tempId, tempPass;
        int tempHighScore;

        while (input >> tempId >> tempPass >> tempHighScore) {
            if (tempId == lp->username) {
                output << tempId << " " << tempPass << " " << lp->userHighScore << endl;
            } else {
                output << tempId << " " << tempPass << " " << tempHighScore << endl;
            }
        }

        input.close();
        output.close();
        remove("record.txt");  // Delete the old record file
        rename("temp.txt", "record.txt");  // Rename the temp file to record.txt
    }

    // Function to save the global high score to a file
    void saveGlobalHighScore() {
        ofstream file("highscore.txt");
        file << globalHighScorer << " " << globalHighScore;
        file.close();
    }

    // Function to run the game loop
    void Run() {
        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(t);  // Control the speed of the game based on difficulty
        }

        getch();  // Wait for user input after game over
        system("cls");  // Clear the screen
        gotoxy(50, 15);
        cout << "******      *     *      *  ******                ******  *       *  ******  ******\n";
        gotoxy(50, 16);
        cout << "*          * *    * *  * *  *                     *    *   *     *   *       *    *\n";
        gotoxy(50, 17);
        cout << "* ****    *****   *  *   *  ******                *    *    *   *    ******  ******\n";
        gotoxy(50, 18);
        cout << "*    *   *     *  *      *  *                     *    *     * *     *       *  *  \n";
        gotoxy(50, 19);
        cout << "******  *       * *      *  ******                ******      *      ******  *    * ";
        gotoxy(60, 22);
        cout << "Your final score is: " << score << endl;
        updateHighScores();  // Update the high scores after game over
        gotoxy(60, 24);
        cout << "Press any key to continue";
        getch();  // Wait for user input

        // After game over, present options to the user
        system("cls");
        gotoxy(60, 15);
        cout << "Choose your choice";
        gotoxy(60, 18);
        cout << "1. Play again";
        gotoxy(60, 20);
        cout << "2. Go to login page";
        gotoxy(60, 22);
        cout << "3. Exit";
        gotoxy(60, 24);
        cout << "Enter your choice: ";
        cin >> m;
        switch (m) {
            case 1:
                Setup();  // Setup the game again
                Run();  // Run the game again
                break;
            case 2:
                break;  // Return to the login page
            case 3:
                lp2.exitProgram();  // Exit the program
                break;
        }
    }
};

int main() {
    while (true) {
        loginPage lp;  // Create a loginPage object
        lp.menu();  // Show the menu and handle login/ID creation

        Game game(50, 20, &lp);  // Create a Game object with the specified width and height
        game.Setup();  // Setup the game
        game.difficulty();  // Set the difficulty level
        game.Run();  // Run the game

        if (game.m == 2) {
            continue;  // Go back to the login menu
        } else if (game.m != 2) {
            break;  // Exit the game loop
        }
    }
    return 0;
}
