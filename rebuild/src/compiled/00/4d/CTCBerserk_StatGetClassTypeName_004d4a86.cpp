// CTCBerserk::StatGetClassTypeName() -- static: the class-type name "CTCBerserk" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBerserk@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4A86: push esi; push -1; push "CTCBerserk"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c274 = "CTCBerserk" = the class name; PDB name set ?StatGetClassTypeName@CTCBerserk@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4a86::build).
// Form (NF407, 2026-11-25): the static is defined in the class body, so it is a pick-any COMDAT, and a scaffolding helper takes its
//   address so this object emits it (the nf86 / nf88 form, landing nf387 ruling; the nf406_b2 0x004D31C0 form). The landed binder
//   CTCBase::RegisterTCs 0x004EE23F defines the same static in the class body (TC_REGISTRATION_INTERFACE, pick-any), and retail's own
//   definition is an in-class inline (RegisterTCs expands it at its first 41 registrations and calls the out-of-line copy after that).
//   The previous out-of-line (NoDuplicates) definition clashed with the binder's pick-any copy at link (LNK4006, landing nf382 ruling).
#define CTCBerserk CTCBerserkLayout
#include "engine/CTCBerserk.h"
#undef CTCBerserk

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBerserk : public CTCBerserkLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString((const char*)0x0123C274, -1); }   // "CTCBerserk"
};

namespace {
// Fidelity: scaffolding, not retail: takes the address of the in-class StatGetClassTypeName so this object emits it out of line as a
// pick-any COMDAT (retail 0x004D4A86 is that out-of-line copy). Internal to this TU; never called.
CCharString (__fastcall* ReferenceStatGetClassTypeName())()
{
    return &CTCBerserk::StatGetClassTypeName;
}
}
