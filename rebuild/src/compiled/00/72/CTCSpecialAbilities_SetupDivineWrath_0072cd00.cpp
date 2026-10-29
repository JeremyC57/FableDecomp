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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    __forceinline CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThingCreatureBase;
class CCreatureActionBase { public:
    virtual void v00();
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _pad04[0x2c - 0x04];
    CIntelligentPointer<CThing> PItem;                     // +0x2c (destroyed by the base destructor)
    virtual ~CCombatActionBase();                               // 0x0062E900
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