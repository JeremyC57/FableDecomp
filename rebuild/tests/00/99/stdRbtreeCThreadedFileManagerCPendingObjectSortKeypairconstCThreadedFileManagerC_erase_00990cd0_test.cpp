// behaviour test for 0x00990CD0 _Rb_tree::erase(const key_type&) (model: count the equal range, erase it, return the count)
#include <cstdio>
#include <map>
int main()
{
    std::multimap<int, int> m;
    m.insert(std::make_pair(1, 1)); m.insert(std::make_pair(2, 2)); m.insert(std::make_pair(2, 3));
    std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> p = m.equal_range(2);
    unsigned int n = 0;
    for (std::multimap<int, int>::iterator it = p.first; it != p.second; ++it) ++n;
    m.erase(p.first, p.second);
    int ok = n == 2 && m.size() == 1;
    if (!ok) { std::printf("00990cd0_TEST FAIL\n"); return 1; }
    std::printf("00990cd0_TEST PASS\n");
    return 0;
}