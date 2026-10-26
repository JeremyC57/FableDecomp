// CAIStateGroup_TrollBase::ThrowRocks() -- post a CCreatureAction_TrollThrowProjectile at the enemy through a brain
// output (flag 8), yield once, and count a shot when the creature is now performing a different action. Returns 2
// (thrown) or 4 (still going).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094DC40, length 255 bytes.
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0 (x2), CCreatureActionBase::GetActionID @ 0x00692620
// (x2, the creature's +0x100 current action), CBrainOutput::CBrainOutput @ 0x00835620, CBaseIntelligentPointer::
// GetPItem @ 0x00A01B50 (PEnemy), CCreatureAction_TrollThrowProjectile(CThingCreatureBase*, CThing*) @ 0x008C7400,
// the action's virtual slot 11 (+0x2c, a counted clone), CCountedPointer::Reset @ 0x00668D90, ~CBaseIntelligentPointer
// @ 0x00A01C10 (the action's projectile pointer, the output's target), ~CCombatActionBase @ 0x0062E900,
// CAIBrain::SetBrainOutput @ 0x0088B8F0, the brain's virtual slot 2 (+0x08, yield), operator delete @ 0x00BFE9BC.
// Shape: the landed AIAction_CombatPerformActionAndWait (0x0097DA40) brain-output model inlined here: the output is
// a function-scope local (its implicit destructor runs on return), the action a temporary passed by pointer to an
// inline SetPerformAction (so Clone is a virtual call on the constructor's returned this; implicit destructor:
// projectile pointer, then the combat-action base); the creature is a named local read before PEnemy.
class CThing;
class CThingCreatureBase;
class C3DVector { public: float X, Y, Z; };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};

struct CCountBlock {                                // CCountedPointer's shared block
    long Count;                                     // +0x00
    void (__fastcall* Destroy)(void* object);       // +0x04
    void* Object;                                   // +0x08
};
struct CCountedPointer {                            // CCountedPointer<..> {object, block}
    void* Object;                                   // +0x00
    CCountBlock* Block;                             // +0x04
    void Reset(void* object);                       // 0x00668D90
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:                               // 0x48 bytes
    unsigned long Flags;                            // +0x00 (8: perform action)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                        // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;                // +0x40 CIntelligentPointer<const CThing>
    CBrainOutput();                                 // 0x00835620
};
class CCombatActionBase;
// post an action in a brain output (flag 8) -- the action arrives by pointer, so its clone is a real virtual call
inline void SetPerformAction(CBrainOutput& output, CCombatActionBase* action);

class CCombatActionBase { public:                          // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                    // slot 11 (+0x2c, a counted copy of itself)
    unsigned char _body[0x114 - 4];
    ~CCombatActionBase();                           // 0x0062E900
};
struct CIntelligentPointerProjectile : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
};
class CCreatureAction_TrollThrowProjectile : public CCombatActionBase { public:  // retail sizeof 0x130
    CIntelligentPointerProjectile PProjectile;      // +0x114
    C3DVector TargetPos;                            // +0x11c
    bool TargetIsPosition;                          // +0x128
    long ThrowFrame;                                // +0x12c
    CCreatureAction_TrollThrowProjectile(CThingCreatureBase* owner, CThing* thing);   // 0x008C7400
};

inline void SetPerformAction(CBrainOutput& output, CCombatActionBase* action)
{
    output.Flags |= 8;
    output.PAction.Reset(action->Clone());
}

class CCreatureActionBase { public:
    long GetActionID() const;                       // 0x00692620
};
class CThingCreatureBase { public:
    unsigned char _pad[0x100];
    CCreatureActionBase* CurrentAction;             // +0x100
    bool IsPerformingAnyAction();                   // 0x006622A0
};

class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                           // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThingCreatureBase* Creature;                   // +0x18
    void SetBrainOutput(const CBrainOutput& output);  // 0x0088B8F0
};

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
};
struct CAIStateGroup_TrollBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;                 // +0x14 CIntelligentPointer<CThingCreatureBase>
    long EnemyKnockDownFrame;                       // +0x1c
    long ShotCounter;                               // +0x20
    long ThrowRocks();
};

long CAIStateGroup_TrollBase::ThrowRocks()
{
    long result = 4;
    long previousAction = 0;
    if (PBrain->Creature->IsPerformingAnyAction())
        previousAction = PBrain->Creature->CurrentAction->GetActionID();
    CBrainOutput output;
    CThingCreatureBase* creature = PBrain->Creature;
    SetPerformAction(output, &CCreatureAction_TrollThrowProjectile(creature, PEnemy.GetPItem()));
    PBrain->SetBrainOutput(output);
    PBrain->Yield();
    if (PBrain->Creature->IsPerformingAnyAction() && PBrain->Creature->CurrentAction->GetActionID() != previousAction) {
        ++ShotCounter;
        result = 2;
    }
    return result;
}