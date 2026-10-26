#include <cstdio>
class CBaseObject { public: int id; };
static int g_live = 0;
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
    virtual ~CBaseIntelligentPointer() { --g_live; }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIP : CBaseIntelligentPointer {
    CIP() {}
    CIP(const CIP& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIP& operator=(const CIP& other) { SetPItem(other.GetPItem()); return *this; }
};
struct Comp { int tag; };
static CBaseObject* g_value = 0; static long g_hole = -1, g_len = -1; static CIP* g_first = 0; static int g_tag = 0;
void adjust_heap(CIP* first, long holeIndex, long len, CIP value, Comp comp)
{ g_first = first; g_hole = holeIndex; g_len = len; g_value = value.GetPItem(); g_tag = comp.tag; first[holeIndex] = value; }
void pop_heap_core(CIP* first, CIP* last, CIP* result, CIP value, Comp comp, int*)
{
    *result = *first;
    adjust_heap(first, 0, long(last - first), value, comp);
}
int main()
{
    CBaseObject o[4]; int k;
    {
        CIP heap[4]; for (k = 0; k < 4; ++k) { o[k].id = k; heap[k].SetPItem(&o[k]); }
        CIP v; v.SetPItem(&o[3]);
        Comp c; c.tag = 77;
        pop_heap_core(heap, heap + 3, heap + 3, v, c, 0);
        if (heap[3].GetPItem() != &o[0] || g_first != heap || g_hole != 0 || g_len != 3 || g_value != &o[3] || g_tag != 77 || heap[0].GetPItem() != &o[3])
        { std::printf("008081b0_TEST FAIL\n"); return 1; }
        if (g_live != 5) { std::printf("008081b0_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("008081b0_TEST FAIL\n"); return 1; }
    std::printf("008081b0_TEST PASS\n");
    return 0;
}