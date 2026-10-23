#include <cstdio>
#ifndef _MSC_VER
// The host fixture widens pointers; the target fixture includes the shared 32-bit header.
#define FABLE_ENGINE_CAIBRAIN_H
struct CThingAICreature;
struct CAIBrain { CThingAICreature* Creature; };
#define __forceinline inline
#define __fastcall
#define __declspec(x)
#endif
// CAIStateGroup_CreatureTakeCover::IsRunnableLeaf() -- gate on the projectile-carrying module, then require a live target outside melee, a projectile weapon, and the cover-base leaf check.
// Retail: Fable.exe (TLC, VC7.1), @0x00979D80, 258 bytes.
// Callees: CAIStateGroupBase::GetPWorld @0x008FCFC0; NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon::IsRunnable @0x00982890 (temporary vtable 0x0128FFD4); CBaseIntelligentPointer::GetPItem @0x00A01B50; IsInMeleeWithCombatWheelAttackers @0x00979910; CVectorMap::LowerBound @0x004365B0; CTCCarrying::GetThingInPrimarySlot @0x006A1440; CTCWeapon::IsProjectileWeapon @0x005DAA40; qualified base IsRunnableLeaf @0x00970F30.
// The PDB header only exposes a flattened 0x50-byte base area. The fields at +0x14/+0x1f and inherited call behavior follow the landed CreatureTakeCover/CreatureUseCoverBase relatives. GetInterface inlines the presence-bit test at +0x28, then searches the sorted component map.
struct CThing;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer   // inline ctor stores the template's vtable after the base ctor
{
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    T* Get() const { return (T*)GetPItem(); }
};

struct CTCBase;

struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                  // +0x00
    SEntry* last;                                   // +0x04
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CInterfaceMap::SEntry* FindEntry(long type)    // the entry; `->component` read at the call site
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->type <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

#include "engine/CAIBrain.h"

struct CTCCarrying                                  // interface type 0x46 (word 2 bit 6)
{
    CThing* GetThingInPrimarySlot();                // 0x006A1440
};

struct CTCWeapon                                    // interface type 0x48 (word 2 bit 8)
{
    bool IsProjectileWeapon();                      // 0x005DAA40
};

struct CThingCreatureBase : CThing
{
    bool IsIncapacitated();                         // 0x00661E80 (flags byte +0x15c: bit 2, else bit 3; manifest "CText::GetAlignment" is propagated)
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0)
    {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}


bool __fastcall IsInMeleeWithCombatWheelAttackers(CThing* creature);   // 0x00979910

// pointer form of the alive test: one GetPItem read
__forceinline bool IS_THING_ALIVE(CThing* thing)
{
    if (thing != 0)
    {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

#pragma pack(push, 1)
struct CWorld;
struct CAIStateGroup_CreatureTakeCover
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CIntelligentPointer<const CThing> PTargetThing; // +0x14
    unsigned char _pad1c[3];                        // +0x1c
    CIntelligentPointer<CThing> PCoverThing;        // +0x1f
    CWorld* GetPWorld(); // 0x008FCFC0
    bool IsCoverBaseRunnable(); // 0x00970F30
    bool IsRunnableLeaf();
};
#pragma pack(pop)

struct CWorld;
namespace NCreatureAI {
struct __declspec(novtable) CIsRunnableModule {
    CWorld* World; CThing* Creature;
    CIsRunnableModule(CWorld* world, CThing* creature) : World(world), Creature(creature) { }
    virtual bool IsRunnable() const = 0;
};
struct CIsRunnableModuleCarryingProjectileWeapon : CIsRunnableModule {
    CIsRunnableModuleCarryingProjectileWeapon(CWorld* world, CThing* creature) : CIsRunnableModule(world, creature) { }
    virtual bool IsRunnable() const;
};
}
struct CWorld;
bool CAIStateGroup_CreatureTakeCover::IsRunnableLeaf()
{
    if (NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon(GetPWorld(), (CThing*)PBrain->Creature).IsRunnable()
        && IS_THING_ALIVE(PTargetThing)
        && !IsInMeleeWithCombatWheelAttackers(PTargetThing.GetPItem())
        )
    {
        CTCCarrying* carrying;
        if (PTargetThing.GetPItem()->GetInterface(0x46, &carrying)) {
            CThing* weapon = carrying->GetThingInPrimarySlot();
            if (weapon) {
                CTCWeapon* tcWeapon;
                if (weapon->GetInterface(0x48, &tcWeapon) && tcWeapon->IsProjectileWeapon()
                    && IsCoverBaseRunnable())
                    return true;
            }
        }
    }
    return false;
}

struct CWorld { bool runnable; };
struct CTCBase {};
struct CarryFixture : CTCCarrying {};
struct WeaponFixture : CTCWeapon {};
static bool g_melee = false, g_cover_base = true, g_projectile = true;
static CThing* g_primary;
static CWorld* g_world;
static CThing* g_expected_creature;
static CAIStateGroup_CreatureTakeCover* g_expected_state;
static CarryFixture g_carry;
static WeaponFixture g_weapon_component;
static int g_melee_calls, g_primary_calls, g_weapon_calls, g_base_calls, g_world_calls;
static int g_lower_calls, g_getitem_calls, g_module_calls, g_order[32], g_order_n, g_bad_receiver;
static void event(int e) { if (g_order_n < 32) g_order[g_order_n++] = e; }
void CAIStateGroup_CreatureTakeCover::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { ++g_getitem_calls; event(2); return m_pItem; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* key) const {
    ++g_lower_calls; event((*key == 0x46) ? 4 : 6);
    SEntry* p = first; while (p != last && p->type < *key) ++p; return p;
}
CThing* CTCCarrying::GetThingInPrimarySlot() { ++g_primary_calls; event(5); if (this != &g_carry) g_bad_receiver=1; return g_primary; }
bool CTCWeapon::IsProjectileWeapon() { ++g_weapon_calls; event(7); if (this != &g_weapon_component) g_bad_receiver=1; return g_projectile; }
CWorld* CAIStateGroup_CreatureTakeCover::GetPWorld() { ++g_world_calls; if (this != g_expected_state) g_bad_receiver=1; return g_world; }
bool CAIStateGroup_CreatureTakeCover::IsCoverBaseRunnable() { ++g_base_calls; event(8); if (this != g_expected_state) g_bad_receiver=1; return g_cover_base; }
bool __fastcall IsInMeleeWithCombatWheelAttackers(CThing* c) { ++g_melee_calls; event(3); if (c != g_expected_creature) g_bad_receiver=1; return g_melee; }
bool NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon::IsRunnable() const {
    ++g_module_calls; event(1);
    if (World != g_world || Creature != g_expected_creature) g_bad_receiver=1;
    return World->runnable;
}
static void init_thing(CThing& t) { for (int i=0;i<9;++i) t.m_presentBits[i]=0; t.m_interfaces.first=0; t.m_interfaces.last=0; t.m_flags91=0; }
static int run_case(bool module, bool target_set, bool dead, bool melee, bool carrying, bool has_weapon, bool weapon_iface, bool projectile, bool coverbase, bool expected)
{
    CThing target, weapon; init_thing(target); init_thing(weapon);
    g_primary = has_weapon ? &weapon : 0; g_projectile=projectile;
    CInterfaceMap::SEntry target_entries[1], weapon_entries[1];
    target_entries[0].type=0x46; target_entries[0].component=(CTCBase*)&g_carry;
    weapon_entries[0].type=0x48; weapon_entries[0].component=(CTCBase*)&g_weapon_component;
    target.m_presentBits[2]=carrying ? (1UL<<6) : 0;
    weapon.m_presentBits[2]=weapon_iface ? (1UL<<8) : 0;
    target.m_interfaces.first=target_entries; target.m_interfaces.last=target_entries+1;
    weapon.m_interfaces.first=weapon_entries; weapon.m_interfaces.last=weapon_entries+1;
    if (dead) target.m_flags91=1;
    CWorld world; world.runnable=module; g_world=&world; g_expected_creature=&target;
    CAIBrain brain; brain.Creature=(CThingAICreature*)&target;
    CAIStateGroup_CreatureTakeCover state;
    g_expected_state=&state;
    state.PBrain=&brain; state.PTargetThing.m_pItem=target_set ? &target : 0; state.PCoverThing.m_pItem=&weapon;
    g_melee=melee; g_cover_base=coverbase;
    g_melee_calls=g_primary_calls=g_weapon_calls=g_base_calls=g_world_calls=g_lower_calls=g_getitem_calls=g_module_calls=g_order_n=g_bad_receiver=0;
    bool got=state.IsRunnableLeaf();
    if (got != expected || g_bad_receiver || g_world_calls!=1 || g_module_calls!=1) return 0;
    int sequence[11]={1,2,2,2,3,2,4,5,6,7,8};
    int n=!module?1:!target_set?2:dead?3:melee?5:!carrying?6:!has_weapon?8:!weapon_iface?8:!projectile?10:11;
    if(g_order_n!=n) return 0;
    for(int k=0;k<n;++k) if(g_order[k]!=sequence[k]) return 0;
    if (!module && (g_getitem_calls || g_melee_calls || g_lower_calls || g_primary_calls || g_weapon_calls || g_base_calls)) return 0;
    if (!target_set && g_melee_calls) return 0;
    if (dead && g_melee_calls) return 0;
    if (melee && (g_lower_calls || g_primary_calls || g_weapon_calls || g_base_calls)) return 0;
    if (!carrying && (g_primary_calls || g_weapon_calls || g_base_calls)) return 0;
    if (!has_weapon && (g_weapon_calls || g_base_calls)) return 0;
    if (!weapon_iface && (g_weapon_calls || g_base_calls)) return 0;
    if (g_melee_calls && !(g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 2 && g_order[4] == 3)) return 0;
    return 1;
}
int main()
{
    if (!run_case(true,true,false,false,true,true,true,true,true,true) ||
        !run_case(false,true,false,false,true,true,true,true,true,false) ||
        !run_case(true,false,false,false,true,true,true,true,true,false) ||
        !run_case(true,true,true,false,true,true,true,true,true,false) ||
        !run_case(true,true,false,true,true,true,true,true,true,false) ||
        !run_case(true,true,false,false,false,true,true,true,true,false) ||
        !run_case(true,true,false,false,true,false,true,true,true,false) ||
        !run_case(true,true,false,false,true,true,false,true,true,false) ||
        !run_case(true,true,false,false,true,true,true,false,true,false) ||
        !run_case(true,true,false,false,true,true,true,true,false,false)) {
        std::printf("00979d80_TEST FAIL\n"); return 1;
    }
    std::printf("00979d80_TEST PASS\n"); return 0;
}