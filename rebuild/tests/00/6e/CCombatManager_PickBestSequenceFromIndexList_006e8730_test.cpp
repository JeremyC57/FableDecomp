#include <cstdio>

// Model of CCombatManager::PickBestSequenceFromIndexList @ 0x006E8730
static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range) { seed = seed * 0x24A1 + 0x24DF; seed = GFROR13(seed); if (range == 0) return 0; return seed % range; }
struct CLongVector { long* m_pFirst; long* m_pLast; long* m_pEnd; long size() const { return m_pLast - m_pFirst; } long operator[](long i) const { return m_pFirst[i]; } };
class CWorld { public: unsigned long seed; unsigned long* GetPRandomSeed() { return &seed; } };
class CCombatManager { public: unsigned char _pad[0x20]; CWorld* World;
    long PickBestSequenceFromIndexList(const CLongVector& indices) { long count = indices.size(); unsigned long* seed = World->GetPRandomSeed(); long pick = WorldRandomRange(*seed, count * 100); return indices[pick / 100]; } };
int main()
{
    long values[4] = { 100, 200, 300, 400 }; CLongVector v; v.m_pFirst = values; v.m_pLast = values + 4; v.m_pEnd = values + 4;
    CWorld w; w.seed = 12345; CCombatManager m; m.World = &w;
    unsigned long expectSeed = GFROR13(12345ul * 0x24A1 + 0x24DF); long expect = values[(long)(expectSeed % 400) / 100];
    long got = m.PickBestSequenceFromIndexList(v);
    if (got != expect || w.seed != expectSeed) { std::printf("006e8730_TEST FAIL\n"); return 1; }
    v.m_pLast = values; if (m.PickBestSequenceFromIndexList(v) != values[0]) { std::printf("006e8730_TEST FAIL\n"); return 1; }   // empty list: range 0 -> index 0
    std::printf("006e8730_TEST PASS\n"); return 0;
}