#include <cstdio>
#include <map>
// Behaviour model: equal_range brackets exactly the entries with the key (empty and positioned when absent).
int main()
{
    std::multimap<int, int> m;
    m.insert(std::make_pair(1, 10)); m.insert(std::make_pair(2, 20)); m.insert(std::make_pair(2, 21)); m.insert(std::make_pair(4, 40));
    std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> r = m.equal_range(2);
    int n = 0; for (std::multimap<int, int>::iterator i = r.first; i != r.second; ++i) ++n;
    if (n != 2 || r.second->first != 4) { std::printf("007adbc0_TEST FAIL\n"); return 1; }
    r = m.equal_range(3);
    if (r.first != r.second || r.first->first != 4) { std::printf("007adbc0_TEST FAIL\n"); return 1; }
    std::printf("007adbc0_TEST PASS\n");
    return 0;
}