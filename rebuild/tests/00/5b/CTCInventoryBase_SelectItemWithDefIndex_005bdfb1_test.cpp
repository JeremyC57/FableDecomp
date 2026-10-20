// Behaviour test for 005bdfb1: src/005bdfb1.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryBase::SelectItemWithDefIndex(long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryBase::SelectItemWithDefIndex(long) -- finds the category of a thing def's inventory item and selects that item type in it
// Leaf: SelectItemWithDefIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDFB1, 117 bytes (+2 int3); frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<CThingDef>(long,
//          CDefPointer*) @ 0x0042AF3C, CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer*) @ 0x005BADE1,
//          STLport find_if<CCategory*, CCategoryHasIdentifier> @ 0x005C08D3 (__fastcall: first in ecx, last in edx,
//          the one-word predicate by value), NInventory::CCategory::SelectItemOfType(long) @ 0x005BDEC1.
// Shape: the thing def is a CDefPointer local (zeroed, released with a null test at the end); the item sub-def's
// pointer is read into ebx and released by hand without a null test; the predicate carries the item's category
// identifier (+0x3C); the end pointer stays in edi across the find.
// Layout: retail CTCInventoryBase Categories {first +0x20, last +0x24}; NInventory::CCategory 0x2C bytes.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRefCounted {
    virtual void v00();
    virtual void Destroy();                          // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};

struct CInventoryItemDef : CRefCounted {
    unsigned char _pad_0x08[0x34];
    long          CategoryIdentifier;                // +0x3C
};

template <class T> struct CRawDefPointer {
    T* Object;
    CRawDefPointer() : Object(0) {}
};

struct CThingDef : CRefCounted {
    bool GetSubDef(CRawDefPointer<CInventoryItemDef>* out) const;     // 0x005BADE1
};
void* FindDef_CThingDef_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CRawDefPointer<CInventoryItemDef>* out) const
{
    void* def = FindDef_CThingDef_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { T* object = Object; if (object && --object->RefCount == 0) object->Destroy(); }
    T* operator->() const { return Object; }
};

struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get(); // 0x0044C6B0
    bool GetDef(long index, CDefPointer<CThingDef>* out);              // 0x0042AF3C
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointer<CThingDef>* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

namespace NInventory {

struct CCategory {                                   // retail 0x2C bytes
    unsigned char _pad_0x00[0x2C];
    void SelectItemOfType(long defIndex);            // 0x005BDEC1
};

struct CCategoryHasIdentifier {
    long Identifier;
    CCategoryHasIdentifier(long identifier) : Identifier(identifier) {}
};

CCategory* __fastcall find_if(CCategory* first, CCategory* last, CCategoryHasIdentifier pred);   // 0x005C08D3

struct CTCInventoryBase {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x18];
    CCategory*    Categories_first;                  // +0x20 vector<CCategory>
    CCategory*    Categories_last;                   // +0x24
    CCategory*    Categories_end;                    // +0x28

    void SelectItemWithDefIndex(long defIndex);
};

void CTCInventoryBase::SelectItemWithDefIndex(long defIndex)
{
    CDefPointer<CThingDef> thingDef;
    CGameDefinitionManager::Get().GetDef(defIndex, &thingDef);
    CRawDefPointer<CInventoryItemDef> pItem;
    thingDef->GetSubDef(&pItem);
    CInventoryItemDef* item = pItem.Object;
    CCategoryHasIdentifier pred(item->CategoryIdentifier);
    CCategory* last = Categories_last;
    CCategory* found = find_if(Categories_first, last, pred);
    if (found != last)
        found->SelectItemOfType(defIndex);
    if (--item->RefCount == 0)
        item->Destroy();
}

}

// ---- stubs and driver ----
using namespace NInventory;
static void* g_def = 0; static void* g_sub = 0; static int g_destroyed = 0; static long g_id = -1; static long g_selected = -1; static CCategory* g_selectedCat = 0;
static CCategory* g_found = 0;
void* FindDef_CThingDef_GetSubDef_1() { return g_sub; }
void* FindDef_CGameDefinitionManager_GetDef_0() { return g_def; }
void CRefCounted::v00() {}
void CRefCounted::Destroy() { ++g_destroyed; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
void CCategory::SelectItemOfType(long defIndex) { g_selected = defIndex; g_selectedCat = this; }
CCategory* __fastcall NInventory::find_if(CCategory* first, CCategory* last, CCategoryHasIdentifier pred) { g_id = pred.Identifier; return g_found ? g_found : last; }
int main()
{
    CThingDef def; def.RefCount = 10; CInventoryItemDef item; item.RefCount = 10; item.CategoryIdentifier = 0x44;
    g_def = &def; g_sub = &item;
    CCategory cats[2];
    CTCInventoryBase inv; memset(&inv, 0, sizeof(inv)); inv.Categories_first = cats; inv.Categories_last = cats + 2;
    g_found = &cats[0]; inv.SelectItemWithDefIndex(9); CHECK(g_id == 0x44); CHECK(g_selected == 9 && g_selectedCat == &cats[0]); CHECK(item.RefCount == 9); CHECK(def.RefCount == 9);
    g_found = 0; g_selected = -1; inv.SelectItemWithDefIndex(3); CHECK(g_selected == -1); CHECK(item.RefCount == 8); CHECK(def.RefCount == 8);
    item.RefCount = 1; def.RefCount = 1; inv.SelectItemWithDefIndex(3); CHECK(g_destroyed == 2);
    if (fails == 0) printf("005bdfb1_TEST PASS\n"); else printf("005bdfb1_TEST FAIL\n");
    return fails;
}