// CAIStateGroup_MinionMotherProtectFamily::GetName() -- static: the AI state group name "SG_MINION_MOTHER_PROTECT_FAMILY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionMotherProtectFamily@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD40D: push esi; push -1; push "SG_MINION_MOTHER_PROTECT_FAMILY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fda4 registers this GetName with Alloc 0x901fd1 (pushed before it); Alloc 0x901fd1 -> ctor 0x8fee89 stores (last) the vtable of CAIStateGroup_MinionMotherProtectFamily at this+0; literal "SG_MINION_MOTHER_PROTECT_FAMILY"; PDB name set ?GetName@CAIStateGroup_MinionMotherProtectFamily@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionMotherProtectFamily@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd40d::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionMotherProtectFamily
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionMotherProtectFamily::GetName()
{
    return CCharString((const char*)0x01288ED8, -1);   // "SG_MINION_MOTHER_PROTECT_FAMILY"
}
