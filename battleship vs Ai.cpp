#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

const int SIZE = 5;
const int NUM_SHIPS = 3;

struct Player {
    char grid[SIZE][SIZE];      // What player sees
    bool ships[SIZE][SIZE];     // True ship positions
};

// Initialize the grid with water '~'
void initializeGrid(char grid[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = '~';
}

// Display grid (e.g., for Player's own grid)
void displayGrid(char grid[SIZE][SIZE]) {
    cout << "  0 1 2 3 4" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < SIZE; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

// Display opponent grid (AI) — hides ships
void displayHiddenGrid(char grid[SIZE][SIZE]) {
    cout << "  0 1 2 3 4" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 'S') cout << "~ ";
            else cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

// Place ships randomly
void placeShips(Player &player) {
    int x, y;
    for (int i = 0; i < NUM_SHIPS; i++) {
        bool placed = false;
        while (!placed) {
            x = rand() % SIZE;
            y = rand() % SIZE;
            if (!player.ships[x][y]) {
                player.ships[x][y] = true;
                placed = true;
            }
        }
    }
}

// Handle attack from one player to another
bool attack(Player &attacker, Player &defender, int x, int y) {
    if (defender.ships[x][y]) {
        defender.grid[x][y] = 'X';  // hit
        defender.ships[x][y] = false;
        return true;
    } else {
        defender.grid[x][y] = 'O';  // miss
        return false;
    }
}

// AI makes random attack
bool aiAttack(Player &ai, Player &player) {
    int x, y;
    while (true) {
        x = rand() % SIZE;
        y = rand() % SIZE;
        if (player.grid[x][y] == '~') {
            return attack(ai, player, x, y);
        }
    }
}

// Check if all ships of a player are sunk
bool allShipsSunk(Player &player) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (player.ships[i][j])
                return false;
    return true;
}

// Main game function
void playGame() {
    srand(time(0));

    Player player1, ai;
    initializeGrid(player1.grid);
    initializeGrid(ai.grid);
    memset(player1.ships, false, sizeof(player1.ships));
    memset(ai.ships, false, sizeof(ai.ships));

    cout << "Placing ships for Player 1..." << endl;
    placeShips(player1);
    cout << "Placing ships for AI..." << endl;
    placeShips(ai);

    bool gameOver = false;
    int turn = 1;

    while (!gameOver) {
        cout << "\n========== Turn " << turn << " ==========\n";

        cout << "\n--- Player 1's Grid ---" << endl;
        displayGrid(player1.grid);

        cout << "--- AI's Grid (Hidden) ---" << endl;
        displayHiddenGrid(ai.grid);

        // Player 1's turn
        int x, y;
        cout << "\nPlayer 1, enter coordinates to attack (x y): ";
        cin >> x >> y;

        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            cout << "Invalid coordinates. Try again." << endl;
            continue;
        }

        if (ai.grid[x][y] == 'X' || ai.grid[x][y] == 'O') {
            cout << "You've already attacked this location. Try again." << endl;
            continue;
        }

        if (attack(player1, ai, x, y)) {
            cout << "Player 1 hit AI's ship!" << endl;
        } else {
            cout << "Player 1 missed." << endl;
        }

        if (allShipsSunk(ai)) {
            cout << "\nPlayer 1 wins! AI has no ships left!" << endl;
            gameOver = true;
            break;
        }

        // AI's turn
        cout << "\nAI is attacking..." << endl;
        if (aiAttack(ai, player1)) {
            cout << "AI hit Player 1's ship!" << endl;
        } else {
            cout << "AI missed." << endl;
        }

        if (allShipsSunk(player1)) {
            cout << "\nAI wins! Player 1 has no ships left!" << endl;
            gameOver = true;
            break;
        }

        turn++;
    }

    // Save results
    ofstream resultFile("game_results.txt");
    resultFile << "Game Over!" << endl;
    resultFile << (allShipsSunk(player1) ? "AI wins!" : "Player 1 wins!") << endl;
    resultFile.close();

    cout << "\nGame results saved to game_results.txt." << endl;
}

int main() {
    playGame();
    return 0;
}