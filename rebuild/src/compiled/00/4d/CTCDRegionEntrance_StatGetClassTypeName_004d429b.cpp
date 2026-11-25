// CTCDRegionEntrance::StatGetClassTypeName() -- the class's type name as a CCharString, "CTCDRegionEntrance" (static)
// Leaf: StatGetClassTypeName@CTCDRegionEntrance
// Retail: Fable.exe @ 0x004D429B, 19 bytes. Catalog label NTCScriptedControl::CActionDoCreatureAction::GetActionName is wrong.
// Evidence: builds CCharString("CTCDRegionEntrance", -1) (string @ 0x0123BA5C) into the result; called by its GetClassTypeName 0x004D42D6 and Init_RegisterGameComponents 0x004F28C3;
// Ego_r.pdb/FableWin.pdb carry ?StatGetClassTypeName@CTCDRegionEntrance@@SI?AVCCharString@@XZ (public static __fastcall), so the hidden result
// pointer arrives in ecx and is returned in eax.
// Callee: CCharString::CCharString(const char*, long) @ 0x0099EBF0.
// Form (NF407, 2026-11-25): the static is defined in the class body, so it is a pick-any COMDAT, and a scaffolding helper takes its
//   address so this object emits it (the nf86 / nf88 form, landing nf387 ruling; the nf406_b2 0x004D31C0 form). The landed binder
//   CTCBase::RegisterTCs 0x004EE23F defines the same static in the class body (TC_REGISTRATION_INTERFACE, pick-any), and retail's own
//   definition is an in-class inline (RegisterTCs expands it at its first 41 registrations and calls the out-of-line copy after that).
//   The previous out-of-line (NoDuplicates) definition clashed with the binder's pick-any copy at link (LNK4006, landing nf382 ruling).
class CCharStringData;
class CCharString {                                     // 4-byte handle (Ego_r.pdb); user copy ctor/dtor: returned through a hidden pointer
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
class CTCDRegionEntrance
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCDRegionEntrance"); }
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D429B is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCDRegionEntrance::StatGetClassTypeName;
}
}
