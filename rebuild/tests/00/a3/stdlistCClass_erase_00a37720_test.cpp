#include <cstdio>
#include <list>
// Behaviour model: erase(first, last) removes exactly [first, last) and returns last.
int main()
{
    std::list<int> l;
    for (int i = 0; i < 6; ++i) l.push_back(i);
    std::list<int>::iterator f = l.begin(), e = l.begin();
    ++f; for (int i = 0; i < 4; ++i) ++e;
    std::list<int>::iterator r = l.erase(f, e);
    if (l.size() != 3 || *r != 4 || l.front() != 0 || l.back() != 5) { std::printf("00a37720_TEST FAIL\n"); return 1; }
    r = l.erase(l.begin(), l.begin());
    if (l.size() != 3 || r != l.begin()) { std::printf("00a37720_TEST FAIL\n"); return 1; }
    std::printf("00a37720_TEST PASS\n");
    return 0;
}