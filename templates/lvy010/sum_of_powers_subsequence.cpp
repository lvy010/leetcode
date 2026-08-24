/*
DP with hashmap
Sum of Powers for k‑length subsequences 

*/
using ll = long long;
constexpr int N = 51;
constexpr ll mod = int(1e9) + 7;

class Solution {
public:
    int sumOfPowers(vector<int>& nums, int k) {
        // dp[i][j][d] := The counter for the first `i` elements
        // with diff `d` when the length of subsequence is `j`.
        std::unordered_map<int, ll> dp[N][N];

        int n = nums.size();
        // Sort for minimum diff calculation.
        std::sort(nums.begin(), nums.end());

        for (int i = 0; i < n; ++ i) {
            // Initialization (here, `mod` is equal to inf)
            dp[i][1][mod] = 1;
            for (int j = 2; j <= k; ++ j) {
                for (int lst_i = 0; lst_i < i; ++ lst_i) {
                    for (auto& [diff, cnt]: dp[lst_i][j - 1]) {
                        auto nxt_diff = std::min(diff, nums[i] - nums[lst_i]);
                        dp[i][j][nxt_diff] += cnt;
                        dp[i][j][nxt_diff] %= mod;
                    }
                }
            }
        }

        ll ans = 0;
        for (int i = 0; i < n; ++ i) {
            for (auto& [diff, cnt]: dp[i][k]) {
                ans = (ans + diff * cnt) % mod;
            }
        }

        return ans;
    }
};
