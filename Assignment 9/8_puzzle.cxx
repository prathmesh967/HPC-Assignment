#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

vector<string> getNeighbors(string state) {
    int idx = state.find('0');
    int x = idx / 3;
    int y = idx % 3;
    vector<string> neighbors;
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
            int nidx = nx * 3 + ny;
            string new_state = state;
            swap(new_state[idx], new_state[nidx]);
            neighbors.push_back(new_state);
        }
    }
    return neighbors;
}

void sequential_bfs(string start, string goal, double& time_taken, int& nodes_explored) {
    auto start_time = omp_get_wtime();
    queue<string> q;
    set<string> visited;
    q.push(start);
    visited.insert(start);
    nodes_explored = 0;

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        nodes_explored++;

        if (current == goal) break;

        vector<string> neighbors = getNeighbors(current);
        for (string& neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    time_taken = omp_get_wtime() - start_time;
}

void sequential_dfs(string start, string goal, double& time_taken, int& nodes_explored) {
    auto start_time = omp_get_wtime();
    stack<string> s;
    set<string> visited;
    s.push(start);
    visited.insert(start);
    nodes_explored = 0;

    while (!s.empty()) {
        string current = s.top();
        s.pop();
        nodes_explored++;

        if (current == goal) break;

        vector<string> neighbors = getNeighbors(current);
        for (int i = neighbors.size() - 1; i >= 0; i--) {
            if (visited.find(neighbors[i]) == visited.end()) {
                visited.insert(neighbors[i]);
                s.push(neighbors[i]);
            }
        }
    }

    time_taken = omp_get_wtime() - start_time;
}

int main() {
    string start = "123456078";
    string goal = "123456780";

    double seq_bfs_time, seq_dfs_time;
    int seq_bfs_nodes, seq_dfs_nodes;

    sequential_bfs(start, goal, seq_bfs_time, seq_bfs_nodes);
    sequential_dfs(start, goal, seq_dfs_time, seq_dfs_nodes);

    cout << "Sequential BFS: Time = " << seq_bfs_time << "s, Nodes = " << seq_bfs_nodes << endl;
    cout << "Sequential DFS: Time = " << seq_dfs_time << "s, Nodes = " << seq_dfs_nodes << endl;

    atomic<bool> solution_found(false);
    atomic<int> winner(-1);

    atomic<int> bfs_visited_count(0);
    atomic<int> dfs_visited_count(0);
    atomic<int> bfs_wasted(0);
    atomic<int> dfs_wasted(0);

    omp_set_num_threads(2);

    auto parallel_start = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            queue<string> q;
            set<string> visited;
            q.push(start);
            visited.insert(start);
            bfs_visited_count++;

            while (!q.empty() && !solution_found.load()) {
                string current = q.front();
                q.pop();

                vector<string> neighbors = getNeighbors(current);
                bool found = false;
                for (string& neighbor : neighbors) {
                    if (neighbor == goal) {
                        if (solution_found.exchange(true) == false) {
                            winner = 0;
                        }
                        found = true;
                        break;
                    }
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        q.push(neighbor);
                        bfs_visited_count++;
                    }
                }
                if (found) break;
            }

            if (solution_found.load() && winner != 0) {
                bfs_wasted = bfs_visited_count.load();
            }
        }

        #pragma omp section
        {
            stack<string> s;
            set<string> visited;
            s.push(start);
            visited.insert(start);
            dfs_visited_count++;

            while (!s.empty() && !solution_found.load()) {
                string current = s.top();
                s.pop();

                vector<string> neighbors = getNeighbors(current);
                bool found = false;
                for (int i = neighbors.size() - 1; i >= 0; i--) {
                    if (neighbors[i] == goal) {
                        if (solution_found.exchange(true) == false) {
                            winner = 1;
                        }
                        found = true;
                        break;
                    }
                    if (visited.find(neighbors[i]) == visited.end()) {
                        visited.insert(neighbors[i]);
                        s.push(neighbors[i]);
                        dfs_visited_count++;
                    }
                }
                if (found) break;
            }

            if (solution_found.load() && winner != 1) {
                dfs_wasted = dfs_visited_count.load();
            }
        }
    }

    double parallel_time = omp_get_wtime() - parallel_start;

    int total_visited = bfs_visited_count.load() + dfs_visited_count.load();
    int wasted_work = bfs_wasted.load() + dfs_wasted.load();

    if (winner == 0) {
        cout << "BFS found the solution" << endl;
    } else if (winner == 1) {
        cout << "DFS found the solution" << endl;
    } else {
        cout << "No solution found" << endl;
    }

    cout << "Parallel speculative execution time: " << parallel_time << "s" << endl;
    cout << "Total nodes explored (parallel): " << total_visited << endl;
    cout << "Wasted computation (discarded work): " << wasted_work << " nodes" << endl;
    cout << "Wasted percentage: " << (wasted_work * 100.0 / total_visited) << "%" << endl;

    double best_sequential = min(seq_bfs_time, seq_dfs_time);
    cout << "Speedup over sequential: " << best_sequential / parallel_time << "x" << endl;

    return 0;
}