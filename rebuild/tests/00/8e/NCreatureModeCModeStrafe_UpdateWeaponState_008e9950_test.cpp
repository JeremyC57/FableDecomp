#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
#include <string.h>
// Behaviour test for NCreatureMode::CModeStrafe::UpdateWeaponState (0x008E9950).
struct CTCBase;
struct CThing;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};

struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];       // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;           // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short m_weaponId;             // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  m_lifeFlags;            // +0x91 (bit 0: dead)
    bool IsDead() const { return (m_lifeFlags & 1) != 0; }
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (const T*)sel->component;
    }
};

struct CTCCarrying {                       // interface type 0x46
    CThing* primary;
    CThing* GetThingInPrimarySlot() const { return primary; }
};

struct CTCWeapon {                         // interface type 0x48
    bool usable; mutable CThing* lastUser;
    bool IsUsableBy(CThing* user) const { lastUser = user; return usable; }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00() {}
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {

struct CModeStrafe : CModeBase {           // vtable 0x01268588
    float         AnimMovementDistance;    // +0x1c
    void*         StrafeAnimName;          // +0x20 (CCharString)
    long          AnimIndex;               // +0x24
    bool          LastUsable;              // +0x28
    unsigned char _filter[0x0c];           // +0x2c CAnimFilterOfGroup
    long          ActiveWeapon;            // +0x38
    int changes; CThing* changed;
    void SetChangeOfWeapon(CThing* weapon) { ++changes; changed = weapon; }
    void UpdateWeaponState();
};

void CModeStrafe::UpdateWeaponState()
{
    const CTCCarrying* carrying;
    if (!ParentThing->GetInterface(&carrying, 0x46))
        return;
    CThing* weapon = carrying->GetThingInPrimarySlot();
    if ((weapon == 0 && ActiveWeapon != -1) || (weapon != 0 && weapon->m_weaponId != ActiveWeapon))
        SetChangeOfWeapon(weapon);
    bool usable = true;
    if (weapon && !weapon->IsDead() && weapon->HasTC(0x48))
        usable = weapon->GetTC<CTCWeapon>(0x48)->IsUsableBy(ParentThing);
    if ((usable && !LastUsable) || (!usable && LastUsable))
        SetChangeOfWeapon(weapon);
    LastUsable = usable;
}

}

using namespace NCreatureMode;
static void Present(CThing& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }
int main()
{
    int ok = 1;
    CThing owner, sword; memset(&owner, 0, sizeof(owner)); memset(&sword, 0, sizeof(sword));
    CTCCarrying carry; carry.primary = 0; CTCWeapon wpn; wpn.usable = false; wpn.lastUser = 0;
    CInterfaceMap::SEntry oe[1] = { { 0x46, (CTCBase*)&carry } }; owner.m_interfaces.first = oe; owner.m_interfaces.last = oe + 1;
    CInterfaceMap::SEntry se[1] = { { 0x48, (CTCBase*)&wpn } }; sword.m_interfaces.first = se; sword.m_interfaces.last = se + 1;
    sword.m_weaponId = 5;
    CModeStrafe m; m.ParentThing = &owner; m.LastUsable = true; m.ActiveWeapon = -1; m.changes = 0; m.changed = &sword;
    m.UpdateWeaponState();                       // no carrying component: nothing
    if (m.changes != 0 || !m.LastUsable) ok = 0;
    Present(owner, 0x46);
    m.UpdateWeaponState();                       // no weapon, active -1, usable stays true
    if (m.changes != 0 || !m.LastUsable) ok = 0;
    carry.primary = &sword;                      // new weapon 5 != -1: change; no TC 0x48 so usable
    m.UpdateWeaponState();
    if (m.changes != 1 || m.changed != &sword || !m.LastUsable) ok = 0;
    m.ActiveWeapon = 5; Present(sword, 0x48);    // same weapon, now unusable: flip true->false
    m.UpdateWeaponState();
    if (m.changes != 2 || m.LastUsable || wpn.lastUser != &owner) ok = 0;
    m.UpdateWeaponState();                       // stays unusable: no change
    if (m.changes != 2) ok = 0;
    wpn.usable = true; m.UpdateWeaponState();    // flip false->true
    if (m.changes != 3 || !m.LastUsable) ok = 0;
    sword.m_lifeFlags = 1; wpn.usable = false; m.UpdateWeaponState();   // dead weapon counts as usable
    if (m.changes != 3 || !m.LastUsable) ok = 0;
    if (ok) { printf("008e9950_TEST PASS\n"); return 0; }
    printf("008e9950_TEST FAIL\n"); return 1;
}