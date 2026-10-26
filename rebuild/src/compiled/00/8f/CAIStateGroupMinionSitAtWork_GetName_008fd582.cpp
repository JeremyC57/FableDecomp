// CAIStateGroup_MinionSitAtWork::GetName() -- static: the AI state group name "SG_MINION_SIT_AT_WORK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionSitAtWork@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD582: push esi; push -1; push "SG_MINION_SIT_AT_WORK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fc14 registers this GetName with Alloc 0x90215c (pushed before it); Alloc 0x90215c stores the vtable of CAIStateGroup_MinionSitAtWork into the new object; literal "SG_MINION_SIT_AT_WORK"; PDB name set ?GetName@CAIStateGroup_MinionSitAtWork@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionSitAtWork@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd582::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionSitAtWork
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionSitAtWork::GetName()
{
    return CCharString((const char*)0x01289118, -1);   // "SG_MINION_SIT_AT_WORK"
}
