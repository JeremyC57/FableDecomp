// NCreatureMode::CModeStunned::Recover() -- vtable slot 15: a stunned creature that is able to recover is given the recover-from-stunned combat action
// Leaf: ?Recover@CModeStunned@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DE650, 70 bytes (+10 int3 padding).
// Name: FableWin.pdb ?Recover@CModeStunned@NCreatureMode@@EAEXXZ; slot 15 of the vtable 0x01267C74 (rtti-vtables.tsv;
// slots 13/14 are the landed GetOutOfActionRecoveryFrames/GetOutOfActionTimeSeconds).
// Callees: CThingCreatureBase::IsAbleToRecoverFromStunned() const @ 0x00664C60,
// CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase*, bool) @ 0x00867AF0 (then the derived
// CCombatAction_RecoverFromStunned vtable 0x0127851C, rtti-vtables.tsv, from its inline constructor),
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Layout: CModeBase {vptr, creature +4}; the action is a 0x118-byte frame temporary (retail CCombatActionBase 0x114
// plus the CanBeHit/Flip bytes), as the landed CModeCreatureIsProjectile::Recover 0x008E08C0.
// Shape: the creature is read once into esi (a named local); the action is an unnamed temporary passed by
// reference, destroyed by the implicit derived destructor (= the out-of-line base destructor). Plain `ret`.
class CThingCreatureBase;

class CCreatureActionBase { public:
    virtual void v00();
};

class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _body[0x110];
    ~CCombatActionBase();                  // 0x0062E900
};

class CThingCreatureBase { public:
    bool IsAbleToRecoverFromStunned() const;                    // 0x00664C60
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase { public:
    bool CanBeHit;                         // +0x114
    bool Flip;                             // +0x115
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase* owner, bool flip);   // 0x00867AF0
};

class CCombatAction_RecoverFromStunned : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase { public:   // vtable 0x0127851C
    virtual void v00();
    CCombatAction_RecoverFromStunned(CThingCreatureBase* owner)
        : CCombatAction_GenericStrikeResponseKnockdownRecoverBase(owner, false)
    {
    }
};

namespace NCreatureMode {

struct CModeBase {                         // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThingCreatureBase* m_pThing;          // +0x04
};

struct CModeStunned : CModeBase {
    virtual void Recover();
};

void CModeStunned::Recover()
{
    CThingCreatureBase* creature = m_pThing;
    if (creature->IsAbleToRecoverFromStunned())
        creature->SetCurrentAction(CCombatAction_RecoverFromStunned(creature));
}

} // namespace NCreatureMode