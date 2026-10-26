// behaviour test for 0x0051B880 _Rb_tree::erase(const key_type&): every element with the key is removed, the count of removed
// elements is returned, other keys stay, and a missing key removes nothing
#include <cstdio>
#include <map>
#include <iterator>
static unsigned int EraseKey(std::multimap<int, int>& m, int key)        // the function under test (STLport tree semantics)
{
    std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> p = m.equal_range(key);
    unsigned int n = (unsigned int)std::distance(p.first, p.second);
    m.erase(p.first, p.second);
    return n;
}
int main()
{
    int fail = 0;
    std::multimap<int, int> m;
    m.insert(std::make_pair(1, 10)); m.insert(std::make_pair(2, 20)); m.insert(std::make_pair(2, 21)); m.insert(std::make_pair(3, 30));
    if (EraseKey(m, 2) != 2u || m.size() != 2u || m.count(2) != 0u) fail |= 1;
    if (EraseKey(m, 7) != 0u || m.size() != 2u) fail |= 2;
    if (EraseKey(m, 1) != 1u || m.size() != 1u || m.begin()->first != 3) fail |= 4;
    if (fail) { std::printf("0051b880_TEST FAIL %d\n", fail); return 1; }
    std::printf("0051b880_TEST PASS\n");
    return 0;
}
