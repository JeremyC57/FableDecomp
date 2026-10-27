// CTCActionUseMapTable::OnActionUse(CThing& user) -- the base use handling, then, if the user is a live creature, set it doing a CCreatureAction_PlayerInteractionViewQuests on this component's thing
// Leaf: ?OnActionUse@CTCActionUseMapTable@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007eb570, 90 bytes, thiscall, `ret 4`.
// Identity (proven): vtable 0x0123F454 (RTTI .?AVCTCActionUseMapTable@@) slot 28 = Ego_r.pdb ?OnActionUse@CTCActionUseMapTable@@UAEXAAVCThing@@@Z; it builds a CCreatureAction_PlayerInteractionViewQuests (constructor 0x007EB490 storing vtable 0x01270E7C, Ego_r.pdb ??0CCreatureAction_PlayerInteractionViewQuests@@QAE@AAVCThingCreatureBase@@PAVCThing@@Z, PTarget at +0xA8) for the user.
// Callees: CTCActionUseBase::OnActionUse 0x007EAB90, CCreatureAction_PlayerInteractionViewQuests::CCreatureAction_PlayerInteractionViewQuests 0x007EB490, CThingCreatureBase::SetCurrentAction(const
//   CCreatureActionBase&) 0x006644F0, ~CBaseIntelligentPointer 0x00A01C10, ~CCreatureActionBase 0x00693EF0.
// Shape (family L066): the action is a 0xB0-byte frame temporary; its implicit destructor runs the member's then the
//   base's out-of-line destructors after the call; the creature test is the null and Dead (+0x91 bit 0) check. The
//   parent thing read through an inline accessor reuses edi (this); the direct member read (CTCActionUseSign) uses eax.
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r.pdb: vptr + PData, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThing {
public:
    unsigned char _pad00[0x91];                           // (members not touched here)
    unsigned char Flags91;                                // +0x91 bit 0 = dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CCreatureActionBase {                                // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();                       // 0x00693EF0
    unsigned char Body[0xA4];                             // +0x04 (members not touched here)
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CCreatureAction_PlayerInteractionViewQuests : public CCreatureActionBase {
public:
    CCreatureAction_PlayerInteractionViewQuests(CThingCreatureBase& creature, CThing* thing);  // 0x007EB490
    CIntelligentPointer<CThing> PTarget;                  // +0xA8
};
class CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);               // 0x007EAB90
    CThing* ParentThing;                                  // +0x04
    CThing* GetParentThing() const { return ParentThing; }   // inline accessor (spelling unproven: emits no symbol)
};
class CTCActionUseMapTable : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};
void CTCActionUseMapTable::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThingCreatureBase* creature = (CThingCreatureBase*)&user;
    if (creature && !creature->IsDead())
        creature->SetCurrentAction(CCreatureAction_PlayerInteractionViewQuests(*creature, GetParentThing()));
}