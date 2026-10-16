// CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::IsRunnable() -- the creature carries a projectile weapon and its interface-8 component holds a specific target: copy it into PTargetThing
// Leaf: IsRunnable@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E260, length 199 bytes.
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0, NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon::IsRunnable
// (slot 0 of vtable 0x0128FFD4 = 0x00982890, called through the vtable of the temporary), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x004365B0 (type 8), CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/
// 0x00A01B90/0x00A01C10.
// Shape: the module is a temporary in the condition (landed 0x0091EB60 form); the component's target pointer is a
// CIntelligentPointer at the odd +0x1d (byte-packed component), read twice; the copy is an inline
// CIntelligentPointer<T>(item) temporary (vtable 0x01237F1C) assigned through SetPItem(GetPItem()).
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
    SEntry* LowerBound(const long* type);           // 0x004365B0
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

struct CAIBrain
{
    unsigned char _pad0[0x18];
    CThing* Creature;                               // +0x18
};

struct CWorld;

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

#pragma pack(push, 1)
struct CTCTargetHolder                              // interface type 8 component, its target link at +0x1d
{
    unsigned char _pad00[0x1d];
    CIntelligentPointer<CThing> PSpecificTarget;    // +0x1d
};
#pragma pack(pop)

struct CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14
    CWorld* GetPWorld();                            // 0x008FCFC0
    bool IsRunnable();
};

bool CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::IsRunnable()
{
    if (NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon(GetPWorld(), PBrain->Creature).IsRunnable()
        && PBrain->Creature->HasTC(8))
    {
        CTCTargetHolder* holder = (CTCTargetHolder*)PBrain->Creature->FindEntry(8)->component;
        if (holder->PSpecificTarget.GetPItem())
        {
            PTargetThing.SetPItem(CIntelligentPointer<CThing>(holder->PSpecificTarget.GetPItem()).GetPItem());
            return true;
        }
    }
    return false;
}
