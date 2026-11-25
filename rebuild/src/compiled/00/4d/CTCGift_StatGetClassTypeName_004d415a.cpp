// CTCGift::StatGetClassTypeName() -- static: the class-type name "CTCGift" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCGift@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D415A: push esi; push -1; push "CTCGift"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123b830 = "CTCGift" = the class name; PDB name set ?StatGetClassTypeName@CTCGift@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d415a::build).
// Form (NF408, 2026-11-25; NF407's mechanical re-form): the static is defined in the class body, so it is a pick-any COMDAT, and a
//   scaffolding helper takes its address so this object emits it (the nf86 / nf88 form, landing nf387 ruling; the nf406_b2 0x004D31C0
//   form). The landed binder CTCBase::RegisterTCs 0x004EE23F defines the same static in the class body (TC_REGISTRATION_INTERFACE,
//   pick-any), and retail's own definition is an in-class inline (RegisterTCs expands it at its first 41 registrations and calls the
//   out-of-line copy after that). The previous out-of-line (NoDuplicates) definition clashed with the binder's pick-any copy at link
//   (LNK4006, landing nf382 ruling).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCGift
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString((const char*)0x0123B830, -1); }   // "CTCGift"
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D415A is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCGift::StatGetClassTypeName;
}
}
