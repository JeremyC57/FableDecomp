// CAIStateGroup_VillageTaskSecuritySweep::GetName() -- static: the AI state group name "SG_VILLAGE_TASK_SECURITY_SWEEP" as a CCharString
// Leaf: ?GetName@CAIStateGroup_VillageTaskSecuritySweep@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD93B: push esi; push -1; push "SG_VILLAGE_TASK_SECURITY_SWEEP"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220204 registers this GetName with Alloc 0x9023e2 (pushed before it); Alloc 0x9023e2 -> ctor 0x8ffa86 stores (last) the vtable of CAIStateGroup_VillageTaskSecuritySweep at this+0; literal "SG_VILLAGE_TASK_SECURITY_SWEEP"; PDB name set ?GetName@CAIStateGroup_VillageTaskSecuritySweep@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_VillageTaskSecuritySweep@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd93b::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_VillageTaskSecuritySweep
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_VillageTaskSecuritySweep::GetName()
{
    return CCharString((const char*)0x01289728, -1);   // "SG_VILLAGE_TASK_SECURITY_SWEEP"
}
