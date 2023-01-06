#include "bits/stdc++.h"

#define ff first
#define ss second
#define pb push_back
#define ll long long
#define db puts("*****")
#define sz(x) int(x.size())
#define pii pair <int , int>
#define mid(x , y) ((x+y)>>1)
#define all(x)	x.begin(),x.end()
#define y1 your_name_engraved_herein

using namespace std;

const int N = 2e5+2;
const int K = 1e3+2;
const int MOD = 998244353;

template<class T> bool umin(T& a, T b) { if(a > b){ a = b; return 1; } return 0; }
template<class T> bool umax(T& a, T b) { if(a < b){ a = b; return 1; } return 0; }

int toInt(const string &s){
  int n = (int)s.length();
  if(n == 0) return 0;
  
  int ret = 0;
  for(int i=0; i<n; i++){
    ret *= 10;
    ret += s[i]-'0';
  }
  return ret;
}

void skipColumns(ifstream &inFile, int len, int endLine){
  string tempBuffer;
  while(!inFile.eof() && len > 0){
    if(len > 1 || endLine == 0){ 
      getline(inFile, tempBuffer, ',');
      len--;
    }
    else{
      getline(inFile, tempBuffer);
      len--;
      break;
    }
  }
}

int main(){
  ifstream inFile;
  ofstream outFile;
  
  outFile.open("outout.out");
  inFile.open("sourceLmao.csv");
  if(inFile.is_open())
    printf("%s\n", "File is open");
  
  int lineSkipCount = 18;
  int beginColSkipCount = 1, endColSkipCount = 3;
  int projectCount = 12, counter = 0;
  
  vector<string> projectName(projectCount);
  vector<vector<int>> teibl(projectCount, vector<int> (projectCount, 0));
  while(!inFile.eof() && counter < projectCount){
    string s;
    if(lineSkipCount > 0){
      getline(inFile, s);
      lineSkipCount--;
      continue;
    }
    
    skipColumns(inFile, beginColSkipCount, 0);
    
    getline(inFile, projectName[counter++], ',');
    cout << projectName[counter-1] << " ";
    for(int i=0; i<projectCount; i++){
      getline(inFile, s, ',');
      teibl[counter-1][i] = toInt(s);
      cout << s << " ";
    }
    
    skipColumns(inFile, endColSkipCount, 1);
    
    cout << endl;
  }
  
  for(int i=0; i<projectCount; i++){
    cout << projectName[i] << " ";
    for(int j=0; j<projectCount; j++)
      cout << teibl[i][j] << " ";
    cout << endl;
  }
  
  vector<vector<int>> tempTeibl(projectCount, vector<int> (projectCount, 0));
  for(int i=0; i<projectCount; i++)
    for(int j=0; j<projectCount; j++)
      tempTeibl[j][i] = teibl[i][j];
  teibl = tempTeibl;
  
  int n = projectCount;
  vector<vector<int>> dp(n+1, vector<int> ((1<<n), 40320));
  vector<vector<int>> btrack(n+1, vector<int> ((1<<n), -1));
  
  dp[0][0] = 0;
  for(int i=1; i<=n; i++){
    for(int mask=0; mask<(1<<n); mask++){
      for(int j=0; j<n; j++)
        if(!((mask>>j)&1) && teibl[i-1][j] > 0)
          if(umin(dp[i][mask | (1<<j)], dp[i-1][mask] + teibl[i-1][j]))
            btrack[i][mask | (1<<j)] = j;
    }
  }
  printf("Minimal Cost: %d\n", dp[n][(1<<n)-1]);
  
  vector<pair<int, pair<int, int>>> sol;
  int team = n, curMask = (1<<n)-1;
  while(team > 0){
    sol.push_back(make_pair(teibl[team-1][btrack[team][curMask]], make_pair(team, btrack[team][curMask])));
    curMask ^= (1<<btrack[team][curMask]);
    team--;
  }
  reverse(all(sol));
  for(auto i : sol){
    printf("cost:%d, team:%d, project#:%d -> ", i.ff, i.ss.ff, i.ss.ss);
    cout << projectName[i.ss.ss] << endl;
  }
  
  return 0;
}
