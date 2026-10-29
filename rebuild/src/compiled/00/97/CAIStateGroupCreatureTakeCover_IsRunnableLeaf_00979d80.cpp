// CAIStateGroup_CreatureTakeCover::IsRunnableLeaf() -- gate on the projectile-carrying module, then require a live target outside melee, a projectile weapon, and the cover-base leaf check.
// Retail: Fable.exe (TLC, VC7.1), @0x00979D80, 258 bytes.
// Callees: CAIStateGroupBase::GetPWorld @0x008FCFC0; NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon::IsRunnable @0x00982890 (temporary vtable 0x0128FFD4); CBaseIntelligentPointer::GetPItem @0x00A01B50; IsInMeleeWithCombatWheelAttackers @0x00979910; CVectorMap::LowerBound @0x004365B0; CTCCarrying::GetThingInPrimarySlot @0x006A1440; CTCWeapon::IsProjectileWeapon @0x005DAA40; qualified base IsRunnableLeaf @0x00970F30.
// The PDB header only exposes a flattened 0x50-byte base area. The fields at +0x14/+0x1f and inherited call behavior follow the landed CreatureTakeCover/CreatureUseCoverBase relatives. GetInterface inlines the presence-bit test at +0x28, then searches the sorted component map.
class CThing;

class CBaseObject;
struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer   // inline ctor stores the template's vtable after the base ctor
{ public:
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    T* Get() const { return (T*)GetPItem(); }
};

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                  // +0x00
    SEntry* last;                                   // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;           // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing
{ public:
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
    __forceinline const CInterfaceMap::SEntry* FindEntry(long type)    // the entry; `->component` read at the call site
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        const CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->first <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

#include "engine/CAIBrain.h"

class CTCCarrying                                  // interface type 0x46 (word 2 bit 6)
{ public:
    CThing* GetThingInPrimarySlot();                // 0x006A1440
};

class CTCWeapon                                    // interface type 0x48 (word 2 bit 8)
{ public:
    bool IsProjectileWeapon();                      // 0x005DAA40
};

class CThingCreatureBase : public CThing
{ public:
    bool IsIncapacitated();                         // 0x00661E80 (flags byte +0x15c: bit 2, else bit 3; manifest "CText::GetAlignment" is propagated)
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0)
    {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
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
class CWorld;
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

class CWorld;
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
class CWorld;
bool CAIStateGroup_CreatureTakeCover::IsRunnableLeaf()
{
    if (NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon(GetPWorld(), (CThing*)PBrain->Creature).IsRunnable()
        && IS_THING_ALIVE(PTargetThing)
        && !IsInMeleeWithCombatWheelAttackers(((CThing*)PTargetThing.GetPItem()))
        )
    {
        CTCCarrying* carrying;
        if (((CThing*)PTargetThing.GetPItem())->GetInterface(0x46, &carrying)) {
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