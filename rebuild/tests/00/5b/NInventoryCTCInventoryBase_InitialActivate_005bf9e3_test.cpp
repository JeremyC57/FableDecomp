// Behaviour test for 0x005BF9E3 CTCInventoryBase::InitialActivate
#include <cstdio>
#include <cstring>
class CCharString { public: const char* m_pData; };
static int g_init = 0, g_released = 0, g_selected[8], g_nsel = 0;
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& n) { return std::strcmp(n.m_pData, "BAD") == 0 ? -1 : (long)std::strlen(n.m_pData); }
};
struct CCharStringVector { CCharString* first; CCharString* last; CCharString* end_of_storage; };
struct CItemVector { int n; };
struct PItems { CItemVector* Data; void Release() { ++g_released; Data = 0; } void operator=(int) { Release(); Data = 0; } };
struct PNames { CCharStringVector* Data; void Release() { ++g_released; Data = 0; } void operator=(int) { Release(); Data = 0; } };
struct CInv {
    PItems SerialisedItems; PNames SerialisedSelectedItems; CDefinitionManager dm;
    CDefinitionManager* GetDefinitionManager() { return &dm; }
    void InitialiseInventoryWithItems(CItemVector* v) { g_init += v->n; }
    void SelectItemWithDefIndex(long i) { g_selected[g_nsel++] = (int)i; }
    void InitialActivate()
    {
        if (SerialisedItems.Data) {
            InitialiseInventoryWithItems(SerialisedItems.Data);
            SerialisedItems = 0;
        }
        if (SerialisedSelectedItems.Data) {
            for (CCharString* it = SerialisedSelectedItems.Data->first; it != SerialisedSelectedItems.Data->last; ++it) {
                long index = GetDefinitionManager()->GetDefGlobalIndexFromName(*it);
                if (index > 0)
                    SelectItemWithDefIndex(index);
            }
            SerialisedSelectedItems = 0;
        }
    }
};
int main()
{
    CItemVector items = { 3 };
    CCharString names[3] = { { "AB" }, { "BAD" }, { "ABCD" } };
    CCharStringVector nv = { names, names + 3, names + 3 };
    CInv inv; inv.SerialisedItems.Data = &items; inv.SerialisedSelectedItems.Data = &nv;
    inv.InitialActivate();
    bool ok = g_init == 3 && g_released == 2 && g_nsel == 2 && g_selected[0] == 2 && g_selected[1] == 4
        && inv.SerialisedItems.Data == 0 && inv.SerialisedSelectedItems.Data == 0;
    inv.InitialActivate();
    if (g_released != 2) ok = false;
    if (!ok) { std::printf("005bf9e3_TEST FAIL\n"); return 1; }
    std::printf("005bf9e3_TEST PASS\n");
    return 0;
}