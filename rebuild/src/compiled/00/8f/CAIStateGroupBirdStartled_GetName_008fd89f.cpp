// CAIStateGroup_BirdStartled::GetName() -- static: the AI state group name "SG_BIRD_STARTLED" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BirdStartled@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD89F: push esi; push -1; push "SG_BIRD_STARTLED"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121ff84 registers this GetName with Alloc 0x8ff90e (pushed before it); Alloc 0x8ff90e stores the vtable of CAIStateGroup_BirdStartled into the new object; literal "SG_BIRD_STARTLED"; PDB name set ?GetName@CAIStateGroup_BirdStartled@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BirdStartled@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd89f::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BirdStartled
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BirdStartled::GetName()
{
    return CCharString((const char*)0x012896A8, -1);   // "SG_BIRD_STARTLED"
}
