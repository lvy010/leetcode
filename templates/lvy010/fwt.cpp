/*
3514
先正变换做三次方卷积再逆变换
*/
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    // XOR FWT
    void fwtXOR(vector<long long>& a, int rsh) {
        int n = a.size();
        for (int l = 2, k = 1; l <= n; l <<= 1, k <<= 1) {
            for (int i = 0; i < n; i += l) {
                for (int j = 0; j < k; j++) {
                    long long x = a[i + j];
                    long long y = a[i + j + k];
                    a[i + j] = (x + y) >> rsh;
                    a[i + j + k] = (x - y) >> rsh;
                }
            }
        }
    }

    vector<long long> fwtXOR3(vector<long long>& a) {
        fwtXOR(a, 0);
        for (long long& val : a) {
            val = val * val * val; 
        }
        fwtXOR(a, 1);
        return a;
    }

    int uniqueXorTriplets(vector<int>& nums) {
        if (nums.empty()) return 0;
        int max_val = *max_element(nums.begin(), nums.end());
        int bit_len = 0;
        while ((1LL << bit_len) <= max_val) bit_len++;
        int size = 1 << bit_len;

        // 计数数组换成long long
        vector<long long> cnt(size, 0);
        for (int x : nums) {
            cnt[x]++;
        }

        vector<long long> res = fwtXOR3(cnt);
        int ans = 0;
        for (long long c : res) {
            if (c > 0) ans++;
        }
        return ans;
    }
};
