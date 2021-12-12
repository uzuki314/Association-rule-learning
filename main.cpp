#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "OnetoOne_onto.hpp"
using namespace std;

const unordered_set<int> split(const string &str, const char &delimiter,
                               OnetoOne_onto<int, string> &i_table) {
  unordered_set<int> result;
  stringstream ss(str);
  string tok;
  while (ss.good()) {
    getline(ss, tok, delimiter);
    if (i_table.hasR(tok)) {
      result.insert(i_table.getL(tok));
    } else {
      int new_key = i_table.size();
      i_table.insert(new_key, tok);
      result.insert(new_key);
    }
  }
  return result;
}
const vector<unordered_set<int>> read_database(
    OnetoOne_onto<int, string> &i_table) {
  vector<unordered_set<int>> DB;
  ifstream ifs;
  int number, tmp_max;
  string str;
  // get file name
  cout << "file name : ";
  string filename;
  cin >> filename;
  // try to open file
  ifs.open(filename);
  if (!ifs.is_open()) {
    cerr << "Failed to open " << filename << endl;
    goto END;
  }
  // read database

  while (ifs >> number >> str) {
    DB.push_back(split(str, ',', i_table));
  }
  // while (ifs >> str) {
  //   DB.push_back(split(str, ',', i_table));
  // }
END:
  return DB;
}

map<int, vector<int>> get_frequent_element(const vector<unordered_set<int>> &DB,
                                           const int max_item,
                                           const int min_supp_num) {
  double start_time = clock();

  map<int, vector<int>> res;
  cout << "\n";
  for (int i = 0, count; i <= max_item; i++) {
    cout << "\rgetting frequent element " << setw(6) << setfill(' ') << fixed
         << setprecision(2) << ((double)i / max_item) * 100 << "%";

    count = 0;
    vector<int> vtmp;
    for (int j = 0; j < DB.size(); j++) {
      if (DB[j].find(i) != DB[j].end()) {
        count++;
        vtmp.push_back(j);
      }
    }
    if (count >= min_supp_num) {
      // cout << i << endl;
      res[i] = vtmp;
    }
  }
  cout << "\t\t--done(" << ((double)(clock() - start_time) / CLOCKS_PER_SEC)
       << "sec)" << endl;
  return res;
}

vector<map<set<int>, vector<int>>> get_frequent_pattern(
    const vector<unordered_set<int>> &DB,
    const map<int, vector<int>> &frequent_element, const int min_supp_num) {
  double start_time = clock();
  cout<<"getting frequent pattern...";
  int deep = frequent_element.size();
  vector<map<set<int>, vector<int>>> match_patterns(deep);
  for (auto &ele : frequent_element) {
    match_patterns[0][set<int>{ele.first}] = ele.second;
  }
  for (int i = 1; i < deep; i++) {
    for (auto &p : match_patterns[i - 1]) {
      for (auto &ele : frequent_element) {
        if (p.first.find(ele.first) == p.first.end()) {
          int count = 0;
          vector<int> vtmp;
          for (auto &tid : p.second) {
            if (DB[tid].find(ele.first) != DB[tid].end()) {
              count++;
              vtmp.push_back(tid);
            }
          }
          if (count >= min_supp_num) {
            set<int> new_pattern = p.first;
            new_pattern.insert(ele.first);
            match_patterns[i][new_pattern] = vtmp;
          }
        }
      }
    }
  }
  cout << "\t\tdone("
       << ((double)(clock() - start_time) / CLOCKS_PER_SEC) << "sec)" << endl;
  return match_patterns;
}

// void show_frequent_pattern(vector<map<set<int>, vector<int>>>
// &match_patterns){
//   ofstream ofs;
//   ofs.open("frequent.txt");
//   for (auto &m_p : match_patterns) {
//     int check = m_p.size();
//     for (auto &p : m_p) {
//       ofs << "[";
//       for (auto &ele : p.first) {
//         if (ele != *p.first.begin()) ofs << ",";
//         ofs << ele;
//       }
//       ofs << "] ";
//     }
//     if (m_p.size() != 0) ofs << endl;
//   }
// }

void show_frequent_pattern(vector<map<set<int>, vector<int>>> &match_patterns,
                           OnetoOne_onto<int, string> &i_table) {
  for (auto &m_p : match_patterns) {
    int check = m_p.size();
    for (auto &p : m_p) {
      cout << "[";
      for (auto &ele : p.first) {
        if (ele != *p.first.begin()) cout << ",";
        cout << i_table.getR(ele);
      }
      cout << "] ";
    }
    if (m_p.size() != 0) cout << endl;
  }
}

vector<set<int>> combination(int n, int m) {
  vector<set<int>> res;
  vector<int> list;
  for (int i = 0; i < m; ++i) {
    list.push_back(i);
  }
  --list[m - 1];
  do {
    for (int i = m - 1; i >= 0; --i) {
      if (list[i] < n + i - m) {
        ++list[i];
        for (int j = i + 1; j < m; ++j) {
          list[j] = list[i] + j - i;
        }
        break;
      }
    }
    res.push_back(set<int>(list.begin(), list.end()));
  } while (list[0] < (n - m));
  return res;
}

int main() {
  OnetoOne_onto<int, string> item_table;
  vector<unordered_set<int>> DB = read_database(item_table);
  int max_item = item_table.size();

  double min_supp, min_conf;
  cout << "enter min_supp = ";
  cin >> min_supp;
  cout << "enter min_conf = ";
  cin >> min_conf;
  int min_supp_num = min_supp * DB.size();

  map<int, vector<int>> frequent_element =
      get_frequent_element(DB, max_item, min_supp_num);
  vector<map<set<int>, vector<int>>> match_patterns =
      get_frequent_pattern(DB, frequent_element, min_supp_num);
  show_frequent_pattern(match_patterns, item_table);

  set<pair<set<int>, set<int>>> Rules;
  int deep;
  for (deep = 0; match_patterns[deep].size(); deep++) {
  }

  vector<vector<vector<set<int>>>> combination_list(1);
  for (int i = 2; i <= deep; i++) {
    vector<vector<set<int>>> tmp;
    for (int j = 1; j <= i / 2; j++) {
      tmp.push_back(combination(i, j));
    }
    combination_list.push_back(tmp);
  }

  for (int i = deep - 1; i > 0; i--) {
    for (auto &p : match_patterns[i]) {
      vector<int> pattern(p.first.begin(), p.first.end());
      for (int j = 1; j <= pattern.size() / 2; j++) {
        for (auto &comb : combination_list[i][j - 1]) {
          set<int> a, b;
          for (int k = 0; k <= i; k++) {
            if (comb.find(k) != comb.end())
              a.insert(pattern[k]);
            else
              b.insert(pattern[k]);
          }
          if ((double)p.second.size() /
                  match_patterns[a.size() - 1][a].size() >=
              min_conf) {
            Rules.insert({a, b});
          }
          if ((double)p.second.size() /
                  match_patterns[b.size() - 1][b].size() >=
              min_conf) {
            Rules.insert({b, a});
          }
        }
      }
    }
  }
  for (auto &rule : Rules) {
    cout << "{";
    for (auto &ele : rule.first) {
      if (ele != *rule.first.begin()) cout << ',';
      cout << item_table.getR(ele);
    }
    cout << "} → {";
    for (auto &ele : rule.second) {
      if (ele != *rule.second.begin()) cout << ',';
      cout << item_table.getR(ele);
    }
    cout << "}\n";
  }

  return 0;
}