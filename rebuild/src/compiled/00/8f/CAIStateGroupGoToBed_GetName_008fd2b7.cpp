// CAIStateGroup_GoToBed::GetName() -- static: the AI state group name "SG_MINION_GO_TO_BED" as a CCharString
// Leaf: ?GetName@CAIStateGroup_GoToBed@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD2B7: push esi; push -1; push "SG_MINION_GO_TO_BED"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f8cc registers this GetName with Alloc 0x901ef7 (pushed before it); Alloc 0x901ef7 -> ctor 0x8feb6b stores (last) the vtable of CAIStateGroup_GoToBed at this+0; literal "SG_MINION_GO_TO_BED"; PDB name set ?GetName@CAIStateGroup_GoToBed@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_GoToBed@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd2b7::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_GoToBed
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_GoToBed::GetName()
{
    return CCharString((const char*)0x01288C98, -1);   // "SG_MINION_GO_TO_BED"
}
