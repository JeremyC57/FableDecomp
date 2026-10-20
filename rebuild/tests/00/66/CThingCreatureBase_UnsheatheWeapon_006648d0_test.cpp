// Behaviour test for 006648d0: body copied from src/006648d0.cpp (CThingCreatureBase::UnsheatheWeapon), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1) : PStringData((void*)text) { CHECK(length == -1); }
    ~CCharString() { }
};
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void Destroy0() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CCarryableDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long ActiveCarrySlot;
    long SecondaryActiveCarrySlot;
    long PassiveCarrySlot;
};
static CCarryableDef* g_carryDef = 0; static int g_getSubDef = 0;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CCarryableDef>* out)
    {
        ++g_getSubDef;
        if (!g_carryDef) return false;
        ++g_carryDef->RefCount; out->Object = g_carryDef; return true;
    }
};
class CThing;
class CThingObject;
static bool g_isCarrying = false; static CThing* g_carried = 0; static int g_removed = 0; static CThing* g_removedThing = 0;
static int g_put = 0; static CThing* g_putThing = 0; static long g_putSlot = -1; static bool g_putFlag = false; static long g_askedDef = -1;
struct CTCCarrying {
    bool IsCarryingThing(long defIndex) { g_askedDef = defIndex; return g_isCarrying; }
    CThing* GetCarriedThing(long defIndex) { CHECK(defIndex == g_askedDef); return g_carried; }
    void RemoveThing(CThing* thing) { ++g_removed; g_removedThing = thing; }
    void PutThingInCarrySlot(CThing* thing, long slot, bool flag) { ++g_put; g_putThing = thing; g_putSlot = slot; g_putFlag = flag; }
};
static C3DVector g_position = { 1, 2, 3 };
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;
    unsigned char _pad74[0x90 - 0x74];
    signed char PlayerNumber;
    const C3DVector& GetPosition() const { return g_position; }
    CCarryableDef* GetCarryableDef()
    {
        CDefPointer<const CCarryableDef> out;
        PBaseDef->GetSubDef(&out);
        return const_cast<CCarryableDef*>(out.Object);
    }
    CTCCarrying* GetCarrying()
    {
        CTCCarrying* carrying = 0;
        if (UsingComponents[0x46 >> 5] & (1UL << (0x46 & 31))) {
            long type = 0x46;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || 0x46 < it->type)
                it = map->last;
            carrying = (CTCCarrying*)it->component;
        }
        return carrying;
    }
};
static int g_created = 0; static CThing* g_createdThing = 0; static long g_createDef = -1; static long g_createPlayer = -1; static const char* g_createName = 0; static C3DVector* g_createPos = 0;
CThingObject* __fastcall CreateObject(long definition, C3DVector* position, long playerID, float* orientation, unsigned long flags, const CCharString& name)
{
    ++g_created; g_createDef = definition; g_createPos = position; g_createPlayer = playerID; g_createName = (const char*)name.PStringData; CHECK(orientation == 0 && flags == 0);
    return (CThingObject*)g_createdThing;
}
class CThingCreatureBase : public CThing {
public:
    void UnsheatheWeapon(long weaponDefIndex, bool useActiveSlot);
};

void CThingCreatureBase::UnsheatheWeapon(long weaponDefIndex, bool useActiveSlot)
{
    if (weaponDefIndex <= 0)
        return;
    CTCCarrying* carrying = GetCarrying();
    CCarryableDef* def;
    CThing* weapon;
    if (carrying->IsCarryingThing(weaponDefIndex)) {
        weapon = carrying->GetCarriedThing(weaponDefIndex);
        carrying->RemoveThing(weapon);
    } else {
        weapon = (CThing*)CreateObject(weaponDefIndex, const_cast<C3DVector*>(&GetPosition()), PlayerNumber, 0, 0, "");
    }
    CDefPointer<const CCarryableDef> out;
    weapon->PBaseDef->GetSubDef(&out);
    def = const_cast<CCarryableDef*>(out.Object);
    carrying->PutThingInCarrySlot(weapon, useActiveSlot ? def->ActiveCarrySlot : def->SecondaryActiveCarrySlot, true);
    if (--def->RefCount == 0)
        def->Destroy();
}
int main()
{
    static CThingCreatureBase creature; static CThing weapon, other; static CDefClassBase classDef; static CCarryableDef carry; static CTCCarrying carrying;
    memset(&creature, 0, sizeof(creature)); memset(&weapon, 0, sizeof(weapon)); memset(&other, 0, sizeof(other));
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x46, (CTCBase*)&carrying } };
    creature.Components.first = entries; creature.Components.last = entries + 2; creature.UsingComponents[0x46 >> 5] |= 1UL << (0x46 & 31); creature.PlayerNumber = 3;
    weapon.PBaseDef = &classDef; other.PBaseDef = &classDef; carry.RefCount = 1; carry.ActiveCarrySlot = 7; carry.SecondaryActiveCarrySlot = 8; g_carryDef = &carry;
    // no weapon def: nothing happens
    creature.UnsheatheWeapon(0, true); creature.UnsheatheWeapon(-4, true); CHECK(g_put == 0 && g_created == 0 && g_askedDef == -1);
    // already carried: removed from its slot and put in the active slot
    g_isCarrying = true; g_carried = &weapon; creature.UnsheatheWeapon(55, true);
    CHECK(g_askedDef == 55 && g_removed == 1 && g_removedThing == &weapon && g_created == 0); CHECK(g_getSubDef == 1 && g_put == 1 && g_putThing == &weapon && g_putSlot == 7 && g_putFlag);
    CHECK(carry.RefCount == 1 && g_destroyed == 0);
    // not carried: created at the creature's position for its player with an empty name, secondary slot
    g_isCarrying = false; g_createdThing = &other; creature.UnsheatheWeapon(66, false);
    CHECK(g_created == 1 && g_createDef == 66 && g_createPos == &g_position && g_createPlayer == 3 && g_createName[0] == 0); CHECK(g_removed == 1 && g_put == 2 && g_putThing == &other && g_putSlot == 8);
    // a def held only by the lookup is destroyed after the put
    carry.RefCount = 0; creature.UnsheatheWeapon(66, true); CHECK(g_destroyed == 1 && g_putSlot == 7);
    if (fails == 0) printf("006648d0_TEST PASS\n"); else printf("006648d0_TEST FAIL\n");
    return fails;
}