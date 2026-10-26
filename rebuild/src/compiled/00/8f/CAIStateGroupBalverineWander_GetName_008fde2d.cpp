// CAIStateGroup_BalverineWander::GetName() -- static: the AI state group name "SG_BALVERINE_WANDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BalverineWander@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDE2D: push esi; push -1; push "SG_BALVERINE_WANDER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12207f4 registers this GetName with Alloc 0x9004a0 (pushed before it); Alloc 0x9004a0 stores the vtable of CAIStateGroup_BalverineWander into the new object; literal "SG_BALVERINE_WANDER"; PDB name set ?GetName@CAIStateGroup_BalverineWander@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BalverineWander@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fde2d::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BalverineWander
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BalverineWander::GetName()
{
    return CCharString((const char*)0x01289DC0, -1);   // "SG_BALVERINE_WANDER"
}
