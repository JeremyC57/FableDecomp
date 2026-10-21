// CTCInventory::UseQuickAccessItem(long) -- uses the item in a quick-access slot: the carried thing if it is that item, else the inventory item
// Leaf: UseQuickAccessItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005840A6, 177 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//          CTCCarrying::GetThingInPrimarySlot() @ 0x006A1440, IS_THING_ALIVE(CBaseObject* const&) @ 0x0040DCE0
//          (__fastcall), and this class's own virtuals OnItemSelectedInHand(CThing*, bool*) (slot 34, 0x0058400A) and
//          OnItemSelected(long, bool*) (slot 33, 0x00583E26) from vtable 0x01250DE4.
// Shape: the carrying component (interface 0x46) comes from the `__forceinline` GetTC helper with a zeroed out
// pointer (`and [ebp-4],0`, the key `push 46h ; pop ebx` reused for the compare), fetched before the slot test.
// The slot is read with the STLport `*(begin() + slot)` form (scaled index kept in edi, begin re-read). The
// thing in hand stays in ebx: IS_THING_ALIVE takes `CBaseObject* const&`, so the CThing* binds through a converted
// temporary homed in the dead `slot` argument slot (as in the landed CTCVillage::DetachFromDummyVillager 0x0065B2C4) and matched on
// its 16-bit def index (+0x8C). Each branch declares its own `handled` out-flag (both in that slot's high byte): one
// flag declared above the `if` overlaps the condition's temporary, which then takes the dead component word [ebp-4].
// Layout: retail CTCInventory quick-access vector<long> at +0x148 (PDB +0x168; engine/CTCInventory.h carries the
// 16-byte PDB containers, so `this` is a local model as in the landed IsQuickAccessItemUsable 0x005849EA).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CBaseObject {
};

struct CThing : CBaseObject {
    unsigned char  _pad_0x00[0x20];
    CPresenceBits  m_present;                        // +0x20
    unsigned char  _pad_0x21[0x23];
    CInterfaceMap  m_interfaces;                     // +0x44
    unsigned char  _pad_0x4c[0x40];
    unsigned short DefIndex;                         // +0x8C
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component = 0;
        GetInterface(&component, wanted);
        return component;
    }
};

bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0

struct CTCCarrying {                                 // interface type 0x46
    CThing* GetThingInPrimarySlot();                 // 0x006A1440
};

struct CLongVector {                                 // std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() const { return _M_start; }
    const long& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CTCInventory {                                // vtable 0x01250DE4
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void OnItemSelected(long item, bool* handled);           // slot 33 (0x00583E26)
    virtual void OnItemSelectedInHand(CThing* thing, bool* handled); // slot 34 (0x0058400A)

    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x140];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)

    void UseQuickAccessItem(long slot);
};

void CTCInventory::UseQuickAccessItem(long slot)
{
    CTCCarrying* carrying = ParentThing->GetTC<CTCCarrying>(0x46);
    if (QuickAccessItems[slot] > 0) {
        CThing* inHand = carrying->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(inHand) && inHand->DefIndex == QuickAccessItems[slot]) {
            bool handled;
            OnItemSelectedInHand(inHand, &handled);
        }
        else {
            bool handled;
            OnItemSelected(QuickAccessItems[slot], &handled);
        }
    }
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
bool CPresenceBits::test(long bit) { return (((const unsigned long*)this)[bit >> 5] >> (bit & 31)) & 1; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static bool g_alive = true; static const CBaseObject* g_aliveArg = 0;
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing) { g_aliveArg = thing; return thing != 0 && g_alive; }
static CThing* g_inHand = 0;
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_inHand; }
static int g_selected = 0, g_selectedInHand = 0; static long g_lastItem = 0; static CThing* g_lastThing = 0;
static bool* g_handledPtr = 0;
void CTCInventory::v00() { } void CTCInventory::v01() { } void CTCInventory::v02() { } void CTCInventory::v03() { }
void CTCInventory::v04() { } void CTCInventory::v05() { } void CTCInventory::v06() { } void CTCInventory::v07() { }
void CTCInventory::v08() { } void CTCInventory::v09() { } void CTCInventory::v10() { } void CTCInventory::v11() { }
void CTCInventory::v12() { } void CTCInventory::v13() { } void CTCInventory::v14() { } void CTCInventory::v15() { }
void CTCInventory::v16() { } void CTCInventory::v17() { } void CTCInventory::v18() { } void CTCInventory::v19() { }
void CTCInventory::v20() { } void CTCInventory::v21() { } void CTCInventory::v22() { } void CTCInventory::v23() { }
void CTCInventory::v24() { } void CTCInventory::v25() { } void CTCInventory::v26() { } void CTCInventory::v27() { }
void CTCInventory::v28() { } void CTCInventory::v29() { } void CTCInventory::v30() { } void CTCInventory::v31() { }
void CTCInventory::v32() { }
void CTCInventory::OnItemSelected(long item, bool* handled) { ++g_selected; g_lastItem = item; g_handledPtr = handled; *handled = true; }
void CTCInventory::OnItemSelectedInHand(CThing* thing, bool* handled) { ++g_selectedInHand; g_lastThing = thing; g_handledPtr = handled; *handled = true; }
static CThing g_owner, g_sword, g_axe;
static CTCCarrying g_carrying;
static CInterfaceMap::SEntry g_entries[2];
static long g_items[4] = { 0, 17, 23, -1 };
int main()
{
    g_entries[0].type = 0x46; g_entries[0].component = (CTCBase*)&g_carrying;
    g_entries[1].type = 0x50; g_entries[1].component = 0;
    g_owner.m_interfaces.first = g_entries; g_owner.m_interfaces.last = g_entries + 2;
    ((unsigned long*)&g_owner.m_present)[0x46 >> 5] = 1UL << (0x46 & 31);
    g_sword.DefIndex = 17; g_axe.DefIndex = 99;
    CTCInventory inv;
    inv.ParentThing = &g_owner;
    inv.QuickAccessItems._M_start = g_items; inv.QuickAccessItems._M_finish = g_items + 4;
    inv.QuickAccessItems._M_end_of_storage = g_items + 4;
    g_inHand = &g_sword;
    inv.UseQuickAccessItem(1);                      // the sword in hand is that item
    CHECK(g_selectedInHand == 1 && g_lastThing == &g_sword && g_selected == 0 && g_aliveArg == &g_sword && g_handledPtr != 0);
    inv.UseQuickAccessItem(2);                      // a different item: selected from the inventory
    CHECK(g_selectedInHand == 1 && g_selected == 1 && g_lastItem == 23);
    g_alive = false;                                // the thing in hand is dead
    inv.UseQuickAccessItem(1);
    CHECK(g_selectedInHand == 1 && g_selected == 2 && g_lastItem == 17);
    g_alive = true; g_inHand = 0;                   // nothing in hand
    inv.UseQuickAccessItem(1);
    CHECK(g_selected == 3 && g_lastItem == 17 && g_aliveArg == 0);
    inv.UseQuickAccessItem(0);                      // empty slot: nothing
    inv.UseQuickAccessItem(3);
    CHECK(g_selected == 3 && g_selectedInHand == 1);
    g_inHand = &g_axe;
    inv.UseQuickAccessItem(1);
    CHECK(g_selected == 4 && g_selectedInHand == 1);
    if (fails == 0) printf("005840a6_TEST PASS\n");
    return fails;
}