// Behaviour test for std::insert_iterator<set<unsigned long> >::operator=(const unsigned long&) @ 0x00AD4810
// Inserts the value at the hint (iter = container->insert(iter, value)), then advances the hint past it.
#include <cstdio>
#include <set>

typedef std::set<unsigned int> TIndexSet;

struct CSetInserter {
    TIndexSet* container;
    TIndexSet::iterator iter;
    CSetInserter(TIndexSet& c, TIndexSet::iterator i) : container(&c), iter(i) { }
    CSetInserter& operator=(const unsigned int& value)
    {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }
};

int main()
{
    TIndexSet s;
    s.insert(10);
    s.insert(30);
    CSetInserter ins(s, s.begin());
    CSetInserter& self = (ins = 5);
    int ok = &self == &ins && s.size() == 3 && *ins.iter == 10;     // hint now just after 5
    ins = 20;
    if (s.size() != 4 || *ins.iter != 30) ok = 0;
    ins = 30;                                                        // duplicate: kept once, hint moves past it
    if (s.size() != 4 || ins.iter != s.end()) ok = 0;
    ins = 40;
    if (s.size() != 5 || ins.iter != s.end()) ok = 0;
    static const unsigned int expected[5] = { 5, 10, 20, 30, 40 };
    int i = 0;
    for (TIndexSet::const_iterator it = s.begin(); it != s.end(); ++it, ++i)
        if (*it != expected[i]) ok = 0;
    if (!ok) { std::printf("00ad4810_TEST FAIL\n"); return 1; }
    std::printf("00ad4810_TEST PASS\n");
    return 0;
}