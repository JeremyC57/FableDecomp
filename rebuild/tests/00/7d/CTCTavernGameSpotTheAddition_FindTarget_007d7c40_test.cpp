#include <cstdio>
// Behaviour test for CTCTavernGameSpotTheAddition::FindTarget (0x007D7C40).
class CThing { public: int id; };
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct STargetEntry { unsigned char _pad[0xc]; CThing* m_pThing; };
struct CTCTavernGameSpotTheAddition {
    unsigned char _pad[0x110];
    CRetailVector<STargetEntry> m_targets;
    unsigned char _pad_0x11c[0x28];
    long m_currentTarget;
    CThing* FindTarget(long unused);
};
CThing* CTCTavernGameSpotTheAddition::FindTarget(long)
{
    if (m_currentTarget < m_targets.size() && m_currentTarget >= 0)
        return m_targets[m_currentTarget].m_pThing;
    return 0;
}
int main()
{
    static CTCTavernGameSpotTheAddition g;
    CThing a, b; STargetEntry e[2]; e[0].m_pThing = &a; e[1].m_pThing = &b;
    g.m_targets.first = e; g.m_targets.last = e + 2; g.m_targets.end_of_storage = e + 2;
    g.m_currentTarget = 1;
    if (g.FindTarget(0) != &b) { std::printf("007d7c40_TEST FAIL\n"); return 1; }
    g.m_currentTarget = 0;
    if (g.FindTarget(5) != &a) { std::printf("007d7c40_TEST FAIL\n"); return 1; }
    g.m_currentTarget = 2;
    if (g.FindTarget(0) != 0) { std::printf("007d7c40_TEST FAIL\n"); return 1; }
    g.m_currentTarget = -1;
    if (g.FindTarget(0) != 0) { std::printf("007d7c40_TEST FAIL\n"); return 1; }
    std::printf("007d7c40_TEST PASS\n");
    return 0;
}