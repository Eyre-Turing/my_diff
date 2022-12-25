#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <stdio.h>
#include <string.h>
#include <math.h>
using namespace std;

struct xyline {
	int xl;
	int yl;
};

vector<struct xyline> LCS(const vector<string> &X, const vector<string> &Y)
{
	int Xlen = X.size();
	int Ylen = Y.size(); 
	vector<vector<int> > dp;
	vector<vector<int> > c;
	dp.resize(Xlen + 1);
	dp[0].resize(Ylen + 1);
	c.resize(Xlen + 1);
	c[0].resize(Ylen + 1);
	for (int i = 1; i <= Xlen; ++i) {
		dp[i].resize(Ylen + 1);
		c[i].resize(Ylen + 1);
		for (int j = 1; j <= Ylen; ++j) {
			if (X[i - 1] == Y[j - 1] && dp[i - 1][j - 1] + 1 > max(dp[i - 1][j], dp[i][j - 1])) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
				c[i][j] = 3;
			} else {
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
				if (dp[i - 1][j] > dp[i][j - 1])
					c[i][j] = 1;
				else
					c[i][j] = 2;
			}
		}
	}
	stack<struct xyline> tmp;
	struct xyline item;
	int posi = Xlen;
	int posj = Ylen;
	while (posi > 0 && posj > 0) {
		if (c[posi][posj] == 1) {
			--posi;
		} else if (c[posi][posj] == 2) {
			--posj;
		} else if (c[posi][posj] == 3) {
			item.xl = posi;
			item.yl = posj;
			tmp.push(item);
			--posi;
			--posj;
		}
	}
	vector<struct xyline> result;
	while (!tmp.empty()) {
		result.push_back(tmp.top());
		tmp.pop();
	}
	return result;
}

bool split_file_to_vector(const char *fileA, vector<string> &X)
{
	FILE *fpA = fopen(fileA, "rb");
	if (!fpA) return false;
	string tmp;
	char ch;
	X.resize(0);
	while (fread(&ch, 1, 1, fpA)) {
		tmp += ch;
		if (ch == '\n') {
			X.push_back(tmp);
			tmp = "";
		}
	}
	if (tmp != "") X.push_back(tmp);
	fclose(fpA);
	return true;
}

void output_one_line(const string &line)
{
	if (line[line.size() - 1] == '\n')
		cout << line;
	else
		cout << line << endl;
}

void show_diff(const vector<string> &X, const vector<string> &Y, const vector<struct xyline> &lcsr, char *fileA, char *fileB)
{
	int len = lcsr.size();
	int apos = 1;
	int bpos = 1;
	cout << "--- a/" << fileA << endl << "+++ b/" << fileB << endl;
	for (int i = 0; i < len; ++i) {
		if (apos != lcsr[i].xl) {
			cout << "@@ -" << apos << "," << lcsr[i].xl - 1 << " b:" << bpos << " @@" << endl;
			for (int j = apos - 1; j < lcsr[i].xl - 1; ++j) {
				cout << "- ";
				output_one_line(X[j]);
			}
		}
		if (bpos != lcsr[i].yl) {
			cout << "@@ a:" << apos << " +" << bpos << "," << lcsr[i].yl - 1 << " @@" << endl;
			for (int j = bpos - 1; j < lcsr[i].yl - 1; ++j) {
				cout << "+ ";
				output_one_line(Y[j]);
			}
		}
		apos = lcsr[i].xl + 1;
		bpos = lcsr[i].yl + 1;
	}
}

void show_comm(const vector<string> &X, const vector<string> &Y, const vector<struct xyline> &lcsr)
{
	int len = lcsr.size();
	for (int i = 0; i < len; ++i) {
		cout << lcsr[i].xl << "|" << lcsr[i].yl << "\t";
		output_one_line(X[lcsr[i].xl - 1]);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <file-a> <file-b> [diff/comm]\n", argv[0]);
		return 1;
	}
	char *fileA = argv[1];
	char *fileB = argv[2];
	bool get_comm = false;
	if (argc == 4 && strcmp(argv[3], "comm") == 0) get_comm = true;
	vector<string> X;
	vector<string> Y;
	if ( (!split_file_to_vector(fileA, X)) || (!split_file_to_vector(fileB, Y)) ) {
		fprintf(stderr, "read file failed\n");
		return 1;
	}
	vector<struct xyline> lcsr = LCS(X, Y);
	if (get_comm)
		show_comm(X, Y, lcsr);
	else
		show_diff(X, Y, lcsr, fileA, fileB);
	return 0;
}

