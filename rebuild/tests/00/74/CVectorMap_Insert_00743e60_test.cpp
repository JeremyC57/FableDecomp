// behaviour test for 0x00743E60 CVectorMap::Insert [CVectorMap<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo, CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> >]: the pair is appended in place while capacity remains, through the grow path otherwise, and Dirty is set either way
#include <cstdio>
#include <cstdlib>
struct Pair { int first; int second; };
static int gGrow = 0;
struct VecMap {
    Pair* start; Pair* finish; Pair* eos; bool Compare; bool Dirty;
    void Overflow(Pair* pos, const Pair& x)                  // stand-in for _M_insert_overflow (fill_len 1, atend)
    {
        ++gGrow;
        unsigned int n = (unsigned int)(finish - start), cap = n ? 2 * n : 1;
        Pair* s = (Pair*)std::malloc(cap * sizeof(Pair));
        for (unsigned int i = 0; i < n; ++i) s[i] = start[i];
        s[pos - start] = x;
        std::free(start); start = s; finish = s + n + 1; eos = s + cap;
    }
    void Insert(const int& key, const int& value)            // the function under test
    {
        Pair p; p.first = key; p.second = value;
        if (finish != eos) { if (finish != 0) *finish = p; ++finish; }
        else Overflow(finish, p);
        Dirty = true;
    }
};
int main()
{
    int fail = 0;
    VecMap m; m.start = m.finish = m.eos = 0; m.Dirty = false;
    m.Insert(7, 70);
    if (gGrow != 1 || m.finish - m.start != 1 || m.start[0].first != 7 || m.start[0].second != 70 || !m.Dirty) fail |= 1;
    m.Dirty = false; m.Insert(3, 30);
    if (gGrow != 2 || m.finish - m.start != 2 || m.start[1].first != 3 || !m.Dirty) fail |= 2;
    m.Dirty = false; m.Insert(5, 50);                        // full again: grows to 4
    if (gGrow != 3 || m.finish - m.start != 3) fail |= 4;
    m.Dirty = false; m.Insert(9, 90);
    if (gGrow != 3 || m.finish - m.start != 4 || m.start[3].first != 9 || m.start[3].second != 90 || !m.Dirty) fail |= 8;
    std::free(m.start);
    if (fail) { std::printf("00743e60_TEST FAIL %d\n", fail); return 1; }
    std::printf("00743e60_TEST PASS\n");
    return 0;
}