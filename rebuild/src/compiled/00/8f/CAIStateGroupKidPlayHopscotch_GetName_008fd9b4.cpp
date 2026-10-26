// CAIStateGroup_KidPlayHopscotch::GetName() -- static: the AI state group name "SG_KID_PLAY_HOPSCOTCH" as a CCharString
// Leaf: ?GetName@CAIStateGroup_KidPlayHopscotch@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD9B4: push esi; push -1; push "SG_KID_PLAY_HOPSCOTCH"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12202cc registers this GetName with Alloc 0x90243a (pushed before it); Alloc 0x90243a -> ctor 0x8ffbee stores (last) the vtable of CAIStateGroup_KidPlayHopscotch at this+0; literal "SG_KID_PLAY_HOPSCOTCH"; PDB name set ?GetName@CAIStateGroup_KidPlayHopscotch@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_KidPlayHopscotch@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd9b4::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_KidPlayHopscotch
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_KidPlayHopscotch::GetName()
{
    return CCharString((const char*)0x012897E8, -1);   // "SG_KID_PLAY_HOPSCOTCH"
}
