// CAIStateGroup_KidReactToBlockage::GetName() -- static: the AI state group name "SG_KID_REACT_TO_BLOCKAGE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_KidReactToBlockage@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD9EC: push esi; push -1; push "SG_KID_REACT_TO_BLOCKAGE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122031c registers this GetName with Alloc 0x902450 (pushed before it); Alloc 0x902450 -> ctor 0x8ffc78 stores (last) the vtable of CAIStateGroup_KidReactToBlockage at this+0; literal "SG_KID_REACT_TO_BLOCKAGE"; PDB name set ?GetName@CAIStateGroup_KidReactToBlockage@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_KidReactToBlockage@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd9ec::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_KidReactToBlockage
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_KidReactToBlockage::GetName()
{
    return CCharString((const char*)0x0128985C, -1);   // "SG_KID_REACT_TO_BLOCKAGE"
}
