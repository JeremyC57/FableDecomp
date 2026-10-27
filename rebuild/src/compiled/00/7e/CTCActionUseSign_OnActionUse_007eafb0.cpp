// CTCActionUseSign::OnActionUse(CThing& user) -- the base use handling, then, if the user is a live creature, set it doing a CCreatureAction_Talk on this component's thing
// Leaf: ?OnActionUse@CTCActionUseSign@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007eafb0, 90 bytes, thiscall, `ret 4`.
// Identity (proven): vtable 0x0123F1AC (RTTI .?AVCTCActionUseSign@@) slot 28 = Ego_r.pdb ?OnActionUse@CTCActionUseSign@@UAEXAAVCThing@@@Z; it builds a CCreatureAction_Talk (constructor 0x0062E020 storing vtable 0x012599E4, Ego_r.pdb ??0CCreatureAction_Talk@@QAE@AAVCThingCreatureBase@@AAVCThing@@Z, PTalkToThing at +0xA8) for the user.
// Callees: CTCActionUseBase::OnActionUse 0x007EAB90, CCreatureAction_Talk::CCreatureAction_Talk 0x0062E020, CThingCreatureBase::SetCurrentAction(const
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
class CCreatureAction_Talk : public CCreatureActionBase {
public:
    CCreatureAction_Talk(CThingCreatureBase& creature, CThing& thing);  // 0x0062E020
    CIntelligentPointer<CThing> PTalkToThing;                  // +0xA8
};
class CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);               // 0x007EAB90
    CThing* ParentThing;                                  // +0x04
    CThing* GetParentThing() const { return ParentThing; }   // inline accessor (spelling unproven: emits no symbol)
};
class CTCActionUseSign : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};
void CTCActionUseSign::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThingCreatureBase* creature = (CThingCreatureBase*)&user;
    if (creature && !creature->IsDead())
        creature->SetCurrentAction(CCreatureAction_Talk(*creature, *ParentThing));
}