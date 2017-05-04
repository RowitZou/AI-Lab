#include <iostream>
#include <algorithm>
#include <queue>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <set>
#include <cmath>
#include <ctime>
#include <iomanip>
#define INF 0x3F3F3F3F

using namespace std;

struct queueStruct {
	string s;	//status
	int first;	//h()
	int second; //g()
	string mov;	//record sequence of moving 
};
priority_queue<queueStruct> statusAStar;
string final_status;

bool operator<(const queueStruct &a,
	const queueStruct &b) {
	if (a.first + a.second == b.first + b.second)
		return a.first > b.first;
	return (a.first + a.second) > (b.first + b.second);
}

bool input(int(*f)(string)) {

	queueStruct init_pair;
	string init_status;
	fstream fin1, fin2;
	int number1, number2, count = 0;
	fin1.open("input.txt", ios::in);
	fin2.open("target.txt", ios::in);
	if (!fin1 || !fin2) {
		cout << "Can not open file!" << endl;
		return false;
	}
	while (count < 27) {
		fin1 >> number1;
		fin2 >> number2;
		if((number1 == -1 || number2 == -1) && number1 != number2){
			cout << "Invalid input!" << endl;
			return false;
		}
		init_status.push_back((char)(number1 + 50));
		final_status.push_back((char)(number2 + 50));
		++count;
	}
	init_pair.s = init_status;
	init_pair.first = f(init_status);
	init_pair.second = 0;
	init_pair.mov = "";
	statusAStar.push(init_pair);
	fin1.close();
	fin2.close();
	return true;
}

vector<pair<string, string> > nextMove(queueStruct qS) {
	
	string curr_status = qS.s;
	vector<pair<string, string> > moving;
	int index = (int)curr_status.find_first_of(char(50));

	if (index - 9 >= 0 && curr_status[index - 9] != char(49)) {
		string next_status = curr_status;
		next_status[index] = next_status[index - 9];
		next_status[index - 9] = char(50);
		pair<string, string> moving_element;
		moving_element.second = qS.mov;
		moving_element.second.append("F");
		moving_element.first = next_status;
		moving.push_back(moving_element);
	}
	if (index + 9 < 27 && curr_status[index + 9] != char(49)) {
		string next_status = curr_status;
		next_status[index] = next_status[index + 9];
		next_status[index + 9] = char(50);
		pair<string, string> moving_element;
		moving_element.second = qS.mov;
		moving_element.second.append("B");
		moving_element.first = next_status;
		moving.push_back(moving_element);
	}
	if ((index % 9) - 3 >= 0 && curr_status[index - 3] != char(49)) {
		string next_status = curr_status;
		next_status[index] = next_status[index - 3];
		next_status[index - 3] = char(50);
		pair<string, string> moving_element;
		moving_element.second = qS.mov;
		moving_element.second.append("U");
		moving_element.first = next_status;
		moving.push_back(moving_element);
	}
	if ((index % 9) + 3 < 9 && curr_status[index + 3] != char(49)) {
		string next_status = curr_status;
		next_status[index] = next_status[index + 3];
		next_status[index + 3] = char(50);
		pair<string, string> moving_element;
		moving_element.second = qS.mov;
		moving_element.second.append("D");
		moving_element.first = next_status;
		moving.push_back(moving_element);
	}
	if ((index % 3) - 1 >= 0 && curr_status[index - 1] != char(49)) {
		string next_status = curr_status;
		next_status[index] = next_status[index - 1];
		next_status[index - 1] = char(50);
		pair<string, string> moving_element;
		moving_element.second = qS.mov;
		moving_element.second.append("L");
		moving_element.first = next_status;
		moving.push_back(moving_element);
	}
	if ((index % 3) + 1 < 3 && curr_status[index + 1] != char(49)) {
		string next_status = curr_status;
		next_status[index] = next_status[index + 1];
		next_status[index + 1] = char(50);
		pair<string, string> moving_element;
		moving_element.second = qS.mov;
		moving_element.second.append("R");
		moving_element.first = next_status;
		moving.push_back(moving_element);
	}
	return moving;
}

int f2(string curr_status) {
	
	int sum = 0;
	for (int i = 0; i < curr_status.size(); ++i) {
		if (curr_status[i] == final_status[i] || curr_status[i] == char(50)) continue;
		int index = final_status.find(curr_status[i]);
		sum += abs(index % 3 - i % 3) + abs((index / 3) % 3 - (i / 3) % 3) 
			+ abs((index / 9) % 3 - (i / 9) % 3);
	}
	return sum;
}

string AStar(int(*f)(string)) {

	vector<pair<string, string> > moving;
	set<string> close;
	queueStruct best_status;
	while (!statusAStar.empty()) {
		best_status = statusAStar.top();
		statusAStar.pop();
		close.insert(best_status.s);
		moving = nextMove(best_status);
		for (int i = 0; i < moving.size(); ++i) {
			if (moving[i].first == final_status) {
				return moving[i].second;
			}
			if (close.find(moving[i].first) != close.end()) continue;
			queueStruct next_status;
			next_status.s = moving[i].first;
			next_status.first = f(moving[i].first);
			next_status.second = best_status.second + 1;
			next_status.mov = moving[i].second;
			statusAStar.push(next_status);
		}
	}
	return "";
}

int main() {

	clock_t start, finish;
	double totaltime;
	
	if (!input(f2)) return -1;
	start = clock();
	string s = AStar(f2);
	if(s == ""){
		cout << "cannot find a solution!" << endl;
		return 0;
	}
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	fstream fout;
	fout.open("output_Ah2.txt", ios::out);
	fout.setf(ios::fixed);
    fout << setprecision(3) << totaltime << endl << s << endl;
	fout.close();
}
