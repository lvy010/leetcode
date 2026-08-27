class Solution {
public:
    long long maximumValueSum(vector<vector<int>>& board) {
        array<pair<int, int>, 3> p; // 最大、次大、第三大
        ranges::fill(p, pair(INT_MIN, -1));

        auto update = [&](vector<int>& row) {
            for (int j = 0; j < row.size(); j++) {
                int x = row[j];
                if (x > p[0].first) {
                    if (p[0].second != j) { // 如果相等，仅更新最大
                        if (p[1].second != j) { // 如果相等，仅更新最大和次大
                            p[2] = p[1];
                        }
                        p[1] = p[0];
                    }
                    p[0] = {x, j};
                } else if (x > p[1].first && j != p[0].second) {
                    if (p[1].second != j) { // 如果相等，仅更新次大
                        p[2] = p[1];
                    }
                    p[1] = {x, j};
                } else if (x > p[2].first && j != p[0].second && j != p[1].second) {
                    p[2] = {x, j};
                }
            }
        };

        int m = board.size(), n = board[0].size();
        vector<array<pair<int, int>, 3>> suf(m);
        for (int i = m - 1; i > 1; i--) {
            update(board[i]);
            suf[i] = p;
        }

        long long ans = LLONG_MIN;
        ranges::fill(p, pair(INT_MIN, -1)); // 重置，计算 pre
        for (int i = 1; i < m - 1; i++) {
            update(board[i - 1]);
            for (int j2 = 0; j2 < n; j2++) { // 第二个车
                for (auto& [x, j1] : p) { // 第一个车
                    for (auto& [z, j3] : suf[i + 1]) { // 第三个车
                        if (j1 != j2 && j1 != j3 && j2 != j3) { // 没有同列的车
                            ans = max(ans, (long long) x + board[i][j2] + z);
                            break;
                        }
                    }
                }
            }
        }
        return ans;
    }
};
