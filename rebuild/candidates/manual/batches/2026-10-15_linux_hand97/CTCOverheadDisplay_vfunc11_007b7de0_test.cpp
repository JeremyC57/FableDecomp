// Behaviour test for CTCOverheadDisplay::vfunc_11 @ 0x007B7DE0: the whole entry range is erased.
#include <cstdio>
struct CEnginePrimitiveHandle { long v; };
static CEnginePrimitiveHandle* g_first = 0;
static CEnginePrimitiveHandle* g_last = 0;
struct CRetailHandleVector {
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last)
    {
        g_first = first; g_last = last;
        m_pLast = first;                            // everything in [first, last) removed
        return first;
    }
    void clear() { erase(m_pFirst, m_pLast); }
};
struct CTCOverheadDisplay {
    CRetailHandleVector PrimitiveHandles;
    void vfunc_11(long) { PrimitiveHandles.clear(); }
};
int main()
{
    CEnginePrimitiveHandle items[3];
    CTCOverheadDisplay d;
    d.PrimitiveHandles.m_pFirst = items; d.PrimitiveHandles.m_pLast = items + 3; d.PrimitiveHandles.m_pEnd = items + 3;
    d.vfunc_11(7);
    if (g_first != items || g_last != items + 3 || d.PrimitiveHandles.m_pLast != items) { std::printf("007b7de0_TEST FAIL\n"); return 1; }
    std::printf("007b7de0_TEST PASS\n");
    return 0;
}
