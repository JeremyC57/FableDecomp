// CAIStateGroup_WorkInWarehouse::GetName() -- static: the AI state group name "SG_WORK_IN_WAREHOUSE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_WorkInWarehouse@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDA7C: push esi; push -1; push "SG_WORK_IN_WAREHOUSE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12203bc registers this GetName with Alloc 0x9024ae (pushed before it); Alloc 0x9024ae -> ctor 0x8ffd3a stores (last) the vtable of CAIStateGroup_WorkInWarehouse at this+0; literal "SG_WORK_IN_WAREHOUSE"; PDB name set ?GetName@CAIStateGroup_WorkInWarehouse@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_WorkInWarehouse@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fda7c::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_WorkInWarehouse
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_WorkInWarehouse::GetName()
{
    return CCharString((const char*)0x01289914, -1);   // "SG_WORK_IN_WAREHOUSE"
}
