#include <cstdio>
struct SMorphEntry { long m_words[20 / 4]; };
struct SMorphPair { long first; SMorphEntry second; SMorphPair(const long& k, const SMorphEntry& v) : first(k), second(v) { } };
struct CMorphPairVector {
    SMorphPair* m_pFirst; SMorphPair* m_pLast; SMorphPair* m_pEnd;
    void PushBack(const SMorphPair& p) { *m_pLast++ = p; }
};
struct CVectorMap {
    CMorphPairVector m_vector; unsigned char Compare; bool Dirty;
    void Add(const long& key, const SMorphEntry& value) { { SMorphPair pair(key, value); m_vector.PushBack(pair); } Dirty = true; }
};
int main()
{
    static unsigned char storage[8 * sizeof(SMorphPair)];
    CVectorMap m; m.m_vector.m_pFirst = m.m_vector.m_pLast = (SMorphPair*)storage; m.m_vector.m_pEnd = m.m_vector.m_pFirst + 8; m.Dirty = false;
    SMorphEntry e; for (int i = 0; i < 20 / 4; ++i) e.m_words[i] = 100 + i;
    long key = 7;
    m.Add(key, e);
    bool ok = m.Dirty && m.m_vector.m_pLast - m.m_vector.m_pFirst == 1 && m.m_vector.m_pFirst->first == 7;
    for (int j = 0; j < 20 / 4; ++j) if (m.m_vector.m_pFirst->second.m_words[j] != 100 + j) ok = false;
    if (!ok) { std::printf("0081d7d0_TEST FAIL\n"); return 1; }
    std::printf("0081d7d0_TEST PASS\n");
    return 0;
}
