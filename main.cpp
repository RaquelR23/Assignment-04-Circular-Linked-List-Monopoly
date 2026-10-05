#include <SDL2/SDL.h>  // Used to create the game window and draw graphics.
#include <cmath>       // Used for sin(), cos(), and calculating board positions.
#include <cstdlib>     // Used for rand() and srand().
#include <ctime>       // Used to generate a random seed for the dice.
#include <iostream>    // Used for console input and output.
#include <string>      // Used to work with text.

using namespace std;

// ---------------- GAME SETTINGS ----------------

// Number of spaces on the circular game board.
const int BOARD_SIZE = 18;

// Width and height of the SDL2 game window.
const int WINDOW_SIZE = 900;


// Each node represents one space on the game board.
// The nodes are connected using pointers to form a circular linked list.
struct Node {
    string name;  // Name of the board space.
    int cost;     // Money paid or received on this space.
    int owner;    // -1 means unowned, 0 means player, 1 means computer.
    int type;     // Number that determines the space's effect.
    Node* next;   // Pointer to the next space on the board.
};


// List of pets the player can choose from at the beginning.
string pets[5] = {
    "Golden Retriever",
    "Dalmatian",
    "Gray Tabby Cat",
    "Chinchilla",
    "White and Gray Rabbit"
};


// ---------------- LINKED LIST FUNCTIONS ----------------

// Adds a new node to the circular linked list.
// Each new node is added to the end of the board.
Node* addNode(Node* head, string name, int cost, int type) {

    // New spaces start with no owner (-1).
    Node* newNode = new Node{name, cost, -1, type, nullptr};

    // If the list is empty, the new node points to itself.
    // This creates a circular list with one node.
    if (head == nullptr) {
        newNode->next = newNode;
        return newNode;
    }

    // Find the last node in the circular list.
    Node* temp = head;

    while (temp->next != head) {
        temp = temp->next;
    }

    // Connect the last node to the new node.
    // Then connect the new node back to the head.
    temp->next = newNode;
    newNode->next = head;

    return head;
}


// Searches the circular linked list for a space by name.
// Returns a pointer to the matching node, or nullptr if not found.
Node* searchNode(Node* head, string name) {

    // An empty list has nothing to search.
    if (head == nullptr) return nullptr;

    Node* temp = head;

    // A do-while loop allows the head node to be checked first.
    // The loop stops after returning to the beginning.
    do {
        if (temp->name == name) return temp;

        temp = temp->next;

    } while (temp != head);

    return nullptr;
}


// Prints all spaces on the board in the console.
// This is useful for checking the linked list.
void printList(Node* head) {

    if (head == nullptr) return;

    Node* temp = head;
    int index = 0;

    cout << "\n--- GAME BOARD SPACES ---\n";

    // Visit each node once and print its information.
    do {
        cout << index << ". " << temp->name
             << " | Cost: $" << temp->cost
             << " | Type: " << temp->type << endl;

        temp = temp->next;
        index++;

    } while (temp != head);
}


// Removes a node from the circular linked list by name.
// Returns the updated head because the first node might be removed.
Node* removeNode(Node* head, string name) {

    if (head == nullptr) return nullptr;

    Node* current = head;
    Node* previous = nullptr;

    // Search for the node that needs to be removed.
    do {
        if (current->name == name) {

            // Special case: the list contains only one node.
            if (current->next == current) {
                delete current;
                return nullptr;
            }

            // If the first node is being removed, find the last node
            // so the circular connection can be updated.
            if (previous == nullptr) {
                Node* last = head;

                while (last->next != head) {
                    last = last->next;
                }

                // Move the head to the next node.
                head = current->next;

                // Make the last node point to the new head.
                last->next = head;

            } else {
                // Skip the current node to remove it from the list.
                previous->next = current->next;
            }

            // Free the memory used by the removed node.
            delete current;

            return head;
        }

        // Move to the next node while keeping track of the previous one.
        previous = current;
        current = current->next;

    } while (current != head);

    // If the name was not found, return the original list.
    return head;
}


// Deletes every node in the circular linked list.
// This prevents dynamically allocated memory from being left behind.
void deleteList(Node* head) {

    if (head == nullptr) return;

    // Find the last node and temporarily break the circular connection.
    Node* last = head;

    while (last->next != head) {
        last = last->next;
    }

    last->next = nullptr;

    // The list is now linear, so each node can be deleted safely.
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}


// ---------------- PIXEL FONT ----------------

// Defines a simple 5-by-5 pixel pattern for a character.
// A '1' represents a pixel that should be drawn.
// A '0' represents an empty pixel.
bool glyph(char c, int x, int y) {
    string rows[5];

    // Each character is represented by five rows of five pixels.
    // This custom font allows the game to display text without
    // loading an external font file.
    switch (c) {
        case 'A': rows[0]="01110"; rows[1]="10001"; rows[2]="11111"; rows[3]="10001"; rows[4]="10001"; break;
        case 'B': rows[0]="11110"; rows[1]="10001"; rows[2]="11110"; rows[3]="10001"; rows[4]="11110"; break;
        case 'C': rows[0]="01111"; rows[1]="10000"; rows[2]="10000"; rows[3]="10000"; rows[4]="01111"; break;
        case 'D': rows[0]="11110"; rows[1]="10001"; rows[2]="10001"; rows[3]="10001"; rows[4]="11110"; break;
        case 'E': rows[0]="11111"; rows[1]="10000"; rows[2]="11110"; rows[3]="10000"; rows[4]="11111"; break;
        case 'F': rows[0]="11111"; rows[1]="10000"; rows[2]="11110"; rows[3]="10000"; rows[4]="10000"; break;
        case 'G': rows[0]="01111"; rows[1]="10000"; rows[2]="10111"; rows[3]="10001"; rows[4]="01111"; break;
        case 'H': rows[0]="10001"; rows[1]="10001"; rows[2]="11111"; rows[3]="10001"; rows[4]="10001"; break;
        case 'I': rows[0]="11111"; rows[1]="00100"; rows[2]="00100"; rows[3]="00100"; rows[4]="11111"; break;
        case 'J': rows[0]="00111"; rows[1]="00010"; rows[2]="00010"; rows[3]="10010"; rows[4]="01100"; break;
        case 'K': rows[0]="10001"; rows[1]="10010"; rows[2]="11100"; rows[3]="10010"; rows[4]="10001"; break;
        case 'L': rows[0]="10000"; rows[1]="10000"; rows[2]="10000"; rows[3]="10000"; rows[4]="11111"; break;
        case 'M': rows[0]="10001"; rows[1]="11011"; rows[2]="10101"; rows[3]="10001"; rows[4]="10001"; break;
        case 'N': rows[0]="10001"; rows[1]="11001"; rows[2]="10101"; rows[3]="10011"; rows[4]="10001"; break;
        case 'O': rows[0]="01110"; rows[1]="10001"; rows[2]="10001"; rows[3]="10001"; rows[4]="01110"; break;
        case 'P': rows[0]="11110"; rows[1]="10001"; rows[2]="11110"; rows[3]="10000"; rows[4]="10000"; break;
        case 'Q': rows[0]="01110"; rows[1]="10001"; rows[2]="10001"; rows[3]="10011"; rows[4]="01111"; break;
        case 'R': rows[0]="11110"; rows[1]="10001"; rows[2]="11110"; rows[3]="10010"; rows[4]="10001"; break;
        case 'S': rows[0]="01111"; rows[1]="10000"; rows[2]="01110"; rows[3]="00001"; rows[4]="11110"; break;
        case 'T': rows[0]="11111"; rows[1]="00100"; rows[2]="00100"; rows[3]="00100"; rows[4]="00100"; break;
        case 'U': rows[0]="10001"; rows[1]="10001"; rows[2]="10001"; rows[3]="10001"; rows[4]="01110"; break;
        case 'V': rows[0]="10001"; rows[1]="10001"; rows[2]="10001"; rows[3]="01010"; rows[4]="00100"; break;
        case 'W': rows[0]="10001"; rows[1]="10001"; rows[2]="10101"; rows[3]="11011"; rows[4]="10001"; break;
        case 'X': rows[0]="10001"; rows[1]="01010"; rows[2]="00100"; rows[3]="01010"; rows[4]="10001"; break;
        case 'Y': rows[0]="10001"; rows[1]="01010"; rows[2]="00100"; rows[3]="00100"; rows[4]="00100"; break;
        case 'Z': rows[0]="11111"; rows[1]="00010"; rows[2]="00100"; rows[3]="01000"; rows[4]="11111"; break;

        // Pixel patterns for numbers.
        case '0': rows[0]="01110"; rows[1]="10011"; rows[2]="10101"; rows[3]="11001"; rows[4]="01110"; break;
        case '1': rows[0]="00100"; rows[1]="01100"; rows[2]="00100"; rows[3]="00100"; rows[4]="01110"; break;
        case '2': rows[0]="01110"; rows[1]="10001"; rows[2]="00010"; rows[3]="00100"; rows[4]="11111"; break;
        case '3': rows[0]="11110"; rows[1]="00001"; rows[2]="01110"; rows[3]="00001"; rows[4]="11110"; break;
        case '4': rows[0]="10010"; rows[1]="10010"; rows[2]="11111"; rows[3]="00010"; rows[4]="00010"; break;
        case '5': rows[0]="11111"; rows[1]="10000"; rows[2]="11110"; rows[3]="00001"; rows[4]="11110"; break;
        case '6': rows[0]="01110"; rows[1]="10000"; rows[2]="11110"; rows[3]="10001"; rows[4]="01110"; break;
        case '7': rows[0]="11111"; rows[1]="00010"; rows[2]="00100"; rows[3]="01000"; rows[4]="01000"; break;
        case '8': rows[0]="01110"; rows[1]="10001"; rows[2]="01110"; rows[3]="10001"; rows[4]="01110"; break;
        case '9': rows[0]="01110"; rows[1]="10001"; rows[2]="01111"; rows[3]="00001"; rows[4]="01110"; break;

        // Pixel patterns for symbols used by the game.
        case '$': rows[0]="01111"; rows[1]="10100"; rows[2]="01110"; rows[3]="00101"; rows[4]="11110"; break;
        case '!': rows[0]="00100"; rows[1]="00100"; rows[2]="00100"; rows[3]="00000"; rows[4]="00100"; break;
        case '-': rows[0]="00000"; rows[1]="00000"; rows[2]="11111"; rows[3]="00000"; rows[4]="00000"; break;
        case ':': rows[0]="00000"; rows[1]="00100"; rows[2]="00000"; rows[3]="00100"; rows[4]="00000"; break;
        case '+': rows[0]="00000"; rows[1]="00100"; rows[2]="11111"; rows[3]="00100"; rows[4]="00000"; break;
        case ' ': rows[0]="00000"; rows[1]="00000"; rows[2]="00000"; rows[3]="00000"; rows[4]="00000"; break;

        // Unsupported characters are displayed as blank pixels.
        default:
            for (int i = 0; i < 5; i++) rows[i] = "00000";
    }

    // Return true if the selected pixel should be filled.
    return rows[y][x] == '1';
}


// Draws text in the SDL2 window using the custom pixel font.
// The scale value controls the size of each pixel.
void drawText(SDL_Renderer* r, string text, int x, int y, int scale) {

    // Remember the starting x position for new lines.
    int startX = x;

    // Draw each character in the text.
    for (char c : text) {

        // Move down and return to the starting x position for a new line.
        if (c == '\n') {
            y += 6 * scale;
            x = startX;
            continue;
        }

        // Check every pixel in the 5-by-5 character pattern.
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 5; col++) {

                // Draw a rectangle for each filled pixel.
                if (glyph(c, col, row)) {
                    SDL_Rect pixel = {
                        x + col * scale,
                        y + row * scale,
                        scale,
                        scale
                    };

                    SDL_RenderFillRect(r, &pixel);
                }
            }
        }

        // Leave a small space before drawing the next character.
        x += 6 * scale;
    }
}


// ---------------- BOARD POSITION ----------------

// Calculates where a board space should appear in the window.
// The spaces are arranged in a circle using sine and cosine.
void position(int index, int& x, int& y) {

    // Calculate the angle for this space around the circle.
    double angle = (2.0 * M_PI * index / BOARD_SIZE) - M_PI / 2.0;

    // Controls the distance of the spaces from the center.
    int radius = 315;

    // Calculate the x and y coordinates for the space.
    x = WINDOW_SIZE / 2 + static_cast<int>(radius * cos(angle));
    y = WINDOW_SIZE / 2 + static_cast<int>(radius * sin(angle));
}


// ---------------- TAKE A TURN ----------------

// Handles one player's turn, including rolling the dice,
// moving around the board, applying space effects, and checking the winner.
void takeTurn(
    Node* player[],       // Current board position of each player.
    int turn,             // 0 = player, 1 = computer.
    int money[],          // Money belonging to each player.
    int care[],           // Care points belonging to each player.
    int& dice,             // Stores the dice result.
    int& turnsPlayed,      // Counts individual turns taken.
    bool& gameOver,        // Becomes true when the game ends.
    bool lost[],           // Tracks whether a player lost immediately.
    string& winnerMessage  // Stores the final winner message.
) {

    // Generate a random dice result from 1 to 6.
    dice = rand() % 6 + 1;

    // Move forward by following the linked list pointers.
    // The circular list automatically returns to START after its last node.
    for (int i = 0; i < dice; i++) {
        player[turn] = player[turn]->next;
    }

    // Count this individual turn.
    turnsPlayed++;

    // Display the player's move in the console.
    cout << "\n"
         << (turn == 0 ? "YOUR TURN" : "COMPUTER TURN")
         << " rolled " << dice
         << " and landed on " << player[turn]->name << endl;

    // Store the space where the current player landed.
    Node* space = player[turn];

    // Apply the rules based on the space's type number.
    switch (space->type) {

        // Type 0: START.
        // The player receives $100 when landing on this space.
        case 0:
            money[turn] += 100;
            cout << "You received $100 for passing START.\n";
            break;

        // Type 1: EXPENSE.
        // The player pays the cost and earns 2 care points.
        case 1:
            if (money[turn] >= space->cost) {
                money[turn] -= space->cost;
                care[turn] += 2;

                cout << "Paid $" << space->cost
                     << " for pet care. Care +2.\n";

            } else {
                // If the player cannot afford the expense,
                // move backward five spaces around the circular board.
                cout << "Not enough money! Moving back 5 spaces.\n";

                for (int i = 0; i < BOARD_SIZE - 5; i++) {
                    player[turn] = player[turn]->next;
                }
            }
            break;

        // Type 2: REWARD.
        // The player receives money and earns 2 care points.
        case 2:
            money[turn] += space->cost;
            care[turn] += 2;

            cout << "You earned $" << space->cost << "! Care +2.\n";
            break;

        // Type 3: LOST PET.
        // Landing here ends the game immediately.
        case 3:
            cout << "Your pet got lost! Game over.\n";

            lost[turn] = true;
            gameOver = true;

            // The other player wins.
            winnerMessage = (turn == 0) ? "COMPUTER WINS!" : "YOU WIN!";
            break;

        // Type 4: BONUS.
        // The player receives bonus money and 1 care point.
        case 4:
            money[turn] += space->cost;
            care[turn] += 1;

            cout << "Bonus! Earned $" << space->cost << ". Care +1.\n";
            break;

        // Type 5: PROPERTY.
        // A player can purchase the property if it is unowned
        // and the player has enough money.
        case 5:
            if (space->owner == -1) {

                if (money[turn] >= space->cost) {

                    // Pay for the property and record its owner.
                    money[turn] -= space->cost;
                    space->owner = turn;

                    // Purchasing a property earns 1 care point.
                    care[turn] += 1;

                    cout << "You purchased " << space->name
                         << " for $" << space->cost << ".\n";

                } else {
                    cout << "Not enough money to buy this space.\n";
                }

            } else {
                // The property cannot be purchased again once owned.
                cout << space->name
                     << " is already owned. No purchase.\n";
            }
            break;

        // Type 6: LAWSUIT/JAIL.
        // Landing here also ends the game immediately.
        case 6:
            cout << "Lawsuit for mistreatment! Game over.\n";

            lost[turn] = true;
            gameOver = true;

            winnerMessage = (turn == 0) ? "COMPUTER WINS!" : "YOU WIN!";
            break;
    }

    // Show both players' updated money and care points.
    cout << "Your money: $" << money[0]
         << " | Your care: " << care[0] << endl;

    cout << "Computer money: $" << money[1]
         << " | Computer care: " << care[1] << endl;

    // If no one has lost immediately, end the game after 20 individual turns.
    if (!gameOver && turnsPlayed >= 20) {
        gameOver = true;

        // Each care point is worth $50 when calculating the final score.
        int yourScore = money[0] + care[0] * 50;
        int computerScore = money[1] + care[1] * 50;

        // Compare the scores to determine the winner.
        if (yourScore > computerScore) {
            winnerMessage = "YOU WIN!";
        } else if (computerScore > yourScore) {
            winnerMessage = "COMPUTER WINS!";
        } else {
            winnerMessage = "IT IS A TIE!";
        }

        cout << "\nMaximum turns reached!\n";
        cout << "Your final score: " << yourScore << endl;
        cout << "Computer final score: " << computerScore << endl;
        cout << winnerMessage << endl;
    }
}


// ---------------- MAIN PROGRAM ----------------

// The main function starts the game, creates the board,
// initializes SDL2, handles user input, and displays the results.
int main() {

    // Use the current time to make the random dice rolls different
    // each time the program runs.
    srand(static_cast<unsigned int>(time(nullptr)));

    // Display the game title in the console.
    cout << "====================================\n";
    cout << "     Happy Tails\n";
    cout << "====================================\n\n";

    // Ask the player to choose a pet.
    cout << "Choose your pet:\n";

    // Display the five available pets with numbered options.
    for (int i = 0; i < 5; i++) {
        cout << i + 1 << ". " << pets[i] << endl;
    }

    int choice;

    cout << "Enter a number from 1 to 5: ";
    cin >> choice;

    // Keep asking until the player enters a valid number.
    while (choice < 1 || choice > 5) {
        cout << "Invalid choice. Enter a number from 1 to 5: ";
        cin >> choice;
    }

    // Save the player's chosen pet.
    string first = pets[choice - 1];

    string second;

    // Randomly choose a pet for the computer.
    // The computer cannot choose the same pet as the player.
    do {
        second = pets[rand() % 5];
    } while (second == first);

    cout << "\nYou chose: " << first << endl;
    cout << "Computer chose: " << second << endl;


    // ---------------- CREATE THE CIRCULAR BOARD ----------------

    // Start with an empty linked list.
    Node* head = nullptr;

    // Add all 18 spaces to the circular linked list.
    // Each space has a name, a cost, and a type number.
    head = addNode(head, "START", 0, 0);
    head = addNode(head, "PET HOUSE", 200, 5);
    head = addNode(head, "FOOD", 100, 1);
    head = addNode(head, "VETERINARY", 120, 1);
    head = addNode(head, "TRAINING", 120, 5);
    head = addNode(head, "PET SALON", 80, 5);
    head = addNode(head, "PET STORE", 100, 5);
    head = addNode(head, "REWARD", 200, 2);
    head = addNode(head, "PARK", 50, 4);
    head = addNode(head, "TREATS", 80, 1);
    head = addNode(head, "LOST PET", 0, 3);
    head = addNode(head, "VETERINARY", 150, 1);
    head = addNode(head, "PET SHOW", 150, 2);
    head = addNode(head, "PET HOUSE", 200, 5);
    head = addNode(head, "GROOMING", 80, 1);
    head = addNode(head, "LAWSUIT JAIL", 0, 6);
    head = addNode(head, "REWARD", 200, 2);
    head = addNode(head, "FOOD BONUS", 50, 4);

    // Print all board spaces in the console for testing.
    printList(head);

    // searchNode() is implemented above but is not called here.
    // removeNode() is also implemented for linked list testing,
    // but this game does not currently remove spaces during play.


    // ---------------- INITIALIZE SDL2 ----------------

    // Start SDL2's video system so the game can display a window.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize: " << SDL_GetError() << endl;

        // Clean up the board if initialization fails.
        deleteList(head);
        return 1;
    }

    // Create the game window.
    SDL_Window* window = SDL_CreateWindow(
        "Happy Tails",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE,
        WINDOW_SIZE,
        SDL_WINDOW_SHOWN
    );

    // Stop if the window could not be created.
    if (window == nullptr) {
        cout << "Window could not be created: " << SDL_GetError() << endl;

        SDL_Quit();
        deleteList(head);
        return 1;
    }

    // Create a renderer, which draws shapes and text inside the window.
    SDL_Renderer* r = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    // Stop if the renderer could not be created.
    if (r == nullptr) {
        cout << "Renderer could not be created: " << SDL_GetError() << endl;

        SDL_DestroyWindow(window);
        SDL_Quit();
        deleteList(head);
        return 1;
    }


    // ---------------- INITIALIZE GAME VARIABLES ----------------

    // Both players start at START, which is the head of the board.
    Node* player[2] = {head, head};

    // Each player starts with $1,000 and zero care points.
    // Index 0 represents the user; index 1 represents the computer.
    int money[2] = {1000, 1000};
    int care[2] = {0, 0};

    int turn = 0;          // The user goes first.
    int dice = 1;          // Initial value shown in the dice box.
    int turnsPlayed = 0;   // Number of individual turns completed.

    bool running = true;   // Controls whether the window stays open.
    bool gameOver = false; // Tracks whether the game has ended.
    bool lost[2] = {false, false}; // Tracks immediate losses.

    string winnerMessage = "";

    // Define the position and size of the blue ROLL button.
    SDL_Rect rollButton = {380, 440, 140, 42};


    // ---------------- MAIN GAME LOOP ----------------

    // This loop continues until the player closes the window.
    while (running) {

        SDL_Event event;

        // Check for events such as closing the window,
        // pressing the spacebar, or clicking the mouse.
        while (SDL_PollEvent(&event)) {

            // Close the game when the window's close button is clicked.
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // Pressing SPACE rolls the dice for the current player.
            if (!gameOver &&
                event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_SPACE) {

                // Process the current player's turn.
                takeTurn(
                    player, turn, money, care, dice,
                    turnsPlayed, gameOver, lost, winnerMessage
                );

                // Switch between player 0 and player 1
                // if the game has not ended.
                if (!gameOver) {
                    turn = 1 - turn;
                }
            }

            // The mouse can also be used to roll the dice.
            if (!gameOver &&
                event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                // Get the mouse's current coordinates.
                int mx = event.button.x;
                int my = event.button.y;

                // Check whether the mouse click is inside the ROLL button.
                if (mx >= rollButton.x &&
                    mx <= rollButton.x + rollButton.w &&
                    my >= rollButton.y &&
                    my <= rollButton.y + rollButton.h) {

                    // Process the current player's turn.
                    takeTurn(
                        player, turn, money, care, dice,
                        turnsPlayed, gameOver, lost, winnerMessage
                    );

                    // Give the next turn to the other player.
                    if (!gameOver) {
                        turn = 1 - turn;
                    }
                }
            }
        }


        // ---------------- DRAW BACKGROUND ----------------

        // Set the background color to light blue.
        SDL_SetRenderDrawColor(r, 173, 216, 230, 255);

        // Clear the previous frame before drawing the new one.
        SDL_RenderClear(r);

        // Draw the game title.
        SDL_SetRenderDrawColor(r, 30, 65, 85, 255);
        drawText(r, "Happy Tails", 170, 18, 4);

        // Display whose turn it is or whether the game has ended.
        SDL_SetRenderDrawColor(r, 25, 55, 65, 255);

        if (gameOver) {
            drawText(r, "GAME OVER", 365, 72, 3);
        } else if (turn == 0) {
            drawText(r, "YOUR TURN - CLICK ROLL", 180, 75, 2);
        } else {
            drawText(r, "COMPUTER TURN - CLICK ROLL", 150, 75, 2);
        }


        // ---------------- BOARD SPACE DESCRIPTIONS ----------------

        // These descriptions are displayed inside the board tiles.
        // The order matches the order in which the nodes were added.
        string descriptions[BOARD_SIZE] = {
            "START\nGET $100",
            "PET\nHOUSE\n$200",
            "FOOD\nPAY $100",
            "VET\nPAY $120",
            "TRAIN\n$120",
            "SALON\n$80",
            "PET\nSTORE\n$100",
            "WIN\n+$200",
            "PARK\n+$50",
            "TREATS\nPAY $80",
            "LOST\nPET!\nYOU LOSE",
            "VET\nPAY $150",
            "PET\nSHOW\n+$150",
            "PET\nHOUSE\n$200",
            "GROOM\nPAY $80",
            "LAWSUIT\nJAIL!\nYOU LOSE",
            "REWARD\n+$200",
            "FOOD\nBONUS\n+$50"
        };


        // ---------------- DRAW BOARD SPACES ----------------

        // Start at the first node and visit each board space.
        Node* space = head;

        for (int i = 0; i < BOARD_SIZE; i++) {

            int x, y;

            // Calculate the position of the current tile.
            position(i, x, y);

            // Define the size and location of the tile.
            SDL_Rect tile = {x - 52, y - 42, 104, 84};

            // Use different colors to show the types of spaces.
            if (space->type == 3 || space->type == 6) {
                // Red: spaces that cause an immediate loss.
                SDL_SetRenderDrawColor(r, 240, 110, 110, 255);

            } else if (space->type == 2) {
                // Green: reward spaces.
                SDL_SetRenderDrawColor(r, 125, 220, 140, 255);

            } else if (space->type == 4) {
                // Blue: bonus spaces.
                SDL_SetRenderDrawColor(r, 120, 200, 245, 255);

            } else if (space->type == 5) {
                // Purple: properties that can be purchased.
                SDL_SetRenderDrawColor(r, 205, 175, 245, 255);

            } else {
                // Light cream: START and expense spaces.
                SDL_SetRenderDrawColor(r, 245, 245, 225, 255);
            }

            // Fill the tile with its selected color.
            SDL_RenderFillRect(r, &tile);

            // Draw the tile's border.
            SDL_SetRenderDrawColor(r, 35, 65, 75, 255);
            SDL_RenderDrawRect(r, &tile);

            // Display the space's number.
            SDL_SetRenderDrawColor(r, 20, 45, 55, 255);
            drawText(r, to_string(i), x - 45, y - 35, 2);

            // Display the space's name and effect.
            SDL_SetRenderDrawColor(r, 20, 45, 55, 255);
            drawText(r, descriptions[i], x - 45, y - 17, 2);

            // If the property has an owner, display who owns it.
            if (space->owner != -1) {
                SDL_SetRenderDrawColor(r, 20, 45, 55, 255);

                if (space->owner == 0) {
                    drawText(r, "YOU OWN", x - 30, y + 27, 1);
                } else {
                    drawText(r, "CPU OWN", x - 30, y + 27, 1);
                }
            }

            // Draw the user's blue token if they are on this space.
            if (player[0] == space) {
                SDL_Rect token = {x - 17, y + 20, 12, 12};

                SDL_SetRenderDrawColor(r, 30, 90, 220, 255);
                SDL_RenderFillRect(r, &token);
            }

            // Draw the computer's red token if it is on this space.
            if (player[1] == space) {
                SDL_Rect token = {x + 5, y + 20, 12, 12};

                SDL_SetRenderDrawColor(r, 220, 50, 50, 255);
                SDL_RenderFillRect(r, &token);
            }

            // Follow the linked list to the next board space.
            space = space->next;
        }


        // ---------------- CENTER PANEL ----------------

        // Create the panel in the middle of the circular board.
        SDL_Rect centerPanel = {320, 285, 260, 310};

        SDL_SetRenderDrawColor(r, 230, 245, 250, 255);
        SDL_RenderFillRect(r, &centerPanel);

        SDL_SetRenderDrawColor(r, 35, 65, 75, 255);
        SDL_RenderDrawRect(r, &centerPanel);

        // Add the dice section title.
        SDL_SetRenderDrawColor(r, 25, 55, 65, 255);
        drawText(r, "ROLL DICE", 375, 305, 3);

        // Draw the white box that displays the dice result.
        SDL_Rect diceBox = {405, 350, 90, 65};

        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderFillRect(r, &diceBox);

        SDL_SetRenderDrawColor(r, 35, 65, 75, 255);
        SDL_RenderDrawRect(r, &diceBox);

        // Display the most recent dice result.
        drawText(r, to_string(dice), 440, 368, 5);

        // Draw the blue ROLL button below the dice.
        SDL_SetRenderDrawColor(r, 65, 140, 195, 255);
        SDL_RenderFillRect(r, &rollButton);

        SDL_SetRenderDrawColor(r, 35, 65, 75, 255);
        SDL_RenderDrawRect(r, &rollButton);

        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        drawText(r, "ROLL", 420, 453, 3);


        // ---------------- PLAYER INFORMATION ----------------

        // Display the user's current money and care points.
        SDL_SetRenderDrawColor(r, 25, 55, 65, 255);

        drawText(r, "YOU", 345, 495, 2);
        drawText(r, "$", 345, 515, 2);
        drawText(r, to_string(money[0]), 360, 515, 2);
        drawText(r, "CARE", 345, 535, 2);
        drawText(r, to_string(care[0]), 405, 535, 2);

        // Display the computer's current money and care points.
        drawText(r, "CPU", 450, 495, 2);
        drawText(r, "$", 450, 515, 2);
        drawText(r, to_string(money[1]), 465, 515, 2);
        drawText(r, "CARE", 450, 535, 2);
        drawText(r, to_string(care[1]), 510, 535, 2);


        // ---------------- GAME OVER PANEL ----------------

        // Display the final result when the game ends.
        if (gameOver) {

            SDL_Rect resultPanel = {225, 285, 450, 310};

            SDL_SetRenderDrawColor(r, 245, 252, 255, 255);
            SDL_RenderFillRect(r, &resultPanel);

            SDL_SetRenderDrawColor(r, 40, 70, 85, 255);
            SDL_RenderDrawRect(r, &resultPanel);

            SDL_SetRenderDrawColor(r, 25, 55, 65, 255);
            drawText(r, "GAME OVER", 315, 310, 5);

            // Display the appropriate message if someone lost
            // on the Lost Pet or Lawsuit/Jail space.
            if (lost[0]) {
                drawText(r, "YOU LOSE!", 340, 380, 4);
                drawText(r, "COMPUTER WINS!", 270, 430, 3);

            } else if (lost[1]) {
                drawText(r, "COMPUTER LOSES!", 260, 380, 3);
                drawText(r, "YOU WIN!", 350, 430, 4);

            } else {
                // Otherwise, display the winner based on the final scores.
                drawText(r, winnerMessage, 270, 405, 3);
            }

            // Calculate the final scores for display.
            // Each care point adds 50 points to the player's money total.
            int yourScore = money[0] + care[0] * 50;
            int computerScore = money[1] + care[1] * 50;

            // Show both final scores.
            SDL_SetRenderDrawColor(r, 25, 55, 65, 255);

            drawText(r, "YOUR SCORE", 270, 490, 2);
            drawText(r, to_string(yourScore), 410, 490, 2);

            drawText(r, "CPU SCORE", 270, 525, 2);
            drawText(r, to_string(computerScore), 410, 525, 2);
        }


        // Display the completed frame in the game window.
        SDL_RenderPresent(r);

        // Wait 16 milliseconds before the next frame.
        // This keeps the game from continuously drawing as fast as possible.
        SDL_Delay(16);
    }


    // ---------------- FINAL CONSOLE RESULTS ----------------

    // Print the final game information in the console after the window closes.
    cout << "\n====================================\n";
    cout << "           FINAL RESULTS\n";
    cout << "====================================\n";

    cout << "Your pet: " << first << endl;
    cout << "Computer pet: " << second << endl;

    cout << "Your money: $" << money[0] << endl;
    cout << "Your care points: " << care[0] << endl;

    cout << "Computer money: $" << money[1] << endl;
    cout << "Computer care points: " << care[1] << endl;

    // Check whether either player lost immediately.
    if (lost[0]) {
        cout << "You lost because of a game-ending event.\n";
        cout << "COMPUTER WINS!\n";

    } else if (lost[1]) {
        cout << "The computer lost because of a game-ending event.\n";
        cout << "YOU WIN!\n";

    } else {
        // If neither player lost immediately, compare final scores.
        int yourScore = money[0] + care[0] * 50;
        int computerScore = money[1] + care[1] * 50;

        cout << "Your final score: " << yourScore << endl;
        cout << "Computer final score: " << computerScore << endl;

        if (yourScore > computerScore) {
            cout << "YOU WIN!\n";
        } else if (computerScore > yourScore) {
            cout << "COMPUTER WINS!\n";
        } else {
            cout << "IT IS A TIE!\n";
        }
    }


    // ---------------- CLEANUP ----------------

    // Free all nodes in the circular linked list.
    deleteList(head);

    // Release the SDL2 renderer and window resources.
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);

    // Shut down SDL2 before the program ends.
    SDL_Quit();

    return 0;
}
