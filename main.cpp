#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstdint>
#include <string>
#include <tbb/tbb.h>

struct Subproblem {
    std::vector<int8_t> board;
    int next_k;
};

uint64_t solve_seq(std::vector<int8_t>& board, int k, int n) {
    if (k == 0) return 1;
    uint64_t count = 0;
    const int len = 2 * n;
    const int max_i = (k == n) ? (n - 2) / 2 : (len - k - 2);

    for (int i = 0; i <= max_i; ++i) {
        const int j = i + k + 1;
        if (board[i] == 0 && board[j] == 0) {
            board[i] = board[j] = static_cast<int8_t>(k);
            count += solve_seq(board, k - 1, n);
            board[i] = board[j] = 0;
        }
    }
    return count;
}

// Разрешение рекурсии: генерация списка независимых корней поддеревьев
void expand_recursion(std::vector<int8_t>& board, int k, int n, int target_k, std::vector<Subproblem>& tasks) {
    if (k == target_k) {
        tasks.push_back({board, k});
        return;
    }

    const int len = 2 * n;
    const int max_i = (k == n) ? (n - 2) / 2 : (len - k - 2);

    for (int i = 0; i <= max_i; ++i) {
        const int j = i + k + 1;
        if (board[i] == 0 && board[j] == 0) {
            board[i] = board[j] = static_cast<int8_t>(k);
            expand_recursion(board, k - 1, n, target_k, tasks);
            board[i] = board[j] = 0;
        }
    }
}

uint64_t solve_parallel_reduce(const std::vector<Subproblem>& tasks, int n) {
    return tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, tasks.size()),
        uint64_t(0),
        [&](const tbb::blocked_range<size_t>& r, uint64_t init) -> uint64_t {
            for (size_t idx = r.begin(); idx < r.end(); ++idx) {
                // Локальная копия доски задачи для независимой работы потока
                auto local_board = tasks[idx].board;
                init += solve_seq(local_board, tasks[idx].next_k, n);
            }
            return init;
        },
        std::plus<uint64_t>()
    );
}

int main(int argc, char* argv[]) {
    int n = 12;
    if (argc > 1) n = std::stoi(argv[1]);

    std::vector<int8_t> initial_board(2 * n, 0);
    std::vector<Subproblem> tasks;
    const int target_k = n - 3;

    expand_recursion(initial_board, n, n, target_k, tasks);
    std::cout << "Erzeugte Subproblems: " << tasks.size() << "\n";

    auto t1 = std::chrono::high_resolution_clock::now();
    uint64_t total = solve_parallel_reduce(tasks, n);
    auto t2 = std::chrono::high_resolution_clock::now();
    double t_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Loesungen (parallel_reduce): " << total << "\n";
    std::cout << "Laufzeit: " << t_ms << " ms\n";
    return 0;
}
