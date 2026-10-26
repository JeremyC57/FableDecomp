// CAIStateGroup_Wander::GetName() -- static: the AI state group name "SG_WANDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_Wander@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD6FA: push esi; push -1; push "SG_WANDER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121ffd4 registers this GetName with Alloc 0x8ff660 (pushed before it); Alloc 0x8ff660 stores the vtable of CAIStateGroup_Wander into the new object; literal "SG_WANDER"; PDB name set ?GetName@CAIStateGroup_Wander@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_Wander@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd6fa::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_Wander
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_Wander::GetName()
{
    return CCharString((const char*)0x012893A4, -1);   // "SG_WANDER"
}
