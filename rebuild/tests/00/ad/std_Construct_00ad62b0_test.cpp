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
namespace N00ad62b0 {
struct CIndexList {                              // set<unsigned long> body, 12 bytes in retail
    void*         m_header;
    unsigned long m_nodeCount;
    unsigned long m_compare;
    CIndexList() { std::memset(this, 0, sizeof(CIndexList)); }
    CIndexList(const CIndexList& other) { std::memcpy(this, &other, sizeof(CIndexList)); ++g_extCopies; }
};
struct CUniqueVertex {                           // QuadricOptimiserInternals::CUniqueVertex (retail sizeof 0x1C)
    bool       Exists;                           // +0x00
    CIndexList VertexIndices;                    // +0x04
    CIndexList FaceList;                         // +0x10
};

void __fastcall _Construct(CUniqueVertex* p, const CUniqueVertex& value)
{
    if (p != 0)
        new (p) CUniqueVertex(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00ad62b0;
    CUniqueVertex src;
    src.Exists = true;
    src.VertexIndices.m_nodeCount = 3u;
    src.FaceList.m_header = reinterpret_cast<void*>(64);
    CUniqueVertex* slot = reinterpret_cast<CUniqueVertex*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->Exists == src.Exists)) ok = false;
    if (!(slot->VertexIndices.m_nodeCount == src.VertexIndices.m_nodeCount)) ok = false;
    if (!(slot->FaceList.m_header == src.FaceList.m_header)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("00ad62b0_TEST FAIL\n"); return 1; }
    std::printf("00ad62b0_TEST PASS\n");
    return 0;
}