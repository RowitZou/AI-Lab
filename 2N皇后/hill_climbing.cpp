#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <ctime>
#include <cmath>
#include <fstream>
#include <stdint.h>
#include <iomanip>

using namespace std;

vector<int64_t> whiteQueen;

int64_t bound;

vector<int64_t> init_status(int64_t n)
{
	vector<int64_t> status = vector<int64_t>(n);
	vector<bool> diagonalstatus1 = vector<bool>(2 * n - 1, false);
	vector<bool> diagonalstatus2 = vector<bool>(2 * n - 1, false);
	int64_t random;
	
	for (int64_t i = 0; i < n; ++i) {
		status[i] = i;
	}
	for (int64_t i = 0; i < bound; ++i) {
		do {
			#if (defined _WIN64) || (defined _WIN32)
			random = (rand() * RAND_MAX + rand()) % (n - i) + i;
			#else
			random = rand() % (n - i) + i;
			#endif
		} while (diagonalstatus1[n - 1 + status[random] - i] || 
			diagonalstatus2[status[random] + i] ||
			status[random] == whiteQueen[i] || 
			status[i] == whiteQueen[random]);
		swap(status[random], status[i]);
		diagonalstatus1[n - 1 + status[i] - i] = true;
		diagonalstatus2[i + status[i]] = true;
	}
	for (int64_t i = bound; i < n; ++i) {
		do {
			#if (defined _WIN64) || (defined _WIN32)
			random = (rand() * RAND_MAX + rand()) % (n - i) + i;
			#else
			random = rand() % (n - i) + i;
			#endif
		} while (status[random] == whiteQueen[i] || 
			status[i] == whiteQueen[random]);
		swap(status[random], status[i]);
	}
	return status;
}

int64_t initDianogal(const vector<int64_t> &status, vector<int64_t> &diagonal1, vector<int64_t> &diagonal2) 
{
	for (int64_t i = 0; i < status.size(); ++i) {
		diagonal1[status.size() - 1 + status[i] - i]++;
		diagonal2[i + status[i]]++;
	}
	int64_t num = 0;
	for(int64_t i = 0; i < diagonal2.size(); ++i){
		num += ((diagonal2[i] * (diagonal2[i] - 1)) / 2 + (diagonal1[i] * (diagonal1[i] - 1)) / 2);
	}
	return num;
}

void updateDianogal(vector<int64_t> &status, const int64_t i, const int64_t j, 
	vector<int64_t> &diagonal1, vector<int64_t> &diagonal2) 
{
	--diagonal1[status.size() - 1 + status[j] - j];
	--diagonal2[j + status[j]];
	--diagonal1[status.size() - 1 + status[i] - i];
	--diagonal2[i + status[i]];
	swap(status[i], status[j]);
	++diagonal1[status.size() - 1 + status[j] - j];
	++diagonal2[j + status[j]];
	++diagonal1[status.size() - 1 + status[i] - i];
	++diagonal2[i + status[i]];
}

int64_t h(const vector<int64_t> &status) 
{
	int64_t num = 0;
	for (int64_t i = 0; i < status.size(); ++i){
		for (int64_t j = i + 1; j < status.size(); ++j) 
			if (abs(status[i] - status[j]) == j - i) 
				num++;
	}
	return num;
}

int64_t newh(vector<int64_t> &status, int64_t i, int64_t j, int64_t oldh, 
	vector<int64_t> &diagonal1, vector<int64_t> &diagonal2) 
{
	int64_t count = 0;
	count -= (diagonal1[status.size() - 1 + status[i] - i] + diagonal2[i + status[i]]);
	count -= (diagonal1[status.size() - 1 + status[j] - j] + diagonal2[j + status[j]]);
	updateDianogal(status, i, j, diagonal1, diagonal2);

	count += (diagonal1[status.size() - 1 + status[i] - i] + diagonal2[i + status[i]]);
	count += (diagonal1[status.size() - 1 + status[j] - j] + diagonal2[j + status[j]]);

	return oldh + count;
}

bool climb(vector<int64_t> &status)
{
	vector<int64_t> diagonal1(2 * status.size() - 1, 0);
	vector<int64_t> diagonal2(2 * status.size() - 1, 0);
	int64_t hValue = initDianogal(status, diagonal1, diagonal2);
	int64_t best_h = hValue;
	int64_t best_i, best_j, count = 0;
	
	while(hValue > 0 && count < log(status.size())){
		best_i = 0;
		best_j = 0;
		for (int64_t i = bound; i < status.size(); ++i) {
			if (diagonal1[status.size() - 1 + status[i] - i] == 1 && diagonal2[i + status[i]] == 1)
				continue;
			for (int64_t j = 0; j < status.size(); ++j) {
				if (i == j || status[i] == whiteQueen[j] || status[j] == whiteQueen[i])
					continue;
				int64_t next_h = newh(status, i, j, hValue, diagonal1, diagonal2);
				if (next_h < best_h) {
					best_h = next_h;
					best_i = i;
					best_j = j;
					goto next;
				}
				updateDianogal(status, i, j, diagonal1, diagonal2);
				if (next_h == best_h && rand() % 2 == 0) {
					best_i = i;
					best_j = j;
				}
			}
		}
		next:
		if (best_i == 0 && best_j == 0)
			return false;
		if (hValue != best_h)
			count = 0;
		else{
			updateDianogal(status, best_i, best_j, diagonal1, diagonal2);
			++count;
		}
		hValue = best_h;
	}
	if (hValue == 0) return true;
	return false;
}

int main() 
{
	int64_t n;
	fstream fin, fout;
	fin.open("input.txt", ios::in);
	fin >> n;
	fin.close();
	whiteQueen = vector<int64_t>(n, -1);
	vector<int64_t> status;
	clock_t start, finish;
	if(n > 5000)
		bound = n - sqrt(n);
	else
		bound = 0;

	srand((unsigned)time(NULL));	
	start = clock();
	bool success = false;
	while (!success) {
		status = init_status(n);
		success = climb(status);
	}
	whiteQueen = status;
	if (n % 2 == 0) {
		for (int64_t i = 0; i < status.size(); ++i) {
			status[i] = status.size() - status[i] - 1;
		}
	}
	else {
		success = false;
		while (!success) {
			status = init_status(n);
			success = climb(status);
		}
	}
	finish = clock();
	fout.open("output_hill_climbing.txt", ios::out);
	double totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	for (int64_t i = 0; i < whiteQueen.size(); ++i)
		fout << whiteQueen[i] + 1 << endl;
	cout << endl;
	for (int64_t i = 0; i < status.size(); ++i)
		fout << status[i] + 1 << endl;
	fout.setf(ios::fixed);
    fout << setprecision(3) << totaltime << endl;
	fout.close();
}
