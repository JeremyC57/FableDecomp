// CAIStateGroup_MinionGuardBribed::GetName() -- static: the AI state group name "SG_MINION_GUARD_BRIBED" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGuardBribed@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE82A: push esi; push -1; push "SG_MINION_GUARD_BRIBED"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12216cc registers this GetName with Alloc 0x901d53 (pushed before it); Alloc 0x901d53 stores the vtable of CAIStateGroup_MinionGuardBribed into the new object; literal "SG_MINION_GUARD_BRIBED"; PDB name set ?GetName@CAIStateGroup_MinionGuardBribed@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGuardBribed@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe82a::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGuardBribed
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGuardBribed::GetName()
{
    return CCharString((const char*)0x0128AE58, -1);   // "SG_MINION_GUARD_BRIBED"
}
