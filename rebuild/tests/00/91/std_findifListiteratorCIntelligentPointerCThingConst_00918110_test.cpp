// behaviour test for 0x00918110 std::find_if over list<CIntelligentPointer<CThing> > with CIsFreeBench (model over std::list<int>)
#include <cstdio>
#include <list>
template <class It, class Pred> It FindIf(It first, It last, Pred pred) { while (first != last && !pred(*first)) ++first; return first; }
struct IsSeven { bool operator()(int v) const { return v == 7; } };
int main()
{
    std::list<int> l; l.push_back(1); l.push_back(7); l.push_back(3); l.push_back(7);
    std::list<int>::const_iterator it = FindIf(l.begin(), l.end(), IsSeven());
    int ok = it != l.end() && *it == 7 && it == ++l.begin();
    std::list<int> e;
    ok = ok && FindIf(e.begin(), e.end(), IsSeven()) == e.end();
    if (!ok) { std::printf("00918110_TEST FAIL\n"); return 1; }
    std::printf("00918110_TEST PASS\n");
    return 0;
}