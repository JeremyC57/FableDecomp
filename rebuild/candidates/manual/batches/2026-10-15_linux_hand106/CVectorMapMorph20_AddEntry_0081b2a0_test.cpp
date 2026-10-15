#include <cstdio>
struct SMorphEntry20 { long m_words[5]; };
struct SMorphPair20 { long first; SMorphEntry20 second; SMorphPair20(const long& k, const SMorphEntry20& v) : first(k), second(v) { } };
struct CMorphPairVector20 {
    SMorphPair20* m_pFirst; SMorphPair20* m_pLast; SMorphPair20* m_pEnd;
    void PushBack(const SMorphPair20& p) { *m_pLast++ = p; }
};
struct CKeyPairCompareLess { };
static bool g_dirtyDuringSort = false;
static bool* g_pDirty = 0;
static void SortMorphPairs(SMorphPair20* first, SMorphPair20* last, CKeyPairCompareLess)
{
    g_dirtyDuringSort = *g_pDirty;
    for (SMorphPair20* i = first; i != last; ++i)
        for (SMorphPair20* j = i + 1; j != last; ++j)
            if (j->first < i->first) { SMorphPair20 t = *i; *i = *j; *j = t; }
}
struct CVectorMap_Morph20 {
    CMorphPairVector20 m_vector; CKeyPairCompareLess Compare; bool Dirty;
    void Add(const long& key, const SMorphEntry20& value) { { SMorphPair20 pair(key, value); m_vector.PushBack(pair); } Dirty = true; }
    void Sort() { SortMorphPairs(m_vector.m_pFirst, m_vector.m_pLast, Compare); Dirty = false; }
    void AddEntry(const SMorphEntry20& entry) { Add(entry.m_words[0], entry); Sort(); }
};
int main()
{
    static unsigned char storage[8 * sizeof(SMorphPair20)];
    CVectorMap_Morph20 m; m.m_vector.m_pFirst = m.m_vector.m_pLast = (SMorphPair20*)storage; m.m_vector.m_pEnd = m.m_vector.m_pFirst + 8;
    m.Dirty = false; g_pDirty = &m.Dirty;
    SMorphEntry20 a = { { 5, 1, 2, 3, 4 } }, b = { { 2, 9, 9, 9, 9 } };
    m.AddEntry(a); m.AddEntry(b);
    bool ok = !m.Dirty && g_dirtyDuringSort && m.m_vector.m_pLast - m.m_vector.m_pFirst == 2 &&
              m.m_vector.m_pFirst[0].first == 2 && m.m_vector.m_pFirst[1].first == 5 && m.m_vector.m_pFirst[1].second.m_words[4] == 4;
    if (!ok) { std::printf("0081b2a0_TEST FAIL\n"); return 1; }
    std::printf("0081b2a0_TEST PASS\n");
    return 0;
}
