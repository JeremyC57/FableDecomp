// CTCBattleCharge::FinishMove(CThingCreatureBase* creature, bool hitSomething) -- end the charge: clean up the effects, start the pose delay, re-equip the item taken out for the charge (fast unsheathe to the combat pose) and, after a hit, start the battle-charge impact action with the two feet effects
// Leaf: FinishMove
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A7BF0, 278 bytes (+10 int3 padding), `ret 8`.
// Callees: CTCBattleCharge::CleanUpEffects 0x007A74A0, CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), CCharString(const char*,
//   long) 0x0099EBF0, ~CCharString 0x0099EAE0, CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase*, const
//   CCharString&, CThing*, long, long, CCreatureActionBase*) 0x0069F0F0 and its out-of-line destructor 0x005CC091
//   (manifest label propagated), CCreatureAction_BattleChargeImpact(CThingCreatureBase*, CIntelligentPointer<CThing>,
//   CIntelligentPointer<CThing>) 0x008CE4B0, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase
//   0x0062E900 (manifest "~CTCLightningOrb" is propagated). Literal 0x0126CCE4.
// Layout: as the landed-chunk Reset 0x007A6F00: PItemRemoved +0x44, LeftFeetEffect +0x58, RightFeetEffect +0x60,
//   StartPoseDelay +0x6C, HitSomething +0x6D; retail CCombatActionBase is 0x114 bytes, so the impact action (0x124)
//   holds its two feet pointers at +0x114 / +0x11C.
// Shape: both actions are temporaries passed by address straight from their constructors (sharing one frame area);
//   the impact action takes its intelligent pointers BY VALUE (each copy built in the argument area through the inline
//   copy constructor: base ctor, vtable store, SetPItem(other.GetPItem())), and its implicit destructor runs the two
//   member destructors then the combat-action base destructor.
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                 // +0x91 (bit 0: dead)
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_stateFlags & 1))
            return true;
    }
    return false;
}
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
};
class CThingCreatureBase;
class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:
    unsigned char _body[0x10];
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase* owner, const CCharString& animation, CThing* item,
                                               long priority, long timeout, CCreatureActionBase* next);   // 0x0069F0F0
    ~CCreatureAction_UnsheatheItemFromInventory();   // 0x005CC091
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _pad0a8[0x114 - 0xa8];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_BattleChargeImpact : public CCombatActionBase { public:
    CIntelligentPointer<CThing> PLeftFeetEffect;    // +0x114
    CIntelligentPointer<CThing> PRightFeetEffect;   // +0x11c
    CCreatureAction_BattleChargeImpact(CThingCreatureBase* owner, CIntelligentPointer<CThing> leftFeetEffect,
                                       CIntelligentPointer<CThing> rightFeetEffect);   // 0x008CE4B0
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};

class CTCBattleCharge {
public:
    void*         __vftable;                    // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x44 - 0x08];
    CIntelligentPointer<CThing> PItemRemoved;   // +0x44
    float         RadiusOfEffect;               // +0x4c
    CIntelligentPointer<CThing> ShieldEffect;   // +0x50
    CIntelligentPointer<CThing> LeftFeetEffect; // +0x58
    CIntelligentPointer<CThing> RightFeetEffect;    // +0x60
    long          PoseDelay;                    // +0x68
    bool          StartPoseDelay;               // +0x6c
    bool          HitSomething;                 // +0x6d
    void CleanUpEffects();                      // 0x007A74A0
    void FinishMove(CThingCreatureBase* creature, bool hitSomething);
};

void CTCBattleCharge::FinishMove(CThingCreatureBase* creature, bool hitSomething)
{
    CleanUpEffects();
    StartPoseDelay = true;
    HitSomething = hitSomething;
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString animation("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE", -1);
        creature->SetCurrentAction(&CCreatureAction_UnsheatheItemFromInventory(creature, animation, PItemRemoved.GetPItem(), 100, 250, 0));
    }
    if (hitSomething)
        creature->SetCurrentAction(&CCreatureAction_BattleChargeImpact(creature, LeftFeetEffect, RightFeetEffect));
}