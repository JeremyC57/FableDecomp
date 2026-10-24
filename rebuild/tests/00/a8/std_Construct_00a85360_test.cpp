#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N00a85360 {
struct CArrayNodes {                                 // retail 12-byte STLport-vector body (void*)
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    CArrayNodes() { std::memset(this, 0, sizeof(CArrayNodes)); }
    CArrayNodes(const CArrayNodes& other) { std::memcpy(this, &other, sizeof(CArrayNodes)); ++g_extCopies; }
};
struct CLineListVector {                                 // retail 12-byte STLport-vector body (void)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CLineListVector() { std::memset(this, 0, sizeof(CLineListVector)); }
    CLineListVector(const CLineListVector& other) { std::memcpy(this, &other, sizeof(CLineListVector)); ++g_extCopies; }
};
struct CSwitchableLineVector {                                 // retail 12-byte STLport-vector body (void)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CSwitchableLineVector() { std::memset(this, 0, sizeof(CSwitchableLineVector)); }
    CSwitchableLineVector(const CSwitchableLineVector& other) { std::memcpy(this, &other, sizeof(CSwitchableLineVector)); ++g_extCopies; }
};
struct CKeyPairCompareLess { };
struct CSwitchableLineMap : CSwitchableLineVector {   // CVectorMap<unsigned __int64, list<C2DLineF>, ...>: vector base, Compare +0xC, Dirty +0xD
    CKeyPairCompareLess Compare;                 // +0x0C (empty)
    bool                Dirty;                   // +0x0D
};
struct CNavigationLayer {                        // CNavQuadTree::CNavigationLayer (retail sizeof 0x28)
    CArrayNodes        Nodes;                    // +0x00 CArray<CANavQuadTreeNode*>
    CLineListVector    LineLists;                // +0x0C vector<list<C2DLineF>>
    CSwitchableLineMap SwitchableLineLists;      // +0x18
};

void __fastcall _Construct(CNavigationLayer* p, const CNavigationLayer& value)
{
    if (p != 0)
        new (p) CNavigationLayer(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00a85360;
    CNavigationLayer src;
    src.Nodes._M_start = reinterpret_cast<void**>(0x10);
    src.LineLists._M_finish = reinterpret_cast<void*>(32);
    src.SwitchableLineLists._M_start = reinterpret_cast<void*>(48);
    src.SwitchableLineLists.Dirty = true;
    CNavigationLayer* slot = reinterpret_cast<CNavigationLayer*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 3 && g_links == 0;
    if (!(slot->Nodes._M_start == src.Nodes._M_start)) ok = false;
    if (!(slot->LineLists._M_finish == src.LineLists._M_finish)) ok = false;
    if (!(slot->SwitchableLineLists._M_start == src.SwitchableLineLists._M_start)) ok = false;
    if (!(slot->SwitchableLineLists.Dirty == src.SwitchableLineLists.Dirty)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 3 || g_links != 0) ok = false;
    if (!ok) { std::printf("00a85360_TEST FAIL\n"); return 1; }
    std::printf("00a85360_TEST PASS\n");
    return 0;
}