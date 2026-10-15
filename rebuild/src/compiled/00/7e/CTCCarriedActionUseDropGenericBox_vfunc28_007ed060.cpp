// CTCCarriedActionUseDropGenericBox::vfunc_28(CThingAICreature* user) -- runs the base carried-action hook, then, when the creature's flag 8 (+0x6c) is set, makes it perform an alert-to-player action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED060, length 63 bytes. CTCCarriedActionUseDropGenericBox vtable 0x0123FA2C slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4`, another TU), CCreatureAction_AlertToPlayer::CCreatureAction_AlertToPlayer
// (CThingAICreature*) @ 0x00842150 (manifest), CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0
// (manifest), the action's destructor @ 0x00693EF0 (the manifest's "~CPhysicsMeshInfo" label is propagated).
// Shape: the 0xA8-byte action is a temporary built in the frame and passed by address straight from the
// constructor's returned `this` (`push eax`), the GOTCHAS `PerformActionAndWait(&CAction(creature))`
// form; it is destroyed right after the call. No EH frame: the TU is built without /EHsc.
struct CThingAICreature;

struct CCreatureActionBase {
    unsigned char _body[0xa8];
    ~CCreatureActionBase();                                     // 0x00693EF0
};

struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingAICreature* creature);  // 0x00842150
};

struct CThingAICreature {
    unsigned char _pad[0x6c];
    unsigned char m_flags6c;                                    // +0x6c
    bool SetCurrentAction(CCreatureActionBase* action);         // 0x006644F0
};

struct CTCCarriedActionUseBase {
    virtual void v00();
    void OnUseBase(CThingAICreature* user);                     // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUseDropGenericBox : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user);
};

void CTCCarriedActionUseDropGenericBox::vfunc_28(CThingAICreature* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    if (user->m_flags6c & 8)
        user->SetCurrentAction(&CCreatureAction_AlertToPlayer(user));
}