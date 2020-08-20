#ifndef PUZZLE_H_INCLUDED
#define PUZZLE_H_INCLUDED


#include<bits/stdc++.h>

using namespace std;

class Puzzle {
public:
    int N;
	vector <vector <int>> nums;
	array <int, 2> pos;

//public:
	vector <int> solution;

	Puzzle (int n);
	void move (int direction, int verse);
	bool solved ();
	bool solvable ();
	void shuffle ();
	void print ();

	void solve ();
	void solve_free_layers ();
	void solve_left_side ();
	void solve_2_2 ();
	void move_num (int num, int Y, int X, int hor_vert);

	array <int, 2> find_num (int n);
	void move_piece (int x_y, int first_prior, int second_prior);
	void move_hole (array <int, 2> dodge, array <int, 2> hole, int hor_vert);
	void swap_near (array <int, 2> left_or_up, int hor_vert);
};


#endif // PUZZLE_H_INCLUDED
