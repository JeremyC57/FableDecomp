// CAIStateGroup_VillageTaskTorch::GetName() -- static: the AI state group name "SG_VILLAGE_TASK_TORCH" as a CCharString
// Leaf: ?GetName@CAIStateGroup_VillageTaskTorch@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD910: push esi; push -1; push "SG_VILLAGE_TASK_TORCH"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12201b4 registers this GetName with Alloc 0x9023b6 (pushed before it); Alloc 0x9023b6 -> ctor 0x8ff9f2 stores (last) the vtable of CAIStateGroup_VillageTaskTorch at this+0; literal "SG_VILLAGE_TASK_TORCH"; PDB name set ?GetName@CAIStateGroup_VillageTaskTorch@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_VillageTaskTorch@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd910::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_VillageTaskTorch
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_VillageTaskTorch::GetName()
{
    return CCharString((const char*)0x012896F8, -1);   // "SG_VILLAGE_TASK_TORCH"
}
