// Behaviour test for NThingMap::CThingMapManager::CThingMapManager (0x005191E0): both arrays empty, the active-map set
// empty with an allocated red header whose leftmost/rightmost point at itself and no root.
#include <cstdio>
#include <cstdlib>
static int g_baseCalls = 0;
struct CBaseClassNonCopyable { CBaseClassNonCopyable() { ++g_baseCalls; } virtual ~CBaseClassNonCopyable() { } };
template <class T> struct CArray { T* m_pFirst; T* m_pLast; T* m_pEnd; CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct SNode { char Color; SNode* Parent; SNode* Left; SNode* Right; long Value; };
struct CSetBase { SNode* m_pHeader; CSetBase() : m_pHeader(0) { m_pHeader = (SNode*)std::malloc(sizeof(SNode)); } };
struct CSet : CSetBase {
    unsigned long m_nodeCount;
    CSet() : m_nodeCount(0) { m_pHeader->Color = 0; m_pHeader->Parent = 0; m_pHeader->Left = m_pHeader; m_pHeader->Right = m_pHeader; }
};
struct CThingMapManager : CBaseClassNonCopyable {
    void* PWorld; CArray<int> ThingMaps; CArray<int*> ActiveThingMaps; CSet ActiveGameMaps;
    CThingMapManager() { }
};
int main()
{
    CThingMapManager m;
    SNode* h = m.ActiveGameMaps.m_pHeader;
    bool ok = g_baseCalls == 1 && m.ThingMaps.m_pFirst == 0 && m.ThingMaps.m_pLast == 0 && m.ThingMaps.m_pEnd == 0
           && m.ActiveThingMaps.m_pFirst == 0 && m.ActiveThingMaps.m_pLast == 0 && m.ActiveThingMaps.m_pEnd == 0
           && h != 0 && h->Color == 0 && h->Parent == 0 && h->Left == h && h->Right == h && m.ActiveGameMaps.m_nodeCount == 0;
    std::free(h);
    if (!ok) { std::printf("005191e0_TEST FAIL\n"); return 1; }
    std::printf("005191e0_TEST PASS\n");
    return 0;
}