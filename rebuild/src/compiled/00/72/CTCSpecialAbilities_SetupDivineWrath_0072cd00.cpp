// CTCSpecialAbilities::SetupDivineWrath() -- start the divine-wrath cast on the hero: with a live item in the primary slot, sheathe it first (a heap cast action follows the sheathe), otherwise cast straight away; returns whether the action was set
// Leaf: ?SetupDivineWrath
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072CD00, 286 bytes (plain `ret`). Ego_r.pdb `?SetupDivineWrath@CTCSpecialAbilities@@AAE_NXZ`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interface 0x46 = CTCCarrying), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440,
//   operator new @ 0x00BFEA1A (0x114 bytes), CCreatureAction_CastDivineWrathSpell(CThingCreatureBase*) @ 0x008BD110 (x2),
//   CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (x2) / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CCreatureAction_SheatheItemToInventory_ForDivineWrath(CThingCreatureBase*,
//   const CCharString&, CCreatureActionBase*, long) @ 0x0069F090 with g_CCharString_013bad30 @ 0x013BAD30, its destructor
//   @ 0x0072D3E0, CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0 (x2), ~CCombatActionBase @ 0x0062E900.
// Layout: CTCSpecialAbilities Creature +0x18; retail CThing interface bitset +0x20 (0x46 = byte +0x28 bit 6), map +0x44, dead
//   bit +0x91; the cast action's item pointer (CIntelligentPointer<CThing>) +0x2C (named descriptively).
// Shape: the base's inline intelligent-pointer member is destroyed by ~CCombatActionBase (the cast class has no destructor
//   of its own); the direct cast is an argument temporary of the const-reference SetCurrentAction (eax reused as the
//   address); the carrying component from the out-parameter GetInterface (uninitialised on a miss); the item pointer is set from
//   the implicit conversion temporary; the sheathe action and the direct cast action are locals of sibling blocks at one frame
//   address, each returning SetCurrentAction's result.
class CTCBase;
class CBaseObject;
class CCharString { public:
    void* m_pData;
};
extern CCharString g_CCharString_013bad30;              // 0x013BAD30
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot();                    // 0x006A1440
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
class CThingCreatureBase;
class CCreatureActionBase { public:
    virtual void v00();
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _pad04[0x2c - 0x04];
    CIntelligentPointerThing PItem;                     // +0x2c (destroyed by the base destructor)
    ~CCombatActionBase();                               // 0x0062E900
};
class CCreatureAction_CastDivineWrathSpell : public CCombatActionBase { public:   // vtable 0x0127E774, 0x114 bytes
    unsigned char _pad34[0x114 - 0x34];
    CCreatureAction_CastDivineWrathSpell(CThingCreatureBase* owner);   // 0x008BD110
};
class CCreatureAction_SheatheItemToInventory_ForDivineWrath : public CCreatureActionBase { public:   // vtable 0x0125CA7C
    unsigned char _pad04[0x114 - 0x04];
    CCreatureAction_SheatheItemToInventory_ForDivineWrath(CThingCreatureBase* owner, const CCharString& slot,
                                                          CCreatureActionBase* next, long flags);   // 0x0069F090
    ~CCreatureAction_SheatheItemToInventory_ForDivineWrath();   // 0x0072D3E0
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CTCSpecialAbilities { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                              // +0x0c
    bool UsingBed;                                      // +0x0d
    long CurrentAbility;                                // +0x10
    void* UndeadFollowers;                              // +0x14 list (retail 4 bytes)
    CThingCreatureBase* Creature;                       // +0x18
    bool SetupDivineWrath();
};

bool CTCSpecialAbilities::SetupDivineWrath()
{
    CTCCarrying* carrying;
    Creature->GetInterface(&carrying, 0x46);
    CThing* item = carrying->GetThingInPrimarySlot();
    if (item && !(item->m_flags91 & 1)) {
        CCreatureAction_CastDivineWrathSpell* cast = new CCreatureAction_CastDivineWrathSpell(Creature);
        cast->PItem = item;
        CCreatureAction_SheatheItemToInventory_ForDivineWrath sheathe(Creature, g_CCharString_013bad30, cast, 0);
        return Creature->SetCurrentAction(sheathe);
    }
    return Creature->SetCurrentAction(CCreatureAction_CastDivineWrathSpell(Creature));
}