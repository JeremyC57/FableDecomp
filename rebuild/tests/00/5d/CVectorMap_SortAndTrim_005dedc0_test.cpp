#include <cstdio>
// Behaviour of CVectorMap::SortAndTrim (0x005DEDC0): sorts by key only when Dirty, always trims capacity.
struct CAnimationEntry { long m_words[4]; };
struct SPair { unsigned long first; CAnimationEntry second; };
struct CKeyPairCompareLess { bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; } };
static long g_sorts = 0;
template <class T, class C> void sort(T* first, T* last, C comp)
{
    ++g_sorts;
    for (T* i = first; i != last; ++i)
        for (T* j = i; j != first && comp(*j, *(j - 1)); --j) { T t = *j; *j = *(j - 1); *(j - 1) = t; }
}
struct CArray {
    SPair* _First; SPair* _Last; SPair* _End;
    SPair* begin() { return _First; }
    SPair* end() { return _Last; }
    void DeallocateUnusedMemory() { _End = _Last; }
};
struct CVectorMap {
    CArray Data; CKeyPairCompareLess Compare; bool Dirty;
    void SortAndTrim()
    {
        if (Dirty) {
            sort(Data.begin(), Data.end(), Compare);
            Dirty = false;
        }
        Data.DeallocateUnusedMemory();
    }
};
int main()
{
    SPair items[5];
    unsigned long keys[3] = { 30, 10, 20 };
    for (int i = 0; i < 3; ++i) { items[i].first = keys[i]; items[i].second.m_words[0] = (long)keys[i] * 2; }
    CVectorMap m; m.Data._First = items; m.Data._Last = items + 3; m.Data._End = items + 5; m.Dirty = true;
    m.SortAndTrim();
    if (g_sorts != 1 || m.Dirty || items[0].first != 10 || items[1].first != 20 || items[2].first != 30 || items[0].second.m_words[0] != 20 || m.Data._End != items + 3) {
        std::printf("005dedc0_TEST FAIL\n"); return 1;
    }
    items[0].first = 99; m.Data._End = items + 5;
    m.SortAndTrim();                               // clean: no sort, still trimmed
    if (g_sorts != 1 || items[0].first != 99 || m.Data._End != items + 3) { std::printf("005dedc0_TEST FAIL\n"); return 1; }
    std::printf("005dedc0_TEST PASS\n");
    return 0;
}