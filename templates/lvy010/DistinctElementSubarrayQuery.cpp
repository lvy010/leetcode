class Solution {
    static inline mt19937_64 rng = mt19937_64(chrono::steady_clock::now().time_since_epoch().count());

public:
    vector<bool> validSubarrays(vector<int>& nums, int k, vector<vector<int>>& queries) {
        int n = nums.size();
        vector<uint64_t> sum(n + 1);
        unordered_map<int, uint64_t> hash;
        for (int i = 0; i < n; i++) {
            int x = nums[i];
            // 把 nums[i] 映射成一个随机的 uint64_t
            if (!hash.contains(x)) {
                hash[x] = rng();
            }
            sum[i + 1] = sum[i] ^ hash[x];
        }

        auto calc_left = [&](int k) -> vector<int> {
            vector<int> lefts(n);
            unordered_map<int, int> cnt;
            int l = 0;
            for (int i = 0; i < n; i++) {
                cnt[nums[i]]++;
                while (cnt.size() >= k) {
                    auto it = cnt.find(nums[l]);
                    if (--it->second == 0) {
                        cnt.erase(it); // 保证 cnt.size() 是窗口内的不同元素个数
                    }
                    l++;
                }
                lefts[i] = l;
            }
            return lefts;
        };

        auto l1 = calc_left(k + 1);
        auto l2 = calc_left(k);

        vector<bool> ans(queries.size());
        for (int i = 0; i < queries.size(); i++) {
            auto& q = queries[i];
            int l = q[0], r = q[1];
            ans[i] = sum[r + 1] == sum[l] && l1[r] <= l && l < l2[r];
        }
        return ans;
    }
};
