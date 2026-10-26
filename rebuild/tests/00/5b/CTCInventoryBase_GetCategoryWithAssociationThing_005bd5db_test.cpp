// Behaviour test for 005bd5db: the model below is a copy of src/005bd5db.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_gets = 0;
class CThing { public:};
struct CBaseIntelligentPointer {
    void* m_a; void* m_b;
    CThing* GetPItem() const { ++g_gets; return (CThing*)m_b; }
};
struct CCategory_Retail {
    CBaseIntelligentPointer PAssociatedThing;
    unsigned char _body[0x24];
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory_Retail* _SweepBegin1() const { return first; }
    CCategory_Retail& operator[](unsigned long n) { return *(_SweepBegin1() + n); }
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;
    CCategory_Retail* GetCategoryWithAssociationThing(CThing* thing);
};
CCategory_Retail* CTCInventoryBase_Retail::GetCategoryWithAssociationThing(CThing* thing)
{
    CCategory_Retail* result = 0;
    long count = Categories.size();
    long i;
    for (i = 0; i < count; ++i)
        if (Categories[i].PAssociatedThing.GetPItem() == thing)
            break;
    if (i < count)
        result = &Categories[i];
    return result;
}
static bool Run(int count, int wanted, int expectIndex, int expectGets)
{
    static CThing things[4]; static CCategory_Retail cats[4];
    for (int i = 0; i < 4; ++i) { std::memset(&cats[i], 0, sizeof(cats[i])); cats[i].PAssociatedThing.m_b = &things[i]; }
    CTCInventoryBase_Retail inv; inv.Categories.first = cats; inv.Categories.last = cats + count; inv.Categories.end_of_storage = cats + 4;
    g_gets = 0;
    CCategory_Retail* r = inv.GetCategoryWithAssociationThing(wanted >= 0 ? &things[wanted] : 0);
    if (r != (expectIndex >= 0 ? &cats[expectIndex] : 0)) return false;
    return g_gets == expectGets;
}
int main()
{
    bool ok = Run(4, 2, 2, 3) && Run(4, 0, 0, 1) && Run(3, 3, -1, 3) && Run(0, 1, -1, 0) && Run(4, -1, -1, 4);
    if (!ok) { std::printf("005bd5db_TEST FAIL\n"); return 1; }
    std::printf("005bd5db_TEST PASS\n");
    return 0;
}