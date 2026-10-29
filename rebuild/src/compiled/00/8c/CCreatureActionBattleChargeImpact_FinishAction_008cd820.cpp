// CCreatureAction_BattleChargeImpact::FinishAction() -- base FinishAction; release the creature's special-ability lock when the battle charge holds it (0xA7 CTCSpecialAbilities with CurrentAbility 12: AbilityMutexLock and CurrentAbility cleared), re-attach both feet effects (Left / Right particle emitters, interface 0x4B) to the creature, and re-equip the item taken out for the charge (fast unsheathe to the combat pose) when it still lives
// Leaf: ?FinishAction@CCreatureAction_BattleChargeImpact
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CD820, 527 bytes, virtual FinishAction (`ret`), frame 0xC0: this is spilled to
//   [esp+10h] at entry, ebx is never used, the 0x4B key is an immediate in both feet lookups.
// Callees: CCombatActionBase::FinishAction 0x00859A20, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap
//   LowerBound 0x0040F020 (interfaces 0xA7, 0x4B), CCharString(const char*, long = -1) 0x0099EBF0, ~CCharString
//   0x0099EAE0, CTCDParticleEmitter::AttachToThing 0x006E0BE0, CCreatureAction_UnsheatheItemFromInventory ctor 0x0069F0F0
//   and its out-of-line destructor 0x005CC091, CThingCreatureBase::SetCurrentAction 0x006644F0.
// Shape: the BattleChargeMiss sibling's source (0x008CDA30) plus the `CurrentAbility == 12` guard. The allocation levers
//   are the sibling's: the dead test through an inline `IsDead()`, the action's owner and target through the
//   CIntelligentPointer accessors (`PCreature->`, `*PCreature`), and the ternary GetInterface out-store. With a raw
//   `!(flags & 1)` test, direct GetPItem calls and an if-assign store VC7.1 pushes ebx, keeps this in ebp and &PCreature
//   in ebx (521 bytes) instead of spilling this to [esp+10h] at entry.
// Layout: local model. engine/CCreatureAction_BattleChargeImpact.h is the Ego_r layout (0x13C, Left +0x12C); TLC's
//   CCombatActionBase is 0x114 bytes (PCreature +0x08, PItemRemoved +0x2C), so Left / Right sit at +0x114 / +0x11C.
//   CThing component bits +0x20, interface map +0x44, dead flag +0x91 bit 0; CTCSpecialAbilities AbilityMutexLock +0x0C,
//   CurrentAbility +0x10.
class CTCBase;
class CThing;
class CBaseObject;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);             // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase;
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
};
class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:   // retail sizeof 0xb8
    unsigned char _body[0x10];
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& owner, const CCharString& animation, CThing& item,
                                               long priority, long timeout, CCreatureActionBase* next);   // 0x0069F0F0
    ~CCreatureAction_UnsheatheItemFromInventory();    // 0x005CC091
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                 // +0x20 component bitset
    CInterfaceMap Components;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                            // +0x91 bit 0: dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PData;                               // +0x04
    CBaseObject* GetPItem() const;                    // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};
template<class T> inline bool IS_THING_ALIVE(const T& p)
{
    if (p.GetPItem() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CTCSpecialAbilities { public:                          // interface 0xA7
    unsigned char _pad00[0x0c];
    bool          AbilityMutexLock;                   // +0x0c
    long          CurrentAbility;                     // +0x10 EHeroAbility
};
class CTCDParticleEmitter { public:                          // interface 0x4B
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};

class CCombatActionBase { public:                            // retail sizeof 0x114 (CCreatureActionBase prefix)
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;  // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointer<CThing> PItemRemoved;         // +0x2c
    unsigned char _pad34[0x114 - 0x34];
    void FinishAction();                              // 0x00859A20
};
class CCreatureAction_BattleChargeImpact : public CCombatActionBase { public:
    CIntelligentPointer<CThing> Left;                 // +0x114 left feet effect
    CIntelligentPointer<CThing> Right;                // +0x11c right feet effect
    void FinishAction();
};

void CCreatureAction_BattleChargeImpact::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCSpecialAbilities* abilities;
    if (PCreature->GetInterface(&abilities, 0xa7) && abilities->CurrentAbility == 12) {
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
    CTCDParticleEmitter* emitter;
    if (IS_THING_ALIVE(Left)) {
        Left->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*PCreature, 1, "", 0, 0.0f);
    }
    if (IS_THING_ALIVE(Right)) {
        Right->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*PCreature, 1, "", 0, 0.0f);
    }
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString animation("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        PCreature->SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory(*PCreature, animation, *PItemRemoved, 100, 250, 0));
    }
}