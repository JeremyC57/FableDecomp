// CAIStateGroup_GoToPub::GetName() -- static: the AI state group name "SG_GO_TO_PUB" as a CCharString
// Leaf: ?GetName@CAIStateGroup_GoToPub@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDCB7: push esi; push -1; push "SG_GO_TO_PUB"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12205ec registers this GetName with Alloc 0x902582 (pushed before it); Alloc 0x902582 -> ctor 0x900173 stores (last) the vtable of CAIStateGroup_GoToPub at this+0; literal "SG_GO_TO_PUB"; PDB name set ?GetName@CAIStateGroup_GoToPub@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_GoToPub@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdcb7::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_GoToPub
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_GoToPub::GetName()
{
    return CCharString((const char*)0x01289C24, -1);   // "SG_GO_TO_PUB"
}
