// Behaviour test for 0x006AC690 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingPlayerCreature::LearnAbility(EHeroAbility ability, bool learn, long quickAccessSlot, bool silent) -- learning: unless already available, spawn the ability's inventory object at the hero, add it to the inventory, mark the ability available and give it a quick-access slot; unlearning: mark it unavailable and take the matching object out of the inventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AC690, length 628 bytes (`ret 0x10`).
// Callees: LowerBound @ 0x0040F020, CTCInventoryAbilities::IsAbilityAvailable @ 0x005EDF08 / GetAbilityPDef @ 0x005EDFD9
//   (CDefPointer by value) / SetAbilityAsAvailable @ 0x005F6BD6, CCharString(const char*, long) @ 0x0099EBF0 / dtor
//   @ 0x0099EAE0 ("" @ 0x0122D70E), CThing::GetPosition @ 0x004C73D0, the __fastcall thing factory @ 0x00703210,
//   CTCInventoryItem::AddToInventory @ 0x005D8D50 / RemoveFromInventory @ 0x005D8E20, CTCInventory::
//   GetQuickAccessNextFreeSlot @ 0x00582027 / SetThingAsQuickAccessItem @ 0x00584157, the inventory's virtual slot
//   47 (+0xbc) finding the thing of a def.
// Layout (retail): CThing presence bitset +0x20, component map +0x44, def index +0x8c, player number +0x90, Dead +0x91
//   bit 0; CHeroAbilityDef inventory object +0x98; interfaces 0x11 inventory, 0x1a inventory item, 0x6f abilities.
// Shape: the def pointer returned by value is homed in the dead `learn` slot; its release is the null-tested
//   CDefPointer dtor calling the pointee's inline ReleaseTemporary() (DecRef / GetRefCount).
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefPointeeBase {
    virtual void v00() { }
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CHeroAbilityDef : CDefPointeeBase {
    unsigned char _pad08[0x98 - 0x08];
    long InventoryObject;                               // +0x98
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
};
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing;
struct CTCInventory {
    virtual void v00() { }
    long GetQuickAccessNextFreeSlot(long item);                         // 0x00582027
    void SetThingAsQuickAccessItem(long item, long slot, bool a);       // 0x00584157
};
struct CTCInventoryItem {
    bool AddToInventory(CTCInventory* inventory, bool a, bool b, long price, bool c, bool d);   // 0x005D8D50
    void RemoveFromInventory();                                         // 0x005D8E20
};
struct CTCInventoryAbilities {
    bool IsAbilityAvailable(EHeroAbility ability);                      // 0x005EDF08
    CDefPointer<CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability);  // 0x005EDFD9
    void SetAbilityAsAvailable(EHeroAbility ability, bool available, bool a);   // 0x005F6BD6
};
struct CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210

class CThing {
public:
    void* m_vtable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short m_defGlobalIndex;                    // +0x8c
    unsigned char _pad8e[0x90 - 0x8e];
    signed char PlayerNumber;                           // +0x90
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (HasTC(type)) {
            *out = (T*)GetTC(type);
            return true;
        }
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CTCInventoryFind : CTCInventory {
    // slot 47 (+0xbc): the first inventory thing of a def
};
__forceinline CThing* CreateThingObject(long defIndex, const CThing* creature, const CCharString& name)
{
    return CreateThingObject(defIndex, &creature->GetPosition(), creature->PlayerNumber, 0, 0, name);
}
struct CTCInventoryVirtual {
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { }
    virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { } virtual void v10() { } virtual void v11() { }
    virtual void v12() { } virtual void v13() { } virtual void v14() { } virtual void v15() { } virtual void v16() { } virtual void v17() { }
    virtual void v18() { } virtual void v19() { } virtual void v20() { } virtual void v21() { } virtual void v22() { } virtual void v23() { }
    virtual void v24() { } virtual void v25() { } virtual void v26() { } virtual void v27() { } virtual void v28() { } virtual void v29() { }
    virtual void v30() { } virtual void v31() { } virtual void v32() { } virtual void v33() { } virtual void v34() { } virtual void v35() { }
    virtual void v36() { } virtual void v37() { } virtual void v38() { } virtual void v39() { } virtual void v40() { } virtual void v41() { }
    virtual void v42() { } virtual void v43() { } virtual void v44() { } virtual void v45() { } virtual void v46() { }
    virtual CThing* GetThingOfType(long defIndex);      // slot 47 (+0xbc)
};
__forceinline CThing* FindThing(CTCBase* inventory, long object)
{
    return ((CTCInventoryVirtual*)inventory)->GetThingOfType(object);
}
class CThingPlayerCreature : public CThing {
public:
    bool LearnAbility(EHeroAbility ability, bool learn, long quickAccessSlot, bool silent);
};

bool CThingPlayerCreature::LearnAbility(EHeroAbility ability, bool learn, long quickAccessSlot, bool silent)
{
    CTCInventoryAbilities* abilities;
    if (GetInterface(&abilities, 0x6f)) {
        if (learn) {
            if (!abilities->IsAbilityAvailable(ability)) {
                CDefPointer<CHeroAbilityDef> def = abilities->GetAbilityPDef(ability);
                CThing* item;
                {
                    CCharString name("");
                    item = CreateThingObject(def->InventoryObject, this, name);
                }
                if (item) {
                    CTCInventory* inventory = 0;
                    if (HasTC(0x11)) {
                        inventory = (CTCInventory*)GetTC(0x11);
                        CTCInventoryItem* itemTC = 0;
                        if (item->HasTC(0x1a))
                            itemTC = (CTCInventoryItem*)item->GetTC(0x1a);
                        itemTC->AddToInventory(inventory, false, true, 0, silent, false);
                    }
                    abilities->SetAbilityAsAvailable(ability, true, true);
                    if (quickAccessSlot != -1 || (quickAccessSlot = inventory->GetQuickAccessNextFreeSlot(item->m_defGlobalIndex)) != -1)
                        inventory->SetThingAsQuickAccessItem(item->m_defGlobalIndex, quickAccessSlot, false);
                    return true;
                }
            }
        } else {
            if (abilities->IsAbilityAvailable(ability)) {
                abilities->SetAbilityAsAvailable(ability, false, true);
                CDefPointer<CHeroAbilityDef> def = abilities->GetAbilityPDef(ability);
                if (HasTC(0x11)) {
                    CTCBase* inventory = GetTC(0x11);
                    CThing* item = FindThing(inventory, def->InventoryObject);
                    if (item && !item->Dead) {
                        CTCInventoryItem* itemTC = 0;
                        if (item->HasTC(0x1a))
                            itemTC = (CTCInventoryItem*)item->GetTC(0x1a);
                        itemTC->RemoveFromInventory();
                    }
                }
            }
        }
    }
    return false;
}

CThing* CTCInventoryVirtual::GetThingOfType(long defIndex) { (void)defIndex; return 0; }
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; (void)length; }
CCharString::~CCharString() { }
static int g_destroyed = 0;
void CDefPointeeBase::Destroy() { ++g_destroyed; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
static void* g_inventoryThis = 0; static long g_qaItem = -1, g_qaSlot = -2; static long g_nextFree = 3;
long CTCInventory::GetQuickAccessNextFreeSlot(long item) { (void)item; return g_nextFree; }
void CTCInventory::SetThingAsQuickAccessItem(long item, long slot, bool a) { g_inventoryThis = this; g_qaItem = item; g_qaSlot = slot; if (a) g_qaSlot += 100; }
static int g_added = 0, g_addSilent = -1, g_removed = 0; static void* g_addInv = 0;
bool CTCInventoryItem::AddToInventory(CTCInventory* inventory, bool a, bool b, long price, bool c, bool d)
{
    ++g_added; g_addInv = inventory; g_addSilent = c ? 1 : 0;
    if (a || !b || price != 0 || d) g_added += 100;
    return true;
}
void CTCInventoryItem::RemoveFromInventory() { ++g_removed; }
static int g_available = 0; static int g_setCalls = 0;
bool CTCInventoryAbilities::IsAbilityAvailable(EHeroAbility ability) { (void)ability; return g_available != 0; }
static CHeroAbilityDef g_def;
CDefPointer<CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability)
{
    static CDefPointer<CHeroAbilityDef> r;
    (void)ability; r.Object = &g_def; ++g_def.m_refCount; return r;
}
void CTCInventoryAbilities::SetAbilityAsAvailable(EHeroAbility ability, bool available, bool a)
{
    (void)ability; ++g_setCalls; g_available = available ? 1 : 0; if (!a) g_setCalls += 100;
}
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing g_item;
static long g_createDef = 0; static long g_createPlayer = 0;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name)
{
    g_createDef = defIndex; g_createPlayer = player;
    if (position->Z != 3.0f || set != 0 || flags != 0 || std::strcmp((const char*)name.m_pData, "") != 0) return 0;
    return defIndex == 0 ? 0 : &g_item;
}
struct CTestInventory : CTCInventoryVirtual {
    long asked;
    CThing* GetThingOfType(long defIndex) { asked = defIndex; return &g_item; }
};
static CTestInventory g_inv;
static CTCInventoryAbilities g_abil;
static CTCInventoryItem g_itemTC;

int main()
{
    static CThingPlayerCreature hero;
    std::memset(&hero, 0, sizeof(hero));
    CInterfaceMap::SEntry he[2];
    he[0].type = 0x11; he[0].component = (CTCBase*)(CTCInventoryVirtual*)&g_inv;
    he[1].type = 0x6f; he[1].component = (CTCBase*)&g_abil;
    hero.m_interfaces.first = he; hero.m_interfaces.last = he + 2;
    hero.PlayerNumber = -2;
    CInterfaceMap::SEntry ie[1];
    ie[0].type = 0x1a; ie[0].component = (CTCBase*)&g_itemTC;
    g_item.m_interfaces.first = ie; g_item.m_interfaces.last = ie + 1;
    g_item.m_presentBits[0] |= 1UL << 0x1a;
    g_item.m_defGlobalIndex = 77;
    g_def.m_refCount = 5; g_def.InventoryObject = 42;
    // no abilities interface -> false
    if (hero.LearnAbility((EHeroAbility)3, true, -1, false)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_setCalls != 0 || g_added != 0) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    hero.m_presentBits[0x6f >> 5] |= 1UL << (0x6f & 31);
    hero.m_presentBits[0] |= 1UL << 0x11;
    // learn: not available -> creates, adds, marks, quick access from next free slot
    if (!hero.LearnAbility((EHeroAbility)3, true, -1, true)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_createDef != 42 || g_createPlayer != -2 || g_added != 1 || g_addSilent != 1 || g_addInv != (void*)(CTCInventory*)(CTCInventoryVirtual*)&g_inv) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_available != 1 || g_setCalls != 1 || g_qaItem != 77 || g_qaSlot != 3) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_def.m_refCount != 5 || g_destroyed != 0) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    // learn again: already available -> false, nothing more
    if (hero.LearnAbility((EHeroAbility)3, true, 2, false)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_added != 1 || g_setCalls != 1) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    // explicit slot, no free slot needed
    g_available = 0; g_nextFree = -1; g_qaSlot = -2;
    if (!hero.LearnAbility((EHeroAbility)3, true, 6, false)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_qaSlot != 6 || g_addSilent != 0 || g_added != 2) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    // no free slot -> no quick access call
    g_available = 0; g_qaSlot = -2;
    if (!hero.LearnAbility((EHeroAbility)3, true, -1, false)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_qaSlot != -2 || g_added != 3) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    // create fails -> false, def released
    g_available = 0; g_def.InventoryObject = 0;
    if (hero.LearnAbility((EHeroAbility)3, true, -1, false)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_added != 3 || g_def.m_refCount != 5) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    // unlearn: available -> mark unavailable, remove item
    g_def.InventoryObject = 42; g_available = 1; g_setCalls = 0;
    if (hero.LearnAbility((EHeroAbility)3, false, -1, false)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_available != 0 || g_setCalls != 1 || g_removed != 1 || g_inv.asked != 42) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    // unlearn: not available -> nothing
    if (hero.LearnAbility((EHeroAbility)3, false, -1, false)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_setCalls != 1 || g_removed != 1) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    // unlearn: dead item -> not removed
    g_available = 1; g_item.Dead = 1;
    if (hero.LearnAbility((EHeroAbility)3, false, -1, false)) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    if (g_removed != 1 || g_setCalls != 2 || g_def.m_refCount != 5) { std::printf("006ac690_TEST FAIL\n"); return 1; }
    std::printf("006ac690_TEST PASS\n");
    return 0;
}