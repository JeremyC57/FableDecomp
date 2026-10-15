// Behaviour test for C3DAnimationBlendState::ClearCombinationBlends @ 0x00A3C330: own copy first, then the whole range erased.
#include <cstdio>
struct CCombinationBlend { long v; };
static int g_step = 0, g_copyStep = 0, g_eraseStep = 0;
static CCombinationBlend* g_first = 0;
static CCombinationBlend* g_last = 0;
struct CRetailBlendVector {
    CCombinationBlend* m_pFirst;
    CCombinationBlend* m_pLast;
    CCombinationBlend* m_pEnd;
    CCombinationBlend* erase(CCombinationBlend* first, CCombinationBlend* last)
    { g_eraseStep = ++g_step; g_first = first; g_last = last; m_pLast = first; return first; }
    void clear() { erase(m_pFirst, m_pLast); }
};
struct CSharedData { long m_refCount; void* m_pBaseAnimation; CRetailBlendVector CombinationBlends; };
struct C3DAnimationBlendState {
    CSharedData* Data;
    CSharedData* m_pOwnCopy;
    void MakeOwnCopyOfData() { g_copyStep = ++g_step; Data = m_pOwnCopy; }
    void ClearCombinationBlends() { MakeOwnCopyOfData(); Data->CombinationBlends.clear(); }
};
int main()
{
    CCombinationBlend blends[2];
    CSharedData shared, own;
    shared.CombinationBlends.m_pFirst = 0; shared.CombinationBlends.m_pLast = 0;
    own.CombinationBlends.m_pFirst = blends; own.CombinationBlends.m_pLast = blends + 2; own.CombinationBlends.m_pEnd = blends + 2;
    C3DAnimationBlendState s; s.Data = &shared; s.m_pOwnCopy = &own;
    s.ClearCombinationBlends();
    if (g_copyStep != 1 || g_eraseStep != 2 || g_first != blends || g_last != blends + 2 || own.CombinationBlends.m_pLast != blends)
    { std::printf("00a3c330_TEST FAIL\n"); return 1; }
    std::printf("00a3c330_TEST PASS\n");
    return 0;
}
