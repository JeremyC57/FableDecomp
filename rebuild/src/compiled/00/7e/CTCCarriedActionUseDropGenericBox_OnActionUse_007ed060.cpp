// CTCCarriedActionUseDropGenericBox::OnActionUse(CThing& user) [Ego_r.pdb; was vfunc_28(CThingAICreature*)] -- runs the base carried-action hook, then, when the creature's flag 8 (+0x6c) is set, makes it perform an alert-to-player action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED060, length 63 bytes. CTCCarriedActionUseDropGenericBox vtable 0x0123FA2C slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4`, another TU), CCreatureAction_AlertToPlayer::CCreatureAction_AlertToPlayer
// (CThingAICreature*) @ 0x00842150 (manifest), CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0
// (manifest), the action's destructor @ 0x00693EF0 (the manifest's "~CPhysicsMeshInfo" label is propagated).
// Shape: the 0xA8-byte action is a temporary built in the frame and passed by address straight from the
// constructor's returned `this` (`push eax`), the GOTCHAS `PerformActionAndWait(&CAction(creature))`
// form; it is destroyed right after the call. No EH frame: the TU is built without /EHsc.
class CThingAICreature;

class CCreatureActionBase { public:
    unsigned char _body[0xa8 - 4];
    virtual ~CCreatureActionBase();                                     // 0x00693EF0
};

namespace N_00842150 { struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingAICreature* creature);  // 0x00842150
}; } using N_00842150::CCreatureAction_AlertToPlayer;

class CThingAICreature { public:
    unsigned char _pad[0x6c];
    unsigned char m_flags6c;                                    // +0x6c
    bool SetCurrentAction(CCreatureActionBase* action);         // 0x006644F0
};

class CThing;
class CTCActionUseBase { public:                 // Ego_r.pdb base of CTCCarriedActionUseBase (row-local view: the one member called)
    virtual void OnActionUse(CThing& user);   // 0x007EAB90 Ego_r.pdb ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z (empty `ret 4`, RTTI slot 28; was OnUseBase(CThingAICreature*))
};
class CTCCarriedActionUseBase : public CTCActionUseBase { public:
    virtual void v00();
};

struct CTCCarriedActionUseDropGenericBox : CTCCarriedActionUseBase {
    virtual void OnActionUse(CThing& user);   // Ego_r.pdb (UAE; RTTI slot 28, was vfunc_28(CThingAICreature*))
};

void CTCCarriedActionUseDropGenericBox::OnActionUse(CThing& thing)
{
    CThingAICreature* user = (CThingAICreature*)&thing;
    CTCCarriedActionUseBase::OnActionUse(thing);
    if (user->m_flags6c & 8)
        user->SetCurrentAction(&CCreatureAction_AlertToPlayer(user));
}