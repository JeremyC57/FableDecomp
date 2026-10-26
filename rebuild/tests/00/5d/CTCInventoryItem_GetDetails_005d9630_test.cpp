// behaviour test for 005d9630: CTCInventoryItem::GetDetails(long) -- cache the def; a non-zero ItemDetails is looked up in the text bank
// (the bank's entry pointer is released after its text is copied out), anything else gives the empty wide string
#include <cstdio>
class CWideString { public: int Text; };
static CWideString g_emptyWideString = { -1 };
struct CGroupEntry {
    unsigned long IVRefCount; int deleted; int textId;
    void Release() { --IVRefCount; if (IVRefCount == 0) ++deleted; }
    CWideString GetText() const { CWideString w; w.Text = textId; return w; }
};
struct CEntryPointer {
    CGroupEntry* Object;
    explicit CEntryPointer(CGroupEntry* p) : Object(p) { if (Object) ++Object->IVRefCount; }
    CEntryPointer(const CEntryPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }   /* no NRVO under VC7.1 */
    ~CEntryPointer() { if (Object) { Object->Release(); Object = 0; } }
};
static CGroupEntry g_entries[8];
static int g_lookups = 0;
struct CDataBank {
    CEntryPointer GetPGroupEntry(unsigned long index, bool load) const
    {
        ++g_lookups;
        return CEntryPointer(load ? &g_entries[index] : 0);
    }
};
static CDataBank g_bank;
class CInventoryItemDef { public: unsigned long ItemDescription; unsigned long ItemDetails; };
static CInventoryItemDef g_defs[4];
static const CInventoryItemDef* s_CachedItemDef = 0;
static bool CacheDef(long defIndex)
{
    if (defIndex < 0 || defIndex >= 4) return false;
    s_CachedItemDef = &g_defs[defIndex];
    return true;
}
static CWideString Get(long defIndex)
{
    if (CacheDef(defIndex)) {
        if (s_CachedItemDef->ItemDetails > 0) {
            const unsigned long& textIndex = s_CachedItemDef->ItemDetails;
            return g_bank.GetPGroupEntry(textIndex, true).Object->GetText();
        }
    }
    return g_emptyWideString;
}
int main()
{
    int bad = 0;
    for (int i = 0; i < 8; ++i) { g_entries[i].IVRefCount = 1; g_entries[i].deleted = 0; g_entries[i].textId = 500 + i; }
    for (int d = 0; d < 4; ++d) { g_defs[d].ItemDescription = (unsigned long)(d * 2); g_defs[d].ItemDetails = (unsigned long)(d * 2); }
    g_defs[0].ItemDetails = 0;                 /* no text id: empty string, no lookup */
    if (Get(0).Text != -1 || g_lookups != 0) ++bad;
    if (Get(9).Text != -1 || g_lookups != 0) ++bad;   /* CacheDef fails */
    if (Get(2).Text != 504 || g_lookups != 1) ++bad;
    if (g_entries[4].IVRefCount != 1 || g_entries[4].deleted != 0) ++bad;   /* temporary pointer released */
    g_entries[6].IVRefCount = 0;            /* the temporary held the last reference: the entry is deleted */
    if (Get(3).Text != 506 || g_entries[6].deleted != 1) ++bad;
    if (bad) { std::printf("005d9630_TEST FAIL\n"); return 1; }
    std::printf("005d9630_TEST PASS\n");
    return 0;
}