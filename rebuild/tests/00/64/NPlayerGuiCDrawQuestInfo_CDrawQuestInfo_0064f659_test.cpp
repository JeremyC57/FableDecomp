// Behaviour test for 0064f659: body copied from src/0064f659.cpp, callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
template <class P> struct CAllocProxy { P p; CAllocProxy(P v) : p(v) { } operator P() const { return p; } CAllocProxy& operator=(P v) { p = v; return *this; } };
static int g_baseCtor = 0, g_wstrCtor = 0, g_longList = 0, g_elemList = 0, g_resize = 0, g_erase = 0; static long g_resizeCount = -1;
static const void* g_baseGui = 0; static const void* g_baseList = 0; static const void* g_eraseFirst = 0; static const void* g_eraseLast = 0;
struct CPlayerGui; struct CEnginePrimitiveHandle;
struct SAllocator { SAllocator() { } };
struct CWideString { unsigned short* m_pData; CWideString() : m_pData(0) { ++g_wstrCtor; } };
namespace NPlayerGui {
struct CRetailHandleVector {
    CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CAllocProxy<CEnginePrimitiveHandle*> m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
    void Resize(long count) { ++g_resize; g_resizeCount = count; }
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last) { ++g_erase; g_eraseFirst = first; g_eraseLast = last; return first; }
    void clear() { erase(begin(), end()); }
};
struct CRetailLongList { void* head; explicit CRetailLongList(const SAllocator& allocator = SAllocator()) : head(0) { ++g_longList; } };
struct CRetailElementList { void* head; explicit CRetailElementList(const SAllocator& allocator = SAllocator()) : head(0) { ++g_elemList; } };
struct CDrawBase {
    virtual void v00() {}
    unsigned char _body[0x1c - 4];
    CRetailHandleVector PrimitiveHandles;
    CDrawBase(CPlayerGui* gui, void* drawList) { ++g_baseCtor; g_baseGui = gui; g_baseList = drawList; }
};
struct CDrawQuestInfo : CDrawBase {
    virtual void v00() {}
    long GuiMode; float FadeInTimer; float MiniModeTimer; bool MiniMode; long FramesUntilMiniMode; long MiniGameElementIndex;
    CRetailLongList OracleSymbols; CRetailHandleVector OraclePrimitiveHandles; CWideString Name; CWideString Text; CRetailElementList Elements;
    CDrawQuestInfo(CPlayerGui* gui, void* drawList);
};
CDrawQuestInfo::CDrawQuestInfo(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), GuiMode(0), FadeInTimer(0.0f), MiniModeTimer(0.0f), MiniMode(false), FramesUntilMiniMode(0),
      MiniGameElementIndex(0), OracleSymbols(), OraclePrimitiveHandles(), Name(), Text(), Elements()
{
    PrimitiveHandles.Resize(9);
    OraclePrimitiveHandles.clear();
}
}
int main()
{
    unsigned char raw[sizeof(NPlayerGui::CDrawQuestInfo)]; memset(raw, 0xAA, sizeof(raw)); int gui = 0, list = 0;
    NPlayerGui::CDrawQuestInfo* d = new (raw) NPlayerGui::CDrawQuestInfo((CPlayerGui*)&gui, &list);
    CHECK(g_baseCtor == 1 && g_baseGui == &gui && g_baseList == &list && g_wstrCtor == 2 && g_longList == 1 && g_elemList == 1);
    CHECK(g_resize == 1 && g_resizeCount == 9 && g_erase == 1 && g_eraseFirst == 0 && g_eraseLast == 0);
    CHECK(d->GuiMode == 0 && d->FadeInTimer == 0.0f && d->MiniModeTimer == 0.0f && d->MiniMode == false && d->FramesUntilMiniMode == 0 && d->MiniGameElementIndex == 0);
    CHECK(d->OraclePrimitiveHandles.m_pFirst == 0 && d->OraclePrimitiveHandles.m_pLast == 0 && d->OraclePrimitiveHandles.m_pEnd.p == 0);
    CHECK(d->Name.m_pData == 0 && d->Text.m_pData == 0 && d->OracleSymbols.head == 0 && d->Elements.head == 0);
    if (fails == 0) printf("0064f659_TEST PASS\n"); else printf("0064f659_TEST FAIL\n");
    return fails;
}