#include "../Sliding_puzzle_solver/Puzzle.h"
#include <conio.h>
using namespace std;

int main() {
	srand((unsigned)time(NULL));


	// standard game

	Puzzle puzzle = Puzzle (4);
	puzzle.shuffle();
	puzzle.print();

	bool show_sol = false;
	int ind = 0;
	vector <int> sol;

	while (1) {
        // move with arrow
        // space to show solution
        // space again to solve
        // r to reset
		switch(_getch()) {
		case 72:
			puzzle.move (1, 1);
			break;
		case 80:
			puzzle.move (1, -1);
			break;
		case 75:
			puzzle.move (-1, 1);
			break;
		case 77:
			puzzle.move (-1, -1);
			break;
		case ' ':
			system ("cls");
			if (show_sol) {
                puzzle.solve();
                show_sol = false;
			} else {
			    sol.clear();
                Puzzle cp = puzzle;
                cp.solve();
                sol = cp.solution;
                ind = 0;
                show_sol = true;
			}
			break;
		case 'r':
			puzzle.shuffle ();
			show_sol = false;
			break;
		default:
			continue;
		}
		system ("cls");
		puzzle.print ();
		if (puzzle.solved()) {
            cout << "YOU WIN!\n";
		} else if (show_sol) {
            if (ind != 0 && puzzle.nums[puzzle.pos[0]][puzzle.pos[1]] != sol[ind - 1]) {
                show_sol = false;
            } else {
                cout << "solution:\n";
                for (int i = ind; i < (int)sol.size(); i++) {
                    if (i != ind && (i - ind) % 10 == 0)
                        cout << endl;
                    cout << sol[i] << string (4 - to_string (sol[i]).size(), ' ');
                }
                ind++;
            }
		}
	}

	return 0;




	// tons of puzzle speed test
    /*
	Puzzle puzzle = Puzzle (4);
	int count = 1000000;
	auto start = std::chrono::system_clock::now ();
	while (count--) {
		puzzle.shuffle ();
		puzzle.solve ();
		if (!puzzle.solved ()) {
			puzzle.print ();
			cout << count << endl;
			break;
		}
		if (count % 1000 == 0)
			cout << count << endl;
	}
	auto end = std::chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	return 0;
    */


	// huge puzzle speed test
    /*
	Puzzle puzzle = Puzzle (100);
	cout << "Shuffling" << endl;;
	puzzle.shuffle ();
	auto start = std::chrono::system_clock::now ();
	puzzle.solve ();
	auto end = std::chrono::system_clock::now ();
	if (!puzzle.solved ()) {
		cout << "Not solved" << endl;
		return 0;
	}
	chrono::duration<double> elapsed_seconds = end - start;
	cout << "elapsed time: " << elapsed_seconds.count () << "s\n";

	return 0;
    */
}
