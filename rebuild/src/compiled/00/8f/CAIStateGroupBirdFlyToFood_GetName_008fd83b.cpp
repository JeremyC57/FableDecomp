// CAIStateGroup_BirdFlyToFood::GetName() -- static: the AI state group name "SG_BIRD_FLY_TO_FOOD" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BirdFlyToFood@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD83B: push esi; push -1; push "SG_BIRD_FLY_TO_FOOD"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121ff34 registers this GetName with Alloc 0x8ff88e (pushed before it); Alloc 0x8ff88e stores the vtable of CAIStateGroup_BirdFlyToFood into the new object; literal "SG_BIRD_FLY_TO_FOOD"; PDB name set ?GetName@CAIStateGroup_BirdFlyToFood@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BirdFlyToFood@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd83b::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BirdFlyToFood
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BirdFlyToFood::GetName()
{
    return CCharString((const char*)0x01289604, -1);   // "SG_BIRD_FLY_TO_FOOD"
}
