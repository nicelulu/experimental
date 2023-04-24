//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//
//void perpos(const string & s, int left, int right, vector<vector<bool>> & dp)
//{
//    while(left >=0 && right < s.size() && s[left] == s[right])
//    {
//        dp[left][right] = true;
//        left--;
//        right++;
//    }
//    return;
//}
//
//void dfs(vector<vector<string>> & ans,vector<string> & path, const string & s, int ind, vector<vector<bool>>& dp)
//{
//    if(ind == s.size())
//    {
//        ans.push_back(path);
//        return;
//    }
//    for(int i = ind; i < s.size(); i++)
//    {
//        if(!dp[ind][i])
//            continue;
//        path.push_back(s.substr(ind,i+1-ind));
//        dfs(ans,path,s,i+1,dp);
//        path.pop_back();
//    }
//    return;
//}
//
//vector<vector<string>> test(const string & s)
//{
//    vector<vector<string>> ans;
//    int len = s.size();
//    vector<vector<bool>> dp(len,vector<bool>(len,false));
//    for(int i = 0; i < len; i++)
//    {
//        perpos(s,i,i,dp);
//        perpos(s,i,i+1,dp);
//    }
//    vector<string> path;
//    dfs(ans,path,s,0,dp);
//    return ans;
//}
//
//int main()
//{
//    string s("aab");
//    vector<vector<string>> res = test(s);
//    for (size_t i = 0; i < res.size(); ++i)
//    {
//        for (size_t j = 0; j < res[i].size(); ++j)
//        {
//            std::cout << res[i][j] << std::endl;
//        }
//    }
//}
