// CAIStateGroup_SmallBirdWander::GetName() -- static: the AI state group name "SG_SMALL_BIRD_WANDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SmallBirdWander@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD816: push esi; push -1; push "SG_SMALL_BIRD_WANDER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121ff0c registers this GetName with Alloc 0x8ff84e (pushed before it); Alloc 0x8ff84e stores the vtable of CAIStateGroup_SmallBirdWander into the new object; literal "SG_SMALL_BIRD_WANDER"; PDB name set ?GetName@CAIStateGroup_SmallBirdWander@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SmallBirdWander@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd816::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SmallBirdWander
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SmallBirdWander::GetName()
{
    return CCharString((const char*)0x012895B0, -1);   // "SG_SMALL_BIRD_WANDER"
}
