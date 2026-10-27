// behaviour test for 0x005BB5EF _Rb_tree::insert_unique(hint, v) (model: hinted inserts keep the map sorted and unique)
#include <cstdio>
#include <map>
int main()
{
    std::map<int, int> m;
    m.insert(m.end(), std::make_pair(5, 1));
    m.insert(m.begin(), std::make_pair(1, 2));
    m.insert(m.find(5), std::make_pair(3, 3));
    m.insert(m.end(), std::make_pair(3, 9));
    int ok = m.size() == 3 && m[3] == 3 && m.begin()->first == 1;
    if (!ok) { std::printf("005bb5ef_TEST FAIL\n"); return 1; }
    std::printf("005bb5ef_TEST PASS\n");
    return 0;
}