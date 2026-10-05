#include <SDL2/SDL.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

const int BOARD_SIZE = 18;
const int WINDOW_SIZE = 900;

struct Node {
    string name;
    int cost;
    int owner;
    int type;
    Node* next;
};

string pets[5] = {
    "Golden Retriever",
    "Dalmatian",
    "Gray Tabby Cat",
    "Chinchilla",
    "White and Gray Rabbit"
};

// ---------------- LINKED LIST FUNCTIONS ----------------

Node* addNode(Node* head, string name, int cost, int type) {
    Node* newNode = new Node{name, cost, -1, type, nullptr};

    if (head == nullptr) {
        newNode->next = newNode;
        return newNode;
    }

    Node* temp = head;

    while (temp->next != head) {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->next = head;

    return head;
}

Node* searchNode(Node* head, string name) {
    if (head == nullptr) return nullptr;

    Node* temp = head;

    do {
        if (temp->name == name) return temp;
        temp = temp->next;
    } while (temp != head);

    return nullptr;
}

void printList(Node* head) {
    if (head == nullptr) return;

    Node* temp = head;
    int index = 0;

    cout << "\n--- GAME BOARD SPACES ---\n";

    do {
        cout << index << ". " << temp->name
             << " | Cost: $" << temp->cost
             << " | Type: " << temp->type << endl;

        temp = temp->next;
        index++;
    } while (temp != head);
}

Node* removeNode(Node* head, string name) {
    if (head == nullptr) return nullptr;

    Node* current = head;
    Node* previous = nullptr;

    do {
        if (current->name == name) {
            if (current->next == current) {
                delete current;
                return nullptr;
            }

            if (previous == nullptr) {
                Node* last = head;

                while (last->next != head) {
                    last = last->next;
                }

                head = current->next;
                last->next = head;
            } else {
                previous->next = current->next;
            }

            delete current;
            return head;
        }

        previous = current;
        current = current->next;

    } while (current != head);

    return head;
}

void deleteList(Node* head) {
    if (head == nullptr) return;

    Node* last = head;

    while (last->next != head) {
        last = last->next;
    }

    last->next = nullptr;

    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// ---------------- PIXEL FONT ----------------

bool glyph(char c, int x, int y) {
    string rows[5];

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

        case '$': rows[0]="01111"; rows[1]="10100"; rows[2]="01110"; rows[3]="00101"; rows[4]="11110"; break;
        case '!': rows[0]="00100"; rows[1]="00100"; rows[2]="00100"; rows[3]="00000"; rows[4]="00100"; break;
        case '-': rows[0]="00000"; rows[1]="00000"; rows[2]="11111"; rows[3]="00000"; rows[4]="00000"; break;
        case ':': rows[0]="00000"; rows[1]="00100"; rows[2]="00000"; rows[3]="00100"; rows[4]="00000"; break;
        case '+': rows[0]="00000"; rows[1]="00100"; rows[2]="11111"; rows[3]="00100"; rows[4]="00000"; break;
        case ' ': rows[0]="00000"; rows[1]="00000"; rows[2]="00000"; rows[3]="00000"; rows[4]="00000"; break;

        default:
            for (int i = 0; i < 5; i++) rows[i] = "00000";
    }

    return rows[y][x] == '1';
}

void drawText(SDL_Renderer* r, string text, int x, int y, int scale) {
    int startX = x;

    for (char c : text) {
        if (c == '\n') {
            y += 6 * scale;
            x = startX;
            continue;
        }

        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 5; col++) {
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

        x += 6 * scale;
    }
}

// ---------------- BOARD POSITION ----------------

void position(int index, int& x, int& y) {
    double angle = (2.0 * M_PI * index / BOARD_SIZE) - M_PI / 2.0;
    int radius = 315;

    x = WINDOW_SIZE / 2 + static_cast<int>(radius * cos(angle));
    y = WINDOW_SIZE / 2 + static_cast<int>(radius * sin(angle));
}

// ---------------- TAKE A TURN ----------------

void takeTurn(
    Node* player[],
    int turn,
    int money[],
    int care[],
    int& dice,
    int& turnsPlayed,
    bool& gameOver,
    bool lost[],
    string& winnerMessage
) {
    dice = rand() % 6 + 1;

    for (int i = 0; i < dice; i++) {
        player[turn] = player[turn]->next;
    }

    turnsPlayed++;

    cout << "\n"
         << (turn == 0 ? "YOUR TURN" : "COMPUTER TURN")
         << " rolled " << dice
         << " and landed on " << player[turn]->name << endl;

    Node* space = player[turn];

    switch (space->type) {
        case 0:
            money[turn] += 100;
            cout << "You received $100 for passing START.\n";
            break;

        case 1:
            if (money[turn] >= space->cost) {
                money[turn] -= space->cost;
                care[turn] += 2;
                cout << "Paid $" << space->cost
                     << " for pet care. Care +2.\n";
            } else {
                cout << "Not enough money! Moving back 5 spaces.\n";

                for (int i = 0; i < BOARD_SIZE - 5; i++) {
                    player[turn] = player[turn]->next;
                }
            }
            break;

        case 2:
            money[turn] += space->cost;
            care[turn] += 2;
            cout << "You earned $" << space->cost << "! Care +2.\n";
            break;

        case 3:
            cout << "Your pet got lost! Game over.\n";
            lost[turn] = true;
            gameOver = true;
            winnerMessage = (turn == 0) ? "COMPUTER WINS!" : "YOU WIN!";
            break;

        case 4:
            money[turn] += space->cost;
            care[turn] += 1;
            cout << "Bonus! Earned $" << space->cost << ". Care +1.\n";
            break;

        case 5:
            if (space->owner == -1) {
                if (money[turn] >= space->cost) {
                    money[turn] -= space->cost;
                    space->owner = turn;
                    care[turn] += 1;

                    cout << "You purchased " << space->name
                         << " for $" << space->cost << ".\n";
                } else {
                    cout << "Not enough money to buy this space.\n";
                }
            } else {
                cout << space->name
                     << " is already owned. No purchase.\n";
            }
            break;

        case 6:
            cout << "Lawsuit for mistreatment! Game over.\n";
            lost[turn] = true;
            gameOver = true;
            winnerMessage = (turn == 0) ? "COMPUTER WINS!" : "YOU WIN!";
            break;
    }

    cout << "Your money: $" << money[0]
         << " | Your care: " << care[0] << endl;

    cout << "Computer money: $" << money[1]
         << " | Computer care: " << care[1] << endl;

    if (!gameOver && turnsPlayed >= 20) {
        gameOver = true;

        int yourScore = money[0] + care[0] * 50;
        int computerScore = money[1] + care[1] * 50;

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

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "====================================\n";
    cout << "     Happy Tails\n";
    cout << "====================================\n\n";

    cout << "Choose your pet:\n";

    for (int i = 0; i < 5; i++) {
        cout << i + 1 << ". " << pets[i] << endl;
    }

    int choice;

    cout << "Enter a number from 1 to 5: ";
    cin >> choice;

    while (choice < 1 || choice > 5) {
        cout << "Invalid choice. Enter a number from 1 to 5: ";
        cin >> choice;
    }

    string first = pets[choice - 1];
    string second;

    do {
        second = pets[rand() % 5];
    } while (second == first);

    cout << "\nYou chose: " << first << endl;
    cout << "Computer chose: " << second << endl;

    Node* head = nullptr;

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

    printList(head);

    // The searchNode function is kept above, but it is no longer
    // called here, so the search-success message will not print.

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize: " << SDL_GetError() << endl;
        deleteList(head);
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Happy Tails",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE,
        WINDOW_SIZE,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        cout << "Window could not be created: " << SDL_GetError() << endl;
        SDL_Quit();
        deleteList(head);
        return 1;
    }

    SDL_Renderer* r = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    if (r == nullptr) {
        cout << "Renderer could not be created: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        deleteList(head);
        return 1;
    }

    Node* player[2] = {head, head};

    int money[2] = {1000, 1000};
    int care[2] = {0, 0};

    int turn = 0;
    int dice = 1;
    int turnsPlayed = 0;

    bool running = true;
    bool gameOver = false;
    bool lost[2] = {false, false};

    string winnerMessage = "";

    // Blue button is separate from the white dice box.
    SDL_Rect rollButton = {380, 440, 140, 42};

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (!gameOver &&
                event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_SPACE) {

                takeTurn(
                    player, turn, money, care, dice,
                    turnsPlayed, gameOver, lost, winnerMessage
                );

                if (!gameOver) {
                    turn = 1 - turn;
                }
            }

            if (!gameOver &&
                event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                int mx = event.button.x;
                int my = event.button.y;

                if (mx >= rollButton.x &&
                    mx <= rollButton.x + rollButton.w &&
                    my >= rollButton.y &&
                    my <= rollButton.y + rollButton.h) {

                    takeTurn(
                        player, turn, money, care, dice,
                        turnsPlayed, gameOver, lost, winnerMessage
                    );

                    if (!gameOver) {
                        turn = 1 - turn;
                    }
                }
            }
        }

        // ---------------- DRAW BACKGROUND ----------------

        SDL_SetRenderDrawColor(r, 173, 216, 230, 255);
        SDL_RenderClear(r);

        SDL_SetRenderDrawColor(r, 30, 65, 85, 255);
        drawText(r, "Happy Tails", 170, 18, 4);

        // Keep the turn message above the top tile.
        SDL_SetRenderDrawColor(r, 25, 55, 65, 255);

        if (gameOver) {
            drawText(r, "GAME OVER", 365, 72, 3);
        } else if (turn == 0) {
            drawText(r, "YOUR TURN - CLICK ROLL", 180, 75, 2);
        } else {
            drawText(r, "COMPUTER TURN - CLICK ROLL", 150, 75, 2);
        }

        // ---------------- BOARD SPACES ----------------

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

        Node* space = head;

        for (int i = 0; i < BOARD_SIZE; i++) {
            int x, y;
            position(i, x, y);

            SDL_Rect tile = {x - 52, y - 42, 104, 84};

            if (space->type == 3 || space->type == 6) {
                SDL_SetRenderDrawColor(r, 240, 110, 110, 255);
            } else if (space->type == 2) {
                SDL_SetRenderDrawColor(r, 125, 220, 140, 255);
            } else if (space->type == 4) {
                SDL_SetRenderDrawColor(r, 120, 200, 245, 255);
            } else if (space->type == 5) {
                SDL_SetRenderDrawColor(r, 205, 175, 245, 255);
            } else {
                SDL_SetRenderDrawColor(r, 245, 245, 225, 255);
            }

            SDL_RenderFillRect(r, &tile);

            SDL_SetRenderDrawColor(r, 35, 65, 75, 255);
            SDL_RenderDrawRect(r, &tile);

            SDL_SetRenderDrawColor(r, 20, 45, 55, 255);
            drawText(r, to_string(i), x - 45, y - 35, 2);

            SDL_SetRenderDrawColor(r, 20, 45, 55, 255);
            drawText(r, descriptions[i], x - 45, y - 17, 2);

            if (space->owner != -1) {
                SDL_SetRenderDrawColor(r, 20, 45, 55, 255);

                if (space->owner == 0) {
                    drawText(r, "YOU OWN", x - 30, y + 27, 1);
                } else {
                    drawText(r, "CPU OWN", x - 30, y + 27, 1);
                }
            }

            // Blue token represents the user.
            if (player[0] == space) {
                SDL_Rect token = {x - 17, y + 20, 12, 12};
                SDL_SetRenderDrawColor(r, 30, 90, 220, 255);
                SDL_RenderFillRect(r, &token);
            }

            // Red token represents the computer.
            if (player[1] == space) {
                SDL_Rect token = {x + 5, y + 20, 12, 12};
                SDL_SetRenderDrawColor(r, 220, 50, 50, 255);
                SDL_RenderFillRect(r, &token);
            }

            space = space->next;
        }

        // ---------------- CENTER PANEL ----------------

        SDL_Rect centerPanel = {320, 285, 260, 310};

        SDL_SetRenderDrawColor(r, 230, 245, 250, 255);
        SDL_RenderFillRect(r, &centerPanel);

        SDL_SetRenderDrawColor(r, 35, 65, 75, 255);
        SDL_RenderDrawRect(r, &centerPanel);

        SDL_SetRenderDrawColor(r, 25, 55, 65, 255);
        drawText(r, "ROLL DICE", 375, 305, 3);

        // White dice box.
        SDL_Rect diceBox = {405, 350, 90, 65};

        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderFillRect(r, &diceBox);

        SDL_SetRenderDrawColor(r, 35, 65, 75, 255);
        SDL_RenderDrawRect(r, &diceBox);

        drawText(r, to_string(dice), 440, 368, 5);

        // Blue ROLL button is below the dice box.
        SDL_SetRenderDrawColor(r, 65, 140, 195, 255);
        SDL_RenderFillRect(r, &rollButton);

        SDL_SetRenderDrawColor(r, 35, 65, 75, 255);
        SDL_RenderDrawRect(r, &rollButton);

        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        drawText(r, "ROLL", 420, 453, 3);

        // Player information.
        SDL_SetRenderDrawColor(r, 25, 55, 65, 255);

        drawText(r, "YOU", 345, 495, 2);
        drawText(r, "$", 345, 515, 2);
        drawText(r, to_string(money[0]), 360, 515, 2);
        drawText(r, "CARE", 345, 535, 2);
        drawText(r, to_string(care[0]), 405, 535, 2);

        drawText(r, "CPU", 450, 495, 2);
        drawText(r, "$", 450, 515, 2);
        drawText(r, to_string(money[1]), 465, 515, 2);
        drawText(r, "CARE", 450, 535, 2);
        drawText(r, to_string(care[1]), 510, 535, 2);

        // ---------------- GAME OVER PANEL ----------------

        if (gameOver) {
            SDL_Rect resultPanel = {225, 285, 450, 310};

            SDL_SetRenderDrawColor(r, 245, 252, 255, 255);
            SDL_RenderFillRect(r, &resultPanel);

            SDL_SetRenderDrawColor(r, 40, 70, 85, 255);
            SDL_RenderDrawRect(r, &resultPanel);

            SDL_SetRenderDrawColor(r, 25, 55, 65, 255);
            drawText(r, "GAME OVER", 315, 310, 5);

            if (lost[0]) {
                drawText(r, "YOU LOSE!", 340, 380, 4);
                drawText(r, "COMPUTER WINS!", 270, 430, 3);
            } else if (lost[1]) {
                drawText(r, "COMPUTER LOSES!", 260, 380, 3);
                drawText(r, "YOU WIN!", 350, 430, 4);
            } else {
                drawText(r, winnerMessage, 270, 405, 3);
            }

            int yourScore = money[0] + care[0] * 50;
            int computerScore = money[1] + care[1] * 50;

            SDL_SetRenderDrawColor(r, 25, 55, 65, 255);

            drawText(r, "YOUR SCORE", 270, 490, 2);
            drawText(r, to_string(yourScore), 410, 490, 2);

            drawText(r, "CPU SCORE", 270, 525, 2);
            drawText(r, to_string(computerScore), 410, 525, 2);
        }

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }

    // ---------------- FINAL CONSOLE RESULTS ----------------

    cout << "\n====================================\n";
    cout << "           FINAL RESULTS\n";
    cout << "====================================\n";

    cout << "Your pet: " << first << endl;
    cout << "Computer pet: " << second << endl;

    cout << "Your money: $" << money[0] << endl;
    cout << "Your care points: " << care[0] << endl;

    cout << "Computer money: $" << money[1] << endl;
    cout << "Computer care points: " << care[1] << endl;

    if (lost[0]) {
        cout << "You lost because of a game-ending event.\n";
        cout << "COMPUTER WINS!\n";
    } else if (lost[1]) {
        cout << "The computer lost because of a game-ending event.\n";
        cout << "YOU WIN!\n";
    } else {
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

    deleteList(head);

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}