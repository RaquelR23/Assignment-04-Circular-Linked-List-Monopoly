# Assignment-04-Circular-Linked-List-Monopoly
Happy Tails:

This is a board game inspired by Monopoly in which the player competes against the computer while taking care of a virtual pet. Players can choose from five pets: Golden Retriever, Dalmatian, Gray Tabby Cat, Chinchilla, or White and Gray Rabbit.

The goal is to manage money, earn care points, and make responsible decisions while moving around the board. Players encounter different spaces that represent pet-related expenses, rewards, services, and unexpected events. The game combines strategy, random dice rolls, and pet care responsibilities to determine the winner.

How the Game Works:
Choose a pet: At the beginning, the player selects one of the five available pets. The computer randomly chooses a different pet.
Start the game: Both the player and computer begin on the START space with $1,000 each and zero care points.
Take turns: The player and computer alternate turns. During each turn, the current player rolls a virtual six-sided die by clicking the ROLL button or pressing the spacebar.
Move around the board: The player moves forward according to the dice result. The board contains 18 spaces arranged in a circular path, so players continue from the beginning after reaching the end.
Follow the space rules: Each space has an effect. Depending on the space, players may earn money, pay for pet care, receive care points, purchase a location, or encounter a game-ending event.
Track progress: The game displays each player's money, care points, position, and property ownership. Information about turns and events is also printed in the console.
Determine the winner: The game ends if a player lands on LOST PET or LAWSUIT JAIL, or when 20 turns have been completed. If the game reaches the turn limit, the winner is determined by the final score.

Game Rules -----------------------------------------------

1. Starting Money and Care Points
-Each player starts with $1,000.
-Both players start with 0 care points.
-The player chooses a pet, and the computer chooses a different one.

2. Board Spaces
-Space type:	What happens
-START: The player receives $100 when landing on this space.
-Pet care expenses:The player pays the listed cost and earns 2 care points. If the player cannot afford the expense, they move backward 5 spaces.
-Rewards:	The player earns the listed amount of money and 2 care points.
-Bonuses:	The player receives the listed amount of money and 1 care point.


3. Taking Turns
-Players alternate turns, starting with the human player.
-Each roll produces a random number from 1 to 6.
-The player moves forward by that number of spaces.
-The effect of the space is applied after the player lands.
-The game tracks a maximum of 20 turns. Each completed turn by the computer increases the turn counter, so the counter advances after each pair of player and computer turns.

4. Game-Ending Events
A player immediately loses if they land on either of these spaces:
-LOST PET: Represents losing a pet.
-LAWSUIT JAIL: Represents a lawsuit related to mistreatment.
The other player is declared the winner, regardless of their current money or care points.

5. Final Scoring
-If neither player encounters a game-ending event, the winner is determined after the turn limit is reached.
-The score is calculated using this formula:
-Final Score = Remaining Money + (Care Points × 50)
-Each care point is worth $50 in the final score. The player with the higher score wins. If both players have the same score, the game ends in a tie.


Delete the list: Free the dynamically allocated memory when the program finishes.
