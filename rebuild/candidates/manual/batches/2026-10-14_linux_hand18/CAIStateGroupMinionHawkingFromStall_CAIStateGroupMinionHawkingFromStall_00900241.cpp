// CAIStateGroup_MinionHawkingFromStall::CAIStateGroup_MinionHawkingFromStall() -- base CAIStateGroupBase(), NextHawkFrame = 0, own vtable, the Customer intelligent pointer built inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00900241, length 40 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255, CBaseIntelligentPointer::
// CBaseIntelligentPointer @ 0x00A01B10 (the member's base). Vtables: 0x0128BF48 =
// CAIStateGroup_MinionHawkingFromStall, 0x0123E7D8 = CIntelligentPointer (rtti-vtables.tsv). The
// manifest's "~CCombatAbilityBlockDefBase" label is propagated; this is a constructor.
// Layout = Ego_r.pdb: CAIStateGroupBase is 0x14 bytes (engine/CAIStateGroupBase.h, a flat POD ->
// `_Calls` model), NextHawkFrame +0x14, Customer (CIntelligentPointer<CThingCreatureBase>) +0x18;
// size 0x20. `and dword ptr [esi+14h],0` marks the size-optimised TU; the long's zero store is
// scheduled above the derived vtable store, the member's constructor (out-of-line base call, then
// its vtable) below it.
#pragma optimize("s", on)
struct CAIStateGroupBase_Calls {
    virtual void v00();
    unsigned char _body[0x10];                              // PBrain, ScriptAIPriority, Name, flags
    CAIStateGroupBase_Calls();                              // 0x008FD255
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* PData;                                            // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase_Calls {
    long                NextHawkFrame;                      // +0x14
    CIntelligentPointer Customer;                           // +0x18
    virtual void v00();                                     // own vtable 0x0128BF48
    CAIStateGroup_MinionHawkingFromStall();
};

CAIStateGroup_MinionHawkingFromStall::CAIStateGroup_MinionHawkingFromStall()
    : CAIStateGroupBase_Calls()
    , NextHawkFrame(0)
    , Customer()
{
}
