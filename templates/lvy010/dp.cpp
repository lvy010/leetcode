class Solution {
public:
    int minMaxWaitingTime(vector<int>& demand, vector<int>& fuel) {
        unordered_map<int, pair<int, int>> memo;

        // 加油机 0 在 wait0 秒后空闲，剩余燃料量 fuel0
        // 加油机 1 在 wait1 秒后空闲，剩余燃料量 fuel1
        auto dfs = [&](this auto&& dfs, int i, int wait0, int wait1, int fuel0, int fuel1) -> pair<int, int> {
            if (i == demand.size()) {
                return {};
            }

            int key = i << 24 | wait0 << 18 | wait1 << 12 | fuel0 << 6 | fuel1;
            if (memo.contains(key)) {
                return memo[key];
            }

            int max_num = 0;
            int best_wait_time = 0;
            int d = demand[i];

            // 选择加油机 0，等 wait0 秒开始加油，加油机 1 的等待时间减少 wait0 秒
            if (d <= fuel0) {
                auto [num, time] = dfs(i + 1, d, max(wait1 - wait0, 0), fuel0 - d, fuel1);
                max_num = num + 1;
                best_wait_time = max(time, wait0);
            }

            // 选择加油机 1，等 wait1 秒开始加油，加油机 0 的等待时间减少 wait1 秒
            if (d <= fuel1) {
                auto [num, time] = dfs(i + 1, max(wait0 - wait1, 0), d, fuel0, fuel1 - d);
                num++;
                time = max(time, wait1);
                if (num > max_num || num == max_num && time < best_wait_time) {
                    max_num = num;
                    best_wait_time = time;
                }
            }

            return memo[key] = {max_num, best_wait_time};
        };

        auto [max_num, best_wait_time] = dfs(0, 0, 0, fuel[0], fuel[1]);
        if (max_num == 0) {
            return -1;
        }
        return best_wait_time;
    }
};
