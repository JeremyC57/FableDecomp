// behaviour test for 0x00777B30 _Rb_tree::erase(const key_type&) (model: the key's equal range is erased and counted)
#include <cstdio>
#include <set>
int main()
{
    std::multiset<int> s; s.insert(1); s.insert(2); s.insert(2);
    unsigned int n = (unsigned int)s.erase(2);
    int ok = n == 2 && s.size() == 1;
    if (!ok) { std::printf("00777b30_TEST FAIL\n"); return 1; }
    std::printf("00777b30_TEST PASS\n");
    return 0;
}
