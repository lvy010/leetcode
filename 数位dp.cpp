/*
//https://oi-wiki.org/dp/number/

#include <cstdio>
using namespace std;
constexpr int N = 15;
using ll = long long;
ll l, r, dp[N], mi[N];
ll ans1[N], ans2[N];
int a[N];

void solve(ll n, ll *ans) {
  ll tmp = n;
  int len = 0;
  while (n) a[++len] = n % 10, n /= 10;
  for (int i = len; i >= 1; --i) {
    for (int j = 0; j < 10; j++) ans[j] += dp[i - 1] * a[i];
    for (int j = 0; j < a[i]; j++) ans[j] += mi[i - 1];
    tmp -= mi[i - 1] * a[i], ans[a[i]] += tmp + 1;
    ans[0] -= mi[i - 1];
  }
}

int main() {
  scanf("%lld%lld", &l, &r);
  mi[0] = 1ll;
  for (int i = 1; i <= 13; ++i) {
    dp[i] = dp[i - 1] * 10 + mi[i - 1];
    mi[i] = 10ll * mi[i - 1];
  }
  solve(r, ans1), solve(l - 1, ans2);
  for (int i = 0; i < 10; ++i) printf("%lld ", ans1[i] - ans2[i]);
  return 0;
}

*/


class Solution {
  typedef long long ll;
public:
  long long countNoZeroPairs(long long n) 
  {
      if(n<=0)
          return 0;

      ll cnt[10][2][2];
      ll m[2][2];
      bool init=false;

      if(!init)
      {
          memset(cnt,0,sizeof(cnt));
          
          for(int d=0;d<=9;d++)
          {
              for(int i=0;i<=1;i++)
                  {
                  for(int a=1;a<=9;a++)
                      {
                          for(int b=1;b<=9;b++)
                              {
                                  int s=a+b+i;
                                  if(s%10==d)
                                  {
                                      if(s>=10)
                                          ++cnt[d][i][1];
                                      else
                                          ++cnt[d][i][0];
                                  }
                              }
                      }
                  }
          }
          m[0][0]=m[0][1]=m[1][0]=m[1][1]=0;
          for(int d=0;d<=9;d++)
              {
                  m[0][0]+=cnt[d][0][0];
                  m[0][1]+=cnt[d][0][1];
                  m[1][0]+=cnt[d][1][0];
                  m[1][1]+=cnt[d][1][1];
              }
          init=true;
      }

      auto t_cnt=[&](int c,int l)->ll
      {
          if(l==0)
              return (c==0);
          
          ll dp0=(c==0);
          ll dp1=(c==1);

          for(int i=0;i<l;i++)
              {
                  ll ndp0=dp0*m[0][0]+dp1*m[1][0];
                  ll ndp1=dp0*m[0][1]+dp1*m[1][1];
                  dp0=ndp0;
                  dp1=ndp1;
              }
          return dp0;
      };

      int a[20],len=0;
      ll t=n;
      while(t)
          {
              a[++len]=(int)(t%10);
              t/=10;
          }
      ll ret=0;
      ll eq0=1,eq1=0;
      for(int i=len;i>=1;i--)
          {
              int cur=a[i];
              if(eq0)
              {
                  int bg=(i==len)?1:0;
                  for(int d=bg;d<cur;d++)
                      {
                          ll w0=cnt[d][0][0];
                          ll w1=cnt[d][0][1];
                          if(w0)
                              ret+=eq0*w0*t_cnt(0,i-1);
                          if(w1)
                              ret+=eq0*w1*t_cnt(1,i-1);
                      }
              }
              if(eq1)
              {
                  for(int d=1;d<cur;d++)
                      {
                          ll w0=cnt[d][1][0];
                          ll w1=cnt[d][1][1];
                          if(w0)
                              ret+=eq1*w0*t_cnt(0,i-1);
                          if(w1)
                              ret+=eq1*w1*t_cnt(1,i-1);
                      }
              }
              ll ne0=0,ne1=0;
              if(eq0)
              {
                  ne0+=eq0*cnt[cur][0][0];
                  ne1+=eq0*cnt[cur][0][1];
              }
               if(eq1)
              {
                  ne0+=eq1*cnt[cur][1][0];
                  ne1+=eq1*cnt[cur][1][1];
              }
              eq0=ne0;
              eq1=ne1;
          }
      ret+=eq0;
      return ret;
  }
};