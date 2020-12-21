/*****************************************************************//**
 * \file   Reinforcment_Learning_Policy_Iteration.cpp
 * \brief  
 * Fun game where you play against the computer that utilizes Policy Iteration.
 * \author tyrre
 * \date   November 2020
 *********************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int  score;
char user_input[20];

#define USER 0
#define COMPUTER 1

#define MOVE_OPTIONS 2		// Configure the game to allow different valid moves (for fun)

#define LARGE_NUM 10000		// Arbitrary large number used to initilize values
#define GAMMA 0.5			// Discount rate
#define WINNING_SCORE 20	// User who gets above this value first wins
#define REWARD 10		

/// <summary>
/// Take input from user for their turn
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int user(void)
{
	int i = 0;
	while (i <= 0 || i > MOVE_OPTIONS) {
		printf("We are at %-2d, make your move user: ", score);
		fgets(user_input, 10, stdin);
		i = atoi(user_input);
	};
	return(i);
}
/// <summary>
/// Recursively go through all possible move combinations, backpropagating the reward as you do so
/// </summary>
/// <param name="depth">Used to determine discount applied to gamma. 
///						Represents recursion depth (i.e. # of times reward has been called)</param>
/// <param name="turn"></param>
/// <returns></returns>
float reward(int depth, int turn) {

	// Check score for the last move made (e.g. if turn == COMPUTER, the last move was made by 
	// the user, and they would be the winner if score >= WINNING_SCORE)
	if (score >= WINNING_SCORE && turn == COMPUTER) return -REWARD;		// User wins
	if (score >= WINNING_SCORE && turn == USER)     return REWARD;		// Computer wins

	float tmp, max = 0;
	float discounted_gamma = pow(GAMMA, depth);

	if (turn == COMPUTER) {
		max = -LARGE_NUM;
		for (int i = 1; i <= MOVE_OPTIONS; i++) {
			score += i;								// Make move
			tmp = reward(depth + 1, USER);
			if (tmp > max)							// > since we want large reward for computer; means we are winning
				max = tmp;

			score -= i;								// Undo move
		}

	}
	else if (turn == USER) {
		max = LARGE_NUM;
		for (int i = 1; i <= MOVE_OPTIONS; i++) {
			score += i;								// Make move
			tmp = reward(depth + 1, COMPUTER);
			if (tmp < max)							// < because we want minimal reward for user
				max = tmp;

			score -= i;								// Undo move
		}
	}
	return(discounted_gamma * max);
}
/// <summary>
/// Computer makes their move
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int computer(void)
{
	float bestVal = -LARGE_NUM;
	int bestMove = 0;

	for (int i = 1; i <= MOVE_OPTIONS; i++) {
		// Evaluate i as a move
		score += i;
		float moveVal = reward(0, USER);
		printf("Move %d  yields a reward of: %f \n", i, moveVal);
		score -= i;		// Undo move

		if (moveVal > bestVal) {
			bestMove = i;
			bestVal = moveVal;
		}
	}
	printf("We are at %-2d Computer adds %d \n", score, bestMove);

	return bestMove;
}
/*
int computer2(void)
{
	if (score % 3 == 1)
		return(1);
	if (score % 3 == 0)
		return(2);
	else
		return(rand() % 2 + 1);
}
*/
int main(void)
{
	srand(time(NULL));
	int i;

	printf("Who says 20 (or greater) first! \n \n");
	score = 0;
	i = 0;

	while (i != 1 && i != 2) {
		printf("Who goes first: you = 1 computer = 2 ? ");
		fgets(user_input, 10, stdin);
		i = atoi(user_input);
	};
	if (i == 2)
		score = computer();

	while (score <= 20) {
		score = score + user();
		if (score >= 20) { 
			printf(" YOU WIN !!\n "); 
			break; 
		};

		score = score + computer();
		if (score >= 20) { 
			printf(" COMPUTER WIN !! \n "); 
			break; 
		};
	};
	return(0);
};
