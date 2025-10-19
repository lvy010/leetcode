
#include <bits/stdc++.h>
using namespace std;

vector<long long> a, b, lazy;

struct SqrtDecomposition {
  const int block_size, n;
  vector<int> ls, rs;
  vector<bool> to_be_eval;
  vector<vector<pair<int,int>>> first;

  explicit SqrtDecomposition(const int n_)
      : block_size((int)max(1.0, floor(sqrt((double)n_)))),
        n((n_ + block_size - 1) / block_size) {
    ls.resize(n);
    rs.resize(n);
    to_be_eval.assign(n, false);
    first.resize(n);
    for (int i = 0; i < n; ++i) {
      ls[i] = block_size * i;
      rs[i] = (i + 1 == n ? n_ : block_size * (i + 1));
      first[i].reserve((rs[i] - ls[i]) * 2);
    }
  }

  void rebuild_block(int b) {
  vector<pair<int,int>> tmp;
  tmp.reserve(rs[b] - ls[b]);
  for (int i = ls[b]; i < rs[b]; ++i) tmp.emplace_back((int)a[i], i);
  sort(tmp.begin(), tmp.end(), [](const pair<int,int>& x, const pair<int,int>& y){
    if (x.first != y.first) return x.first < y.first;
    return x.second < y.second;
  });
  auto &mp = first[b];
  mp.clear();
  mp.reserve(tmp.size());
  int last_key = INT_MIN;
  for (auto &p : tmp) {
    if (p.first != last_key) {
      last_key = p.first;
      mp.emplace_back(p.first, p.second);
    }
  }
}

  void build_maps() {
    for (int b = 0; b < n; ++b) rebuild_block(b);
  }

  inline int get_real(int idx) const {
    int blk = idx / block_size;
    return (int)(a[idx] + lazy[blk]);
  }

  int find_leftmost_equal(int uptoIdx, int target) const {
  int br = uptoIdx / block_size;
  for (int b = 0; b < br; ++b) {
    int key = target - (int)lazy[b];
    const auto &vec = first[b];
    auto it = lower_bound(vec.begin(), vec.end(), key,
      [](const pair<int,int>& p, int val){ return p.first < val; });
    if (it != vec.end() && it->first == key) return it->second;
  }
  int key_lazy = (int)lazy[br];
  for (int i = ls[br]; i <= uptoIdx; ++i) {
    if ((int)(a[i] + key_lazy) == target) return i;
  }
  return -1;
}

  void range_add_and_fix(int l, int r, int val) {
    if (r <= l) return;
    const int b_l = l / block_size, b_r = (r - 1) / block_size;
    update(l, r, val);
    if (b_l == b_r) {
      rebuild_block(b_l);
    } else {
      if (l != ls[b_l]) rebuild_block(b_l);
      if (r != rs[b_r]) rebuild_block(b_r);
    }
  }

  template <typename T>
  void partial_update(const int idx, const T val) {
    a[idx] += val;
    b[idx / block_size] += val;
  }

  template <typename T>
  void total_update(const int idx, const T val) {
    lazy[idx] += val;
    to_be_eval[idx] = true;
  }

  template <typename T>
  void update(const int l, const int r, const T val) {
    if (r <= l) return;
    const int b_l = l / block_size, b_r = (r - 1) / block_size;
    if (b_l < b_r) {
      if (l == ls[b_l]) {
        total_update(b_l, val);
      } else {
        for (int i = l; i < rs[b_l]; ++i) {
          partial_update(i, val);
        }
      }
      for (int i = b_l + 1; i < b_r; ++i) {
        total_update(i, val);
      }
      if (r == rs[b_r]) {
        total_update(b_r, val);
      } else {
        for (int i = ls[b_r]; i < r; ++i) {
          partial_update(i, val);
        }
      }
    } else {
      for (int i = l; i < r; ++i) {
        partial_update(i, val);
      }
    }
  }

  template <typename T>
  void partial_query(const int idx, T* val) {
    const int block = idx / block_size;
    if (to_be_eval[block]) {
      for (int i = ls[block]; i < rs[block]; ++i) {
        partial_update(i, lazy[block]);
      }
      lazy[block] = 0;
      to_be_eval[block] = false;
      const_cast<SqrtDecomposition*>(this)->rebuild_block(block);
    }
    *val += a[idx];
  }

  template <typename T>
  void total_query(const int idx, T* val) {
    *val += b[idx] + lazy[idx] * (rs[idx] - ls[idx]);
  }

  template <typename T>
  T query(const int l, const int r, const T id) {
    const int b_l = l / block_size, b_r = (r - 1) / block_size;
    T res = id;
    if (b_l < b_r) {
      if (l == ls[b_l]) {
        total_query(b_l, &res);
      } else {
        for (int i = l; i < rs[b_l]; ++i) {
          partial_query(i, &res);
        }
      }
      for (int i = b_l + 1; i < b_r; ++i) {
        total_query(i, &res);
      }
      if (r == rs[b_r]) {
        total_query(b_r, &res);
      } else {
        for (int i = ls[b_r]; i < r; ++i) {
          partial_query(i, &res);
        }
      }
    } else {
      for (int i = l; i < r; ++i) {
        partial_query(i, &res);
      }
    }
    return res;
  }
};

class Solution {
public:
    int longestBalanced(vector<int>& nums) 
    {
        int n=nums.size();
        int N=n+1;
        SqrtDecomposition sd(N);
        a.assign(N,0);
        b.assign(sd.n,0);
        lazy.assign(sd.n,0);
        sd.build_maps();
        int MAXV=100000;
        vector<int> last(MAXV+1,-1);
        int ans=0;
        for(int r=0;r<n;r++){
            int x=nums[r];
            int pre=last[x];
            int d=(x&1)?+1:-1;
            sd.range_add_and_fix(pre+1,r+1,d);
            last[x]=r;
            int t=sd.get_real(r+1);
            int j=sd.find_leftmost_equal(r,t);
            if(j!=-1)
                ans=max(ans,r-j+1);
        }
        return ans;
        
    }
};