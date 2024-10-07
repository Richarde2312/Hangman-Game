#include<iostream>
#include<fstream>
#include<cstring>
#include<ctime>
#include<cstdio>
#include<cstdlib>

#define NUM_ROWS 10
#define NUM_COLUMNS 50
#define MAX_WORD_LENGTH 100
#define MAX_WRONG_GUESSES 6
#define HIDDEN_CHAR '*'

using namespace std;

struct Word{
	char word[MAX_WORD_LENGTH + 1];
	char category[101];
	bool seen;
};

struct Round{
	Word actualWord[5];
	Word hiddenWord[5];
	int numWords;
	int numWrongGuesses;
};

int loadWords(Word words[]){
	int numWords = 0;
	ifstream input;
	Word w;
	w.seen = false;
	
    input.open("food_and_drink.txt");
    
	if (!input.is_open()) {
		cout << "Error opening file...Aborting!" << endl;
        return 0;
    }
    
    strcpy(w.category, "Food and drink"); //Use custom implementation
    input >> w.word;
    
    while (strcmp(w.word, "END") != 0){ //Use custom implementation
    	words[numWords] = w;
		++numWords;
		input >> w.word;
	}
	
	input.close();
	
    input.open("fictional_characters.txt");
    
	if (!input.is_open()) {
		cout << "Error opening file...Aborting!" << endl;
        return 0;
    }
    
	strcpy(w.category, "Fictional characters"); //Use custom implementation
    input >> w.word;
    
    while (strcmp(w.word, "END") != 0){ //Use custom implementation
    	words[numWords] = w;
		++numWords;
		input >> w.word;
	}
    
	input.close();
	
    input.open("classic_tv.txt");
    
	if (!input.is_open()) {
		cout << "Error opening file...Aborting!" << endl;
        return 0;
    }
    
	strcpy(w.category, "Classic TV"); //Use custom implementation
    input >> w.word;
    
    while (strcmp(w.word, "END") != 0){ //Use custom implementation
    	words[numWords] = w;
		++numWords;
		input >> w.word;
	}
	
	input.close();
	
	return numWords;
}

void drawHead(char board[][NUM_COLUMNS]){
	for (int j = 42 ; j <= 46; ++j){
		board[1][j] = '-';
		board[4][j] = '-';
	}
	
	for (int i = 2 ; i <= 3; ++i){
		board[i][42] = '|';
		board[i][46] = '|';
	}
	
	board[2][43] = '^';
	board[2][45] = '^';
	board[3][44] = '_';
}

void drawNeck(char board[][NUM_COLUMNS]){
	for (int i = 5 ; i <= 6; ++i)
		board[i][44] = '|';
}

void drawLeftHand(char board[][NUM_COLUMNS]){
	for (int j = 42 ; j <= 43; ++j)
		board[5][j] = '_';
}

void drawRightHand(char board[][NUM_COLUMNS]){
	for (int j = 45 ; j <= 46; ++j)
		board[5][j] = '_';
}

void drawLeftFoot(char board[][NUM_COLUMNS]){
	for (int i = 7, j = 43; i <= 8; ++i, --j)
		board[i][j] = '/';
}

void drawRightFoot(char board[][NUM_COLUMNS]){
	for (int i = 7, j = 45; i <= 8; ++i, ++j)
		board[i][j] = '\\';
}


void initializeBoard(char board[][NUM_COLUMNS]){
	for(int i = 0; i < NUM_ROWS; ++i){
		for(int j = 0; j < NUM_COLUMNS; ++j){
			if((i == 0) || (i == 9) || (j == 0) || (j == 40) || (j == 49))
				board[i][j] = '*';
			else
				board[i][j] = ' ';
		}
	}
}

void updateBoard(char board[][NUM_COLUMNS], Round round){
	int verticalOffset = (NUM_ROWS - 2 - round.numWords) / 2;
	int row = verticalOffset + 1;
	int horizontalOffset;
	int col;
	
	for (int i = 0; i < round.numWords; ++i){
		int length = strlen(round.hiddenWord[i].word); // Implement strlen()
		
		horizontalOffset = (NUM_COLUMNS - 11 - length) / 2;
		col = horizontalOffset + 1;
		
		for (int j = 0; j < length; ++j)
			board[row][col + j] = round.hiddenWord[i].word[j];
		
		++row;
	}
}

void printBoard(char board[][NUM_COLUMNS]){
	for (int i = 0; i < NUM_ROWS; ++i, cout << endl)
		for (int j = 0; j < NUM_COLUMNS; ++j)
			cout << board[i][j];
}

void initializeHiddenWord(char actualWord[], char hiddenWord[]){
	int i = 0;
	
	while (actualWord[i] != '\0'){
		if ((actualWord[i] == 'R') || (actualWord[i] == 'r') || (actualWord[i] == 'S') || (actualWord[i] == 's') || (actualWord[i] == 'T') || (actualWord[i] == 't') || (actualWord[i] == 'L') || (actualWord[i] == 'l') || (actualWord[i] == 'N') || (actualWord[i] == 'n') || !isalpha(actualWord[i])) //Change
			hiddenWord[i] = actualWord[i];
		else
			hiddenWord[i] = HIDDEN_CHAR;
		++i;
	}
	
	hiddenWord[i] = '\0';
}

Round updateGuesses(Round round, char guess){
	bool correctGuess = false;
	
	for (int i = 0; i < round.numWords; ++i){
		int length = strlen(round.actualWord[i].word); // Replace strlen()
		for (int j = 0; j < length; ++j){
			if ((tolower(round.actualWord[i].word[j]) == tolower(guess)) && (round.hiddenWord[i].word[j] == HIDDEN_CHAR)){ // Replace toLower()
				round.hiddenWord[i].word[j] = round.actualWord[i].word[j];
				correctGuess = true;
			}
		}
	}
	
	if (!correctGuess)
		++round.numWrongGuesses;
	
	return round;
}

int numSolvedWords(Round round){
	int total = 0;
	
	for (int i = 0; i < round.numWords; ++i){
		int j = 0;
		
		while ((round.hiddenWord[i].word[j] != '\0') && (round.hiddenWord[i].word[j] != HIDDEN_CHAR))
			++j;
		
		if (round.hiddenWord[i].word[j] == '\0')
			++total;
	}
	
	return total;
}

int numLettersFound(Round previousRound, Round currentRound){
	int total = 0;
	
	for (int i = 0; i < previousRound.numWords; ++i){
		int j = 0;
		
		while (previousRound.hiddenWord[i].word[j] != '\0'){
			if ((previousRound.hiddenWord[i].word[j] == HIDDEN_CHAR) && (currentRound.hiddenWord[i].word[j] != HIDDEN_CHAR))
				++total;
			++j;
		}
	}
	
	return total;
}

int main(){
	srand(time(NULL));
	char board[NUM_ROWS][NUM_COLUMNS];
	Word words[300];
	int numWords = loadWords(words);
	
	char playAgain = 'Y';
	int n;
	Round round;
	bool roundFinished = false;
	char guess;
	
	while (playAgain == 'Y'){
		initializeBoard(board);
		n = 1 + (rand() % 5);
		
		round.numWords = n;
		round.numWrongGuesses = 0;
		
		for (int i = 0; i < round.numWords;){
			int randWord = rand() % numWords;
			
			if (!words[randWord].seen){
				words[randWord].seen = true;
				round.actualWord[i] = words[randWord];
				round.hiddenWord[i] = words[randWord];
				initializeHiddenWord(round.actualWord[i].word, round.hiddenWord[i].word);
				++i;
			}
		}
		
		updateBoard(board, round);
		bool roundFinished = false;
		bool lost = false;
		bool won = false;
		int numWordsSolved;
		bool bodyPartDrawn[MAX_WRONG_GUESSES];
		int randomPart;
		int numLetters;
		
		for (int i = 0; i < MAX_WRONG_GUESSES; ++i)
			bodyPartDrawn[i] = false;
		
		numWordsSolved = 0;
		while (!roundFinished){
			if (round.numWrongGuesses == 6){
				lost = true;
				roundFinished = true;
				cout << "GAME OVER!" << endl;
			}
			
			if (round.numWords == numWordsSolved){
				roundFinished = true;
			}
			
			cout << "You solved " << numWordsSolved << " of " << round.numWords << " words" << endl;
			printBoard(board);
			
			for (int i = 0; i < round.numWords; ++i)
				cout << "The clue for word " << (i + 1) << " is: " << round.actualWord[i].category << endl;
			
			cout << endl;
			
			if ((won) || (lost)){
				cout << "The correct words are:" << endl;
				
				for (int i = 0; i < round.numWords; ++i)
					cout << round.actualWord[i].word << endl;
				
				break;
			}
			
			cout << "Guess a letter: ";
			cin >> guess;
			
			system("cls");
			
			Round tempRound = updateGuesses(round, guess);
			numWordsSolved = numSolvedWords(tempRound);
			if (round.numWrongGuesses < tempRound.numWrongGuesses) {
				bool isBodyPartDrawn = false;
				
				while (!isBodyPartDrawn){
					randomPart = rand() % MAX_WRONG_GUESSES;
					
					if (!bodyPartDrawn[randomPart]){
						bodyPartDrawn[randomPart] = true;
						isBodyPartDrawn = true;
						
						if (randomPart == 0)
							drawNeck(board);
						else if (randomPart == 1)
							drawLeftHand(board);
						else if (randomPart == 2)
							drawRightHand(board);
						else if (randomPart == 3)
							drawLeftFoot(board);
						else if (randomPart == 4)
							drawRightFoot(board);
						else if (randomPart == 5)
							drawHead(board);
					}
				}
				
				cout << "No " << guess << "'s found...try again!" << endl;
			} else {
				numLetters = numLettersFound(round, tempRound);
				
				if (round.numWords == numWordsSolved){
					won = true;
					cout << "You won!" << endl;
				} else
					cout << "Great " << numLetters << ' ' << guess << "'s found! keep going!" << endl;
			}
			
			round = tempRound;
			updateBoard(board, round);
		}
		
		cout << "Continue playing enter Y for Yes or N for No: ";
		cin >> playAgain;
		system("cls");
	}
	
	
	return 0;
}

