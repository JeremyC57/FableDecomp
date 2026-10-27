#include <cstdio>
#include <list>
#include <algorithm>
// Behaviour model: find_if returns the first accepted element, else end.
static bool odd(int v) { return v & 1; }
int main()
{
    std::list<int> l; l.push_back(2); l.push_back(4); l.push_back(5); l.push_back(7);
    std::list<int>::const_iterator it = std::find_if(l.begin(), l.end(), odd);
    if (it == l.end() || *it != 5) { std::printf("0092e790_TEST FAIL\n"); return 1; }
    std::list<int> e; e.push_back(2);
    if (std::find_if(e.begin(), e.end(), odd) != e.end()) { std::printf("0092e790_TEST FAIL\n"); return 1; }
    std::printf("0092e790_TEST PASS\n");
    return 0;
}