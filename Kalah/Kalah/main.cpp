//
//  main.cpp
//  Kalah
//
//  Created by Zhou Ye on 9/24/14.
//  Copyright (c) 2014 ZhouYe. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

class GameState{
    
private:
    int* yourHoles;
    int* oppoHoles;
    int yourKalah;
    int oppoKalah;
    /*
     Randomly split integer into several parts
     n: number to split
     m: number of parts
     */
    int* randomSplit(int n, int m) {
        int target = n;
        int* result = new int[m];
        int part1;
        int part2;
        for (int i=0; i<m-1; i++) {
            part1 = rand()%target; //random split
            part2 = target-part1; //number after split
            result[i] = min(part1, part2); //store the smaller part
            target = max(part1, part2); //further split on the larger part
        }
        //the reason I split on the larger one is to make the distribution more uniform
        result[m-1] = target; //no need to split the last one
        return result;
    }
    /*
     Sum up six holes
     */
    int sumUpHoles(int* holes) {
        int sum = 0;
        for (int i=0; i<sizeof(holes); i++) {
            sum += holes[i];
        }
        return sum;
    }
    /*Move back the rest of seeds when game is over*/
    void clearUp() {
        for (int i=1; i<=6; i++) {
            if (yourHoles[i-1]>0) {
                yourKalah += yourHoles[i-1];
                yourHoles[i-1] = 0;
            }
            if (oppoHoles[i-1]>0) {
                oppoKalah += oppoHoles[i-1];
                oppoHoles[i-1] = 0;
            }
        }
    }
    
public:
    /*Get Your hole 1 to 6*/
    int getYourHoles(int index) {
        return yourHoles[index-1];
    }
    /*Get opponent hole 1 to 6*/
    int getOppoHoles(int index) {
        return oppoHoles[index-1];
    }
    /*Get your Kalah number*/
    int getYourKalah() {
        return yourKalah;
    }
    /*Get opponent Kalah number*/
    int getOppoKalah() {
        return oppoKalah;
    }
    /*Initialize game board*/
    void initialize() {
        yourHoles = randomSplit(30, 6);
        oppoHoles = randomSplit(30, 6);
        yourKalah = 0;
        oppoKalah = 0;
    }
    /*Check if game is over*/
    bool checkOver() {
        if (sumUpHoles(yourHoles)==0 || sumUpHoles(oppoHoles)==0) {
            clearUp();
            return true;
        }
        else {
            return false;
        }
    }
    /*
     Sow seeds
     player: you/opponent
     index: the hole number to sow
     return: additional move or not
     */
    bool sowSeed(bool player, int index) {
        if (player) { //you
            bool lastPlayer; //the player at last seed
            int lastIndex; //the hole at last seed
            int number = getYourHoles(index); //number of seeds
            if (number==0) {
                cout << "Empty Move!" << endl;
            }
            yourHoles[index-1] = 0; //move the seeds
            if (number>6-index) {
                for (int i=1; i<=6-index; i++) {
                    yourHoles[index+i-1] += 1;
                }
                int round = (number-(6-index))/13; //number of loops from Kalah
                int addition = (number-(6-index))%13; //additional step
                if (round>0 && addition>0) { //large number
                    for (int i=1; i<=round; i++) {
                        yourKalah += 1;
                        for (int j=1; j<=6; j++) {
                            oppoHoles[j-1] += 1;
                        }
                        for (int j=1; j<=6; j++) {
                            yourHoles[j-1] += 1;
                        }
                    }
                    for (int i=1; i<=addition; i++) {
                        if (i==1) {
                            yourKalah += 1;
                        }
                        else if (i>1 && i<=7) {
                            oppoHoles[i-1-1] += 1;
                        }
                        else {
                            yourHoles[i-7-1] += 1;
                        }
                    }
                    if (addition==1) {
                        lastPlayer = player;
                        lastIndex = 7;
                    }
                    else if (addition>1 && addition<=7) {
                        lastPlayer = !player;
                        lastIndex = addition-1;
                    }
                    else {
                        lastPlayer = player;
                        lastIndex = addition-7;
                    }
                }
                else if (round==0 && addition>0){ //small number
                    for (int i=1; i<=addition; i++) {
                        if (i==1) {
                            yourKalah += 1;
                        }
                        else if (i>1 && i<=7) {
                            oppoHoles[i-1-1] += 1;
                        }
                        else {
                            yourHoles[i-7-1] += 1;
                        }
                    }
                    if (addition==1) {
                        lastPlayer = player;
                        lastIndex = 7;
                    }
                    else if (addition>1 && addition<=7) {
                        lastPlayer = !player;
                        lastIndex = addition-1;
                    }
                    else {
                        lastPlayer = player;
                        lastIndex = addition-7;
                    }
                }
                else { //very rare case
                    for (int i=1; i<=addition; i++) {
                        if (i==1) {
                            yourKalah += 1;
                        }
                        else if (i>1 && i<=7) {
                            oppoHoles[i-1-1] += 1;
                        }
                        else {
                            yourHoles[i-7-1] += 1;
                        }
                    }
                    lastPlayer = player;
                    lastIndex = 6;
                }
            }
            else {
                for (int i=1; i<=number; i++) {
                    yourHoles[index+i-1] += 1;
                }
                lastPlayer = player;
                lastIndex = index+number;
            }
            if (lastPlayer==player && getYourHoles(lastIndex)==1 && getOppoHoles(7-lastIndex)>0) {
                yourKalah += yourHoles[lastIndex-1]+oppoHoles[6-lastIndex]; //capture both seeds
                yourHoles[lastIndex-1] = 0;
                oppoHoles[6-lastIndex] = 0;
            }
            if (lastPlayer==player && lastIndex==7) {
                return true;
            }
            else {
                return false;
            }
        }
        else { //opponent
            bool lastPlayer; //the player at last seed
            int lastIndex; //the hole at last seed
            int number = getOppoHoles(index); //number of seeds
            if (number==0) {
                cout << "Empty Move!" << endl;
            }
            oppoHoles[index-1] = 0; //move the seeds
            if (number>6-index) {
                for (int i=1; i<=6-index; i++) {
                    oppoHoles[index+i-1] += 1;
                }
                int round = (number-(6-index))/13; //number of loops from Kalah
                int addition = (number-(6-index))%13; //additional step
                if (round>0 && addition>0) { //large number
                    for (int i=1; i<=round; i++) {
                        oppoKalah += 1;
                        for (int j=1; j<=6; j++) {
                            yourHoles[j-1] += 1;
                        }
                        for (int j=1; j<=6; j++) {
                            oppoHoles[j-1] += 1;
                        }
                    }
                    for (int i=1; i<=addition; i++) {
                        if (i==1) {
                            oppoKalah += 1;
                        }
                        else if (i>1 && i<=7) {
                            yourHoles[i-1-1] += 1;
                        }
                        else {
                            oppoHoles[i-7-1] += 1;
                        }
                    }
                    if (addition==1) {
                        lastPlayer = player;
                        lastIndex = 7;
                    }
                    else if (addition>1 && addition<=7) {
                        lastPlayer = !player;
                        lastIndex = addition-1;
                    }
                    else {
                        lastPlayer = player;
                        lastIndex = addition-7;
                    }
                }
                else if (round==0 && addition>0){ //small number
                    for (int i=1; i<=addition; i++) {
                        if (i==1) {
                            oppoKalah += 1;
                        }
                        else if (i>1 && i<=7) {
                            yourHoles[i-1-1] += 1;
                        }
                        else {
                            oppoHoles[i-7-1] += 1;
                        }
                    }
                    if (addition==1) {
                        lastPlayer = player;
                        lastIndex = 7;
                    }
                    else if (addition>1 && addition<=7) {
                        lastPlayer = !player;
                        lastIndex = addition-1;
                    }
                    else {
                        lastPlayer = player;
                        lastIndex = addition-7;
                    }
                }
                else { //very rare case
                    for (int i=1; i<=addition; i++) {
                        if (i==1) {
                            oppoKalah += 1;
                        }
                        else if (i>1 && i<=7) {
                            yourHoles[i-1-1] += 1;
                        }
                        else {
                            oppoHoles[i-7-1] += 1;
                        }
                    }
                    lastPlayer = player;
                    lastIndex = 6;
                }
            }
            else {
                for (int i=1; i<=number; i++) {
                    oppoHoles[index+i-1] += 1;
                }
                lastPlayer = player;
                lastIndex = index+number;
            }
            if (lastPlayer==player && getOppoHoles(lastIndex)==1 && getYourHoles(7-lastIndex)>0) {
                oppoKalah += oppoHoles[lastIndex-1]+yourHoles[6-lastIndex]; //capture both seeds
                oppoHoles[lastIndex-1] = 0;
                yourHoles[6-lastIndex] = 0;
            }
            if (lastPlayer==player && lastIndex==7) {
                if (player && sumUpHoles(yourHoles)==0) {
                    return false; //game should end
                }
                else if (!player && sumUpHoles(oppoHoles)==0) {
                    return false; //game should end
                }
                else {
                    return true;
                }
            }
            else {
                return false;
            }

        }
    }
};

// class Kalah: Kalah game main class
class Kalah{
private:
    bool player; //true: you; false: opponent
    /*
     Game Start
     two human roll the dice; who has larger number starts first.
     */
    void gameStart() {
        int yourDice = 0;
        int oppoDice = 0;
        while (yourDice==oppoDice) {
            cout << "You Roll The Dice:" << endl;
            yourDice = rand()%6+1;
            cout << "You Get " << yourDice << endl;
            cout << "Opponent Roll The Dice:" << endl;
            oppoDice = rand()%6+1;
            cout << "Opponent Get " << oppoDice << endl;
        }
        if (yourDice>oppoDice) {
            cout << "You Move First!" << endl;
            player = true;
        }
        else {
            cout << "Opponent Move First!" << endl;
            player = false;
        }
    }
    
    void printState(GameState state) {
        cout << "Opponent" << endl;
        cout << "  -------------  " << endl;
        cout << " |";
		for(int i=6; i>=1; i--){
            cout << state.getOppoHoles(i) << " ";
		}
        cout << "| " << endl;
        cout << state.getOppoKalah() << "|-------------|" << state.getYourKalah() << endl;
        cout << " |";
		for(int i=1; i<=6; i++){
			cout << state.getYourHoles(i) << " ";
		}
        cout << "| " << endl;
        cout << "  -------------  " << endl;
		cout << "You" << endl;
	}
	
	int inputHoleNumber(bool player) {
        if (player) {
            cout << "Your Move" << endl;
        }
        else {
            cout << "Opponent Move" << endl;
        }
		int hole;
        cout << "Please enter the hole number (1/2/3/4/5/6)" << endl;
		cin >> hole;
        while (hole<1 || hole>6) {
            cout << "Invalid hole number!" << endl;
            cout << "Please enter the hole number (1/2/3/4/5/6)" << endl;
            cin >> hole;
        }
		return hole;
	}
    
	bool startGameAgain() {
        char restart;
		cout << "Replay the game? (y/n)" << endl;
		cin >> restart;
        while (restart!='y' && restart!='n') {
            cout << "Invalid input" << endl;
            cout << "Replay the game? (y/n)" << endl;
            cin >> restart;
        }
		if (restart=='y') {
            return true;
        }
		else {
            return false;
        }
	}
    
public:
    static void main() {
        Kalah game;
        GameState state;
        game.gameStart(); //start the game
        state.initialize(); //initialize the board
        game.printState(state);
        while (!state.checkOver()) {
            int holeNumber = game.inputHoleNumber(game.player);
            bool move = state.sowSeed(game.player, holeNumber);
            game.printState(state);
            while (move) { //additional move
                cout << "Additional Move!" << endl;
                holeNumber = game.inputHoleNumber(game.player);
                move = state.sowSeed(game.player, holeNumber);
                game.printState(state);
            }
            game.player = !game.player; //change player
        }
        game.printState(state);
        if (state.getYourKalah()>state.getOppoKalah()) {
            cout << "You Win!" << endl;
        }
        else if (state.getYourKalah()<state.getOppoKalah()) {
            cout << "Opponent Win!" << endl;
        }
        else {
            cout << "Game Tie!" << endl;
        }
        bool restart = game.startGameAgain();
        if (restart) {
            main(); //play it again!
        }
    }
    
};

int main() {
    srand(static_cast<unsigned int>(time(NULL))); //generate random seed
    Kalah::main();
    return 0;
}
