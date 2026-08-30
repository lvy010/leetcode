class Solution {
public:
    int maxConsistentColumns(vector<vector<int>>& grid, int limit) {
        int n = grid[0].size();
        vector<int> f(n, 1);
        int ans = 0;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) { // 枚举上一个保留的列
                bool ok = true;
                for (auto& row : grid) {
                    if (abs(row[i] - row[j]) > limit) {
                        ok = false; // 列 i 和列 j 不是一致的
                        break;
                    }
                }
                if (ok) {
                    f[i] = max(f[i], f[j] + 1);
                }
            }
            ans = max(ans, f[i]);
        }

        return ans;
    }
};
