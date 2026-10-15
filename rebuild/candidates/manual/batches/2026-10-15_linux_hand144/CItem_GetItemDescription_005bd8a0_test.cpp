// Behaviour test for 0x005bd8a0 (CItem::GetItemDescription): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CWideString { const char* text; };
static CWideString g_empty = { "" };
struct CCounted { long refs; int deleted; CCounted() : refs(1), deleted(0) {} void Release() { if (--refs == 0) deleted = 1; } };
struct CDef : CCounted { long DescriptionTextIndex; };
struct CEntry : CCounted { CWideString text; CWideString GetText() const { return text; } };
struct CBank { CEntry* entry; long asked;
    CEntry* GetPGroupEntry(long index, bool) { asked = index; ++entry->refs; return entry; } };
struct CItem {
    long ItemType; long NumberOfItems; CDef* def; CBank* bank;
    CDef* GetInventoryItemDef() const { ++def->refs; return def; }
    CWideString GetItemDescription() const
    {
        if (NumberOfItems == 0) return g_empty;
        CDef* d = GetInventoryItemDef(); long textIndex = d->DescriptionTextIndex; d->Release();
        if (textIndex > 0) { CEntry* e = bank->GetPGroupEntry(textIndex, true); CWideString r = e->GetText(); e->Release(); return r; }
        return g_empty;
    }
};
static void Run()
{
    CDef d; d.DescriptionTextIndex = 12; CEntry e; e.text.text = "A sword"; CBank b; b.entry = &e; b.asked = 0;
    CItem it; it.ItemType = 1; it.NumberOfItems = 2; it.def = &d; it.bank = &b;
    CHECK(std::strcmp(it.GetItemDescription().text, "A sword") == 0 && b.asked == 12 && d.refs == 1 && e.refs == 1);
    d.DescriptionTextIndex = 0; CHECK(it.GetItemDescription().text == g_empty.text);
    it.NumberOfItems = 0; CHECK(it.GetItemDescription().text == g_empty.text && d.refs == 1);
}

int main()
{
    Run();
    if (g_fail) { std::printf("005bd8a0_TEST FAIL\n"); return 1; }
    std::printf("005bd8a0_TEST PASS\n");
    return 0;
}
