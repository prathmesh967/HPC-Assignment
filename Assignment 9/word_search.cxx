#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

struct WordLocation {
    bool found = false;
    int row = -1;
    int col = -1;
    int dir = -1;
};

vector<string> grid;
vector<string> original_words;
vector<string> search_words;
int rows, cols;

unordered_map<string, WordLocation> global_result;

string removeSpaces(string &s) {
    string result = s;
    result.erase(remove(result.begin(), result.end(), ' '), result.end());
    return result;
}

bool checkWord(string &word, int r, int c, int d) {
    int len = word.length();
    for (int i = 0; i < len; ++i) {
        int nr = r + i * dx[d];
        int nc = c + i * dy[d];
        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols || grid[nr][nc] != word[i])
            return false;
    }
    return true;
}

vector<string> dirNames = {
    "NW", "N", "NE",
    "W", "E",
    "SW", "S", "SE"};

void sequential_search() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            for (int d = 0; d < 8; ++d) {
                for (size_t i = 0; i < original_words.size(); ++i) {
                    string &orig = original_words[i];
                    string &clean = search_words[i];
                    if (global_result[orig].found)
                        continue;
                    if (checkWord(clean, r, c, d)) {
                        global_result[orig].found = true;
                        global_result[orig].row = r;
                        global_result[orig].col = c;
                        global_result[orig].dir = d;
                    }
                }
            }
        }
    }
}

int main() {
    grid = {
        "MACPIUPSMNRD",
        "EENNEEHOHMEI",
        "LESCALMRAALT",
        "LFGCHEIRRLAN",
        "EIHNNOLCIEHT",
        "NNKKAIRGALSA",
        "SDGGNGMLONIO",
        "KIUENAKULLFL",
        "RNRDEBTNHMNB",
        "AGGNYRODACWR",
        "HLLDENTISTOU",
        "SGEBUBBLESLC",
        "BARRACUDAUCE",
        "CORALREEFOKR"};

    rows = grid.size();
    cols = grid[0].size();

    original_words = {
        "FINDING", "NEMO", "ELLEN", "MARLIN", "CLOWNFISH",
        "CORAL REEF", "PELICAN", "BARRACUDA", "DORY", "SHARKS",
        "BRUCE", "ANCHOR", "CHUM", "DENTIST", "TANK GANG",
        "GILL", "BLOAT", "BUBBLES", "PEACH", "GURGLE", "DEB"};

    for (auto &w : original_words)
        search_words.push_back(removeSpaces(w));

    for (auto &w : original_words)
        global_result[w] = WordLocation{};

    double seq_start = omp_get_wtime();
    sequential_search();
    double seq_time = omp_get_wtime() - seq_start;

    cout << "=== Sequential Search ===" << endl;
    cout << "Time: " << seq_time << "s" << endl << endl;

    for (auto &w : original_words)
        global_result[w] = WordLocation{};

    double par_start = omp_get_wtime();

#pragma omp parallel
    {
#pragma omp for schedule(dynamic)
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                for (int d = 0; d < 8; ++d) {
                    for (size_t i = 0; i < original_words.size(); ++i) {
                        string &orig = original_words[i];
                        string &clean = search_words[i];
                        if (checkWord(clean, r, c, d)) {
#pragma omp critical
                            {
                                if (!global_result[orig].found) {
                                    global_result[orig].found = true;
                                    global_result[orig].row = r;
                                    global_result[orig].col = c;
                                    global_result[orig].dir = d;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    double par_time = omp_get_wtime() - par_start;

    cout << "=== Parallel Exploratory Search ===" << endl;
    cout << "Time: " << par_time << "s" << endl;
    cout << "Speedup over sequential: " << seq_time / par_time << "x" << endl << endl;

    cout << "=== Final Results ===" << endl;
    for (auto &w : original_words) {
        WordLocation &loc = global_result[w];
        if (loc.found) {
            cout << w << ": FOUND at (" << loc.row << ", " << loc.col
                 << ") direction " << dirNames[loc.dir] << " (" << loc.dir << ")\n";
        } else {
            cout << w << ": NOT FOUND\n";
        }
    }
    return 0;
}
