class Solution {
    static constexpr int MOD = 1'000'000'007;

    long long pow(long long x, int n) {
        long long res = 1;
        for (; n; n /= 2) {
            if (n % 2) {
                res = res * x % MOD;
            }
            x = x * x % MOD;
        }
        return res;
    }

public:
    int sumOfNumbers(int l, int r, int k) {
        int m = r - l + 1;
        return (l + r) * m * (pow(10, k) - 1 + MOD) % MOD * pow(18, MOD - 2) % MOD * pow(m, k - 1) % MOD;
    }
};
