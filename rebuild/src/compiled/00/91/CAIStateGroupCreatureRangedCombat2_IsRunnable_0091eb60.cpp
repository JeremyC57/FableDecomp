// CAIStateGroup_CreatureRangedCombat2::IsRunnable() -- the creature carries a projectile weapon and its attack component names a best target: remember it and require IsAbleToFire
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091EB60, length 198 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0, NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon::IsRunnable
// (slot 0 of vtable 0x0128FFD4 = 0x00982890, called through the vtable of the local), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x004365B0 (type 8), component slot 29 (+0x74), IsBestTargetForAttack(CThing*, CThing*) @ 0x00864840
// (__fastcall), CIntelligentPointer<const CThing>::CIntelligentPointer(const CThing*) @ 0x00668B60 (emitted out of line in
// this TU), CBaseIntelligentPointer GetPItem/SetPItem/dtor @ 0x00A01B50/0x00A01B90/0x00A01C10, IsAbleToFire @ 0x0091EA70.
// Shape: the 12-byte module local {vptr, world, creature}; the creature is read before the GetPWorld call (arguments
// right to left) and the member stores precede the vtable store (novtable base ctor). The module is a temporary (a named local
// gets its own slot and a direct call); the intelligent pointer is a named local in its own block (a temporary reuses the
// out-of-line ctor's eax) and shares the module's slot. Layout: PTargetThing +0x14 (Ego_r.pdb CAIStateGroup_CreatureRangedCombat2).
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

class CAIBrain
{ public:
    unsigned char _pad0[0x18];
    CThing* Creature;                               // +0x18
};

class CWorld;

namespace NCreatureAI
{
    struct __declspec(novtable) CIsRunnableModule  // members set by the novtable base ctor, before the derived vtable store
    {
        CWorld* World;                              // +0x04
        CThing* Creature;                           // +0x08
        CIsRunnableModule(CWorld* world, CThing* creature) : World(world), Creature(creature) { }
        virtual bool IsRunnable() const = 0;
    };
    struct CIsRunnableModuleCarryingProjectileWeapon : CIsRunnableModule   // vtable 0x0128FFD4
    {
        CIsRunnableModuleCarryingProjectileWeapon(CWorld* world, CThing* creature) : CIsRunnableModule(world, creature) { }
        virtual bool IsRunnable() const;            // slot 0: 0x00982890 (called through the known vtable: `call [0x0128FFD4]`)
    };
}

struct CTCAttackTarget                              // interface type 8 component
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetTarget();                    // slot 29 (+0x74)
};

bool __fastcall IsBestTargetForAttack(CThing* creature, CThing* target);   // 0x00864840

struct CIntelligentPointerConstThing : CBaseIntelligentPointer    // CIntelligentPointer<const CThing>, vtable 0x0123E7D8
{
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item);    // 0x00668B60 (out of line here)
};

struct CAIStateGroup_CreatureRangedCombat2
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14 CIntelligentPointer<CThingCreatureBase>
    CWorld* GetPWorld();                            // 0x008FCFC0
    bool IsAbleToFire();                            // 0x0091EA70
    bool IsRunnable();
};

bool CAIStateGroup_CreatureRangedCombat2::IsRunnable()
{
    if (NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon(GetPWorld(), PBrain->Creature).IsRunnable()
        && PBrain->Creature->HasTC(8))
    {
        CThing* target = ((CTCAttackTarget*)PBrain->Creature->FindEntry(8)->second)->GetTarget();
        if (target && IsBestTargetForAttack(PBrain->Creature, target))
        {
            {
                CIntelligentPointerConstThing item(target);
                PTargetThing.SetPItem(((CThing*)item.GetPItem()));
            }
            if (IsAbleToFire())
                return true;
        }
    }
    return false;
}