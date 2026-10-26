// Behaviour test for 00f37030: body copied from src/00f37030.cpp (CGPUScratchBufferTextureManager::FreeMemoryPage), callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SScalarTag { SScalarTag() { } };
static int g_grow = 0; static long g_growValue = 0;
struct CLongVector {
    long* first;
    long* last;
    long* end;
    long* begin() { return first; }
    unsigned long size() { return last - first; }
    long& operator[](unsigned long n) { return *(begin() + n); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
    {
        ++g_grow; g_growValue = value; CHECK(where == last && count == 1 && noTail);
        static long storage[8]; long n = last - first; for (long i = 0; i < n; ++i) storage[i] = first[i]; storage[n] = value; first = storage; last = storage + n + 1; end = storage + 8;
    }
    void push_back(long value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CResourcePage {
    long          ResourceIndex;
    CLongVector   Slots;
    unsigned char _pad10[0x0c];
};
struct CResourcePageVector {
    CResourcePage* first;
    CResourcePage* last;
    CResourcePage* end;
    CResourcePage* begin() { return first; }
    CResourcePage& operator[](long n) { return *(begin() + n); }
};
static int g_freed = 0; static long g_freedSlots[8]; static CResourcePage* g_page = 0;
class CGPUScratchBufferTextureManager { public:
    unsigned char       Resources[0x0c];
    CResourcePageVector MemoryPages;
    unsigned char       ScratchPools[0x0c];
    CLongVector         FreeTexturePageSlots;
    CLongVector         FreeTextureSlots;
    bool                ShutdownFlag;
    void FreeTextureSlot(long slot)
    {
        if (g_freed < 8) g_freedSlots[g_freed] = slot; ++g_freed;
        // the real call removes the page's first slot
        CLongVector& slots = g_page->Slots; for (long* p = slots.first; p + 1 < slots.last; ++p) *p = p[1]; --slots.last;
    }
    void FreeMemoryPage(long page);
};
void CGPUScratchBufferTextureManager::FreeMemoryPage(long page)
{
    MemoryPages[page].ResourceIndex = -1;
    while (MemoryPages[page].Slots.size() > 0)
        FreeTextureSlot(MemoryPages[page].Slots[0]);
    FreeTexturePageSlots.push_back(page);
}
int main()
{
    static CResourcePage pages[3]; memset(pages, 0, sizeof(pages)); long slots[3] = { 40, 41, 42 }; pages[2].ResourceIndex = 9; pages[2].Slots.first = slots; pages[2].Slots.last = slots + 3; pages[2].Slots.end = slots + 3; g_page = &pages[2];
    long freeBuf[1] = { 0 };
    CGPUScratchBufferTextureManager m; memset(&m, 0, sizeof(m)); m.MemoryPages.first = pages; m.MemoryPages.last = pages + 3; m.FreeTexturePageSlots.first = freeBuf; m.FreeTexturePageSlots.last = freeBuf; m.FreeTexturePageSlots.end = freeBuf + 1;
    m.FreeMemoryPage(2);
    CHECK(pages[2].ResourceIndex == -1); CHECK(g_freed == 3 && g_freedSlots[0] == 40 && g_freedSlots[1] == 41 && g_freedSlots[2] == 42 && pages[2].Slots.size() == 0);
    CHECK(m.FreeTexturePageSlots.last == freeBuf + 1 && freeBuf[0] == 2 && g_grow == 0);
    // a page without slots: nothing freed; the free-page list grows through the overflow path
    pages[1].ResourceIndex = 3; m.FreeMemoryPage(1); CHECK(pages[1].ResourceIndex == -1 && g_freed == 3 && g_grow == 1 && g_growValue == 1 && m.FreeTexturePageSlots.first[1] == 1);
    if (fails == 0) printf("00f37030_TEST PASS\n"); else printf("00f37030_TEST FAIL\n");
    return fails;
}