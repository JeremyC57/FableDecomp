// CTCActionUseKickChicken::OnActionUse(CThing& user) -- the user (a creature) is handed a CCombatAction_KickThingOnGround(user, this component's thing) temporary as its current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ECE20, length 55 bytes.
// Leaf: OnActionUse
// Callees: CCombatAction_KickThingOnGround::CCombatAction_KickThingOnGround(CThingCreatureBase*, CThing*) @ 0x0087FD20,
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, the action's destructor @ 0x0062E900 (the
// manifest's "~CTCLightningOrb" is propagated). Slot 28 is CTCActionUseBase::OnActionUse(CThing&) (Ego_r.pdb
// `?OnActionUse@CTCActionUseKickChicken@@UAEXAAVCThing@@@Z`); the chunk's "vfunc_28" is that slot.
// Shape: the 0x130-byte action is a temporary built in the call (`&CAction(..)` reuses the constructor's returned eax as
// the pushed argument) and destroyed after it. The owner thing is read through an inline CTCBase accessor: a direct
// `ParentThing` read is allocated to eax, the accessor lands it in ecx (`mov ecx,[ecx+4]`) as retail has it.
// engine/CThingCreatureBase.h / CCombatAction_KickThingOnGround.h / CCreatureActionBase.h are flattened PDB PODs; the
// calls need member declarations, so the classes are modelled locally.
class CThing { public: unsigned char _body[0x10]; };

class CCreatureActionBase { public: virtual void v00(); };

class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CCombatAction_KickThingOnGround : public CCreatureActionBase { public:
    unsigned char _body[0x12c];                // a 0x130-byte action
    CCombatAction_KickThingOnGround(CThingCreatureBase* creature, CThing* thing);   // 0x0087FD20
    ~CCombatAction_KickThingOnGround();        // 0x0062E900
};

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CThing* GetThing() const { return ParentThing; }
};

class CTCActionUseBase : public CTCBase { public:
    unsigned char _pad0c[8];                   // +0x0c PActionUseDef, +0x10 Usable
    virtual void OnActionUse(CThing& user);    // slot 28
};

struct CTCActionUseKickChicken : CTCActionUseBase {
    virtual void OnActionUse(CThing& user);
};

void CTCActionUseKickChicken::OnActionUse(CThing& user)
{
    CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(user);
    creature.SetCurrentAction(CCombatAction_KickThingOnGround(&creature, GetThing()));
}