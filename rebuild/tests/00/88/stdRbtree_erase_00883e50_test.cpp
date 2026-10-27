// behaviour test for 0x00883E50: erasing [first, last) removes exactly that range; the whole range empties the tree
#include <cstdio>
#include <set>
int main()
{
    std::set<int> s; for (int i = 0; i < 10; ++i) s.insert(i);
    std::set<int>::iterator a = s.find(3), b = s.find(7);
    s.erase(a, b);
    bool part = s.size() == 6 && s.count(3) == 0 && s.count(6) == 0 && s.count(7) == 1 && s.count(2) == 1;
    s.erase(s.begin(), s.end());
    if (!part || !s.empty()) { std::printf("00883e50_TEST FAIL\n"); return 1; }
    std::printf("00883e50_TEST PASS\n");
    return 0;
}