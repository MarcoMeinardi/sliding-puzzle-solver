#include "Puzzle.h"

Puzzle::Puzzle (int n) {
    N = n;
    nums = vector <vector <int>> (N, vector <int> (N));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			nums[i][j] = i * N + j + 1;
		}
	}
	pos = {N - 1, N - 1};
}

void Puzzle::move (int direction, int verse) {
	if (direction == 1) {
		if (verse == 1 && pos[0] != N - 1) {						//up
			solution.push_back (nums[pos[0] + 1][pos[1]]);
			nums[pos[0]][pos[1]] = nums[pos[0] + 1][pos[1]];
			pos[0]++;
		}
		else if (verse == -1 && pos[0] != 0) {						//down
			solution.push_back (nums[pos[0] - 1][pos[1]]);
			nums[pos[0]][pos[1]] = nums[pos[0] - 1][pos[1]];
			pos[0]--;
		}
	}
	else if (direction == -1) {
		if (verse == 1 && pos[1] != N - 1) {						//left
			solution.push_back (nums[pos[0]][pos[1] + 1]);
			nums[pos[0]][pos[1]] = nums[pos[0]][pos[1] + 1];
			pos[1]++;
		}
		else if (verse == -1 && pos[1] != 0) {						//right
			solution.push_back (nums[pos[0]][pos[1] - 1]);
			nums[pos[0]][pos[1]] = nums[pos[0]][pos[1] - 1];
			pos[1]--;
		}
	}
}

bool Puzzle::solved () {
	if (pos[1] != N - 1 || pos[0] != N - 1) {
		return false;
	}
	for (int i = 0; i < N * N - 1; i++) {
		if (nums[(int)i / N][i % N] != i + 1) {
			return false;
		}
	}
	return true;
}

// https://www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html
bool Puzzle::solvable () {
    int inv = 0;
    for (int i = 0; i < N * N; i++) {
        if ((int)(i / N) == pos[0] && (i % N) == pos[1]) {
			continue;
		}
		for (int j = i + 1; j < N * N; j++) {
			if (((int)(j / N) != pos[0] || (j % N) != pos[1]) && nums[(int)(i / N)][i % N] > nums[(int)(j / N)][j % N]) {
				inv++;
			}
		}
    }

    return (N % 2 == 1 && inv % 2 == 0) || (N % 2 == 0 && ((pos[0] % 2 == 1) == (inv % 2 == 0)));
}

void Puzzle::shuffle () {
    do {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int y = rand() % N, x = rand() % N;
                if (i == pos[0] && j == pos[1]) {
                    pos[0] = y;
                    pos[1] = x;
                } else if (y == pos[0] && x == pos[1]) {
                    pos[0] = i;
                    pos[1] = j;
                }
                swap (nums[i][j], nums[y][x]);
            }
        }
    } while (solved () || !solvable ());
}

void Puzzle::print () {
	cout << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i != pos[0] || j != pos[1]) {
				cout << nums[i][j] << "\t";
			}
			else {
				cout << "\t";
			}
		}
		cout << "\n\n";
	}
	cout << endl;
}



void Puzzle::solve () {
	solution = {};
	cout << "Solving free layers" << endl;
	solve_free_layers ();
	cout << "Solving last two layers" << endl;
	solve_left_side ();
	solve_2_2 ();
	cout << "solved";
}

void Puzzle::solve_free_layers () {
	int n;
	for (int layer = 0; layer < N - 2; layer++) {
		move_num (1 + layer * N, layer, 0, 0);
		for (int i = 1; i < N - 2; i++) {
			if (nums[layer][i] != i + 1 || (pos[0] == layer && pos[1] == i)) {
				n = layer * N + i + 1;
				array <int, 2> num = find_num (n);
				if (num[0] != layer && pos[0] == layer) {
					move (1, 1);
				}
				if (num[0] == layer + 1 && num[1] <= i && pos[0] == layer + 1 && pos[1] < num[1]) {
					move (1, 1);
				}
				if (num[0] == layer + 1 && num[1] < i) {
					move_num (n, layer + 1, i, 0);
					move (1, 1);
				}
				move_num (n, layer, i, 1);
			}
		}
		n = (layer + 1) * N;
		if (nums[layer][N - 2] != n - 1 || nums[layer][N - 1] != n) {
			array <int, 2> last = find_num (n);
			if (last[0] != layer && pos[0] == layer) {
				move (1, 1);
			}
			if (last[0] == layer + 1 && last[1] < N - 1) {
				move_num (n, layer + 1, N - 2, 1);
				move (1, 1);
			}
			move_num (n, layer, N - 2, 1);

			if (nums[layer + 1][N - 1] == n - 1 && pos[0] == layer) {
				move (1, 1);
			}
			if (nums[layer][N - 2] == n && nums[layer][N - 1] == n - 1 && pos[0] != layer) {
				swap_near ({layer, N - 2}, 1);
			}
			else {
				move_num (n - 1, layer + 1, N - 2, 0);
				move_hole ({layer + 1, N - 2}, {layer + 1, N - 1}, 1);
				move (1, -1);
				move (-1, -1);
				move (1, 1);
			}
		}
	}
}

void Puzzle::solve_left_side () {
	for (int layer = 0; layer < N - 2; layer++) {
		int n1 = N * (N - 2) + layer + 1, n2 = N * (N - 1) + layer + 1;
		if (pos[1] == layer || nums[N - 2][layer] != n1 || nums[N - 1][layer] != n2) {
			move_num (n2, N - 2, layer, 1);
			if (pos[1] == layer && nums[N - 1][layer + 1] == n1) {
				move (-1, 1);
			}
			if (nums[N - 1][layer] == n1 && pos[1] != layer) {
				swap_near ({N - 2, layer}, -1);
			}
			else {
				move_num (n1, N - 2, layer + 1, 1);
				move_hole ({N - 2, layer + 1}, {N - 1, layer}, 0);
				move (1, -1);
				move (-1, 1);
			}
		}
	}
}

void Puzzle::solve_2_2 () {
	move_hole ({0, 0}, {N - 1, N - 1}, 0);
	int count = 0;
	while (nums[N - 2][N - 1] != N * (N - 1)) {
		move (1, -1);
		move (-1, -1);
		move (1, 1);
		move (-1, 1);
		if (count++ > 3) {
			solution = {};
			break;
		}
	}
}

void Puzzle::move_num (int num, int Y, int X, int hor_vert) {
	array <int, 2> target = {Y, X};
	array <int, 2> coord = find_num (num);
	if (coord[0] == Y && coord[1] == X) {
		return;
	}

	array <int ,2> hole;
	if (target[hor_vert] > coord[hor_vert]) {
		hole[!hor_vert] = coord[!hor_vert];
		hole[hor_vert] = coord[hor_vert] + 1;
	}
	else if (target[hor_vert] < coord[hor_vert]) {
		hole[!hor_vert] = coord[!hor_vert];
		hole[hor_vert] = coord[hor_vert] - 1;
	}
	else if (target[!hor_vert] > coord[!hor_vert]) {
		hole[!hor_vert] = coord[!hor_vert] + 1;
		hole[hor_vert] = coord[hor_vert];
	}
	else if (target[!hor_vert] < coord[!hor_vert]) {
		hole[!hor_vert] = coord[!hor_vert] - 1;
		hole[hor_vert] = coord[hor_vert];
	}
	else {
		goto hole_right;
	}

	move_hole (coord, hole, hor_vert);

	hole_right: int verse;
	if (pos[hor_vert] != coord[hor_vert]) {
		verse = pos[hor_vert] > coord[hor_vert] ? -1 : 1;
		move (hor_vert  ? -1 : 1, verse);
		coord[hor_vert] -= verse;
		while (coord[hor_vert] != target[hor_vert]) {
			move_piece (hor_vert ? 1 : -1, verse, pos[!hor_vert] == N - 1 ? -1 : 1);
			coord[hor_vert] -= verse;
		}
		if (coord[!hor_vert] != target[!hor_vert]) {
			move (hor_vert ? 1 : -1, target[!hor_vert] > coord[!hor_vert] ? 1 : -1);
			move (hor_vert  ? -1 : 1, pos[hor_vert] > coord[hor_vert] ? -1 : 1);
		}
	}

	if (coord[!hor_vert] != target[!hor_vert]) {
		verse = pos[!hor_vert] > coord[!hor_vert] ? -1 : 1;
		move (hor_vert ? 1 : -1, verse);
		coord[!hor_vert] -= verse;
		while (coord[!hor_vert] != target[!hor_vert]) {
			move_piece (hor_vert ? -1 : 1, verse, pos[hor_vert] == N - 1 ? -1 : 1);
			coord[!hor_vert] -= verse;
		}
	}
}

array <int, 2> Puzzle::find_num (int n) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if ((i != pos[0] || j != pos[1]) && nums[i][j] == n) {
				return {i, j};
			}
		}
	}
	return {-1, -1};
}

void Puzzle::move_piece (int x_y, int first_prior, int second_prior) {
	move (x_y, second_prior);
	move (-x_y, -first_prior);
	move (-x_y, -first_prior);
	move (x_y, -second_prior);
	move (-x_y, first_prior);
}

void Puzzle::move_hole (array <int, 2> dodge, array <int, 2> hole, int hor_vert) {
	int verse = pos[hor_vert] > hole[hor_vert] ? -1 : 1;
	bool shifted = false, opposite = false;
	while (pos[hor_vert] != hole[hor_vert]) {
		if (pos[hor_vert] + verse == dodge[hor_vert] && pos[!hor_vert] == dodge[!hor_vert]) {
			opposite = pos[!hor_vert] == 0 || (pos[!hor_vert] <= hole[!hor_vert] && pos[!hor_vert] != N - 1);
			move (hor_vert ? 1 : -1, opposite ? 1 : -1);
			shifted = true;
		}
		move (hor_vert ? -1 : 1, verse);
	}
	if (shifted && pos[hor_vert] != dodge[hor_vert]) {
		move (hor_vert ? 1 : -1, opposite ? -1 : 1);
	}

	verse = pos[!hor_vert] > hole[!hor_vert] ? -1 : 1;
	shifted = false;
	opposite = false;
	while (pos[!hor_vert] != hole[!hor_vert]) {
		if (pos[!hor_vert] + verse == dodge[!hor_vert] && pos[hor_vert] == dodge[hor_vert]) {
			opposite = pos[hor_vert] == 0 || (pos[hor_vert] <= hole[hor_vert] && pos[hor_vert] != N - 1);
			move (hor_vert ? -1 : 1, opposite ? 1 : -1);
			shifted = true;
		}
		move (hor_vert ? 1 : -1, verse);
	}
	if (shifted && pos[!hor_vert] != dodge[!hor_vert]) {
		move (hor_vert ? -1 : 1, opposite ? -1 : 1);
	}
}

void Puzzle::swap_near (array <int, 2> left_or_up, int hor_vert) {
	move_hole ({0, 0}, {left_or_up[0] + 1, left_or_up[1] + 1}, 1);
	move ( hor_vert, -1); move (-hor_vert, -1); move ( hor_vert,  1); move ( hor_vert,  1);
	move (-hor_vert,  1); move ( hor_vert, -1); move (-hor_vert, -1); move ( hor_vert, -1);
	move (-hor_vert,  1); move ( hor_vert,  1); move ( hor_vert,  1); move (-hor_vert, -1);
	move ( hor_vert, -1); move (-hor_vert,  1); move ( hor_vert, -1); move (-hor_vert, -1);
	move ( hor_vert,  1);
}
