// CAIStateGroup_VersusRockTroll::GetName() -- static: the AI state group name "SG_VERSUS_ROCK_TROLL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_VersusRockTroll@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE464: push esi; push -1; push "SG_VERSUS_ROCK_TROLL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220d44 registers this GetName with Alloc 0x902e75 (pushed before it); Alloc 0x902e75 stores the vtable of CAIStateGroup_VersusRockTroll into the new object; literal "SG_VERSUS_ROCK_TROLL"; PDB name set ?GetName@CAIStateGroup_VersusRockTroll@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_VersusRockTroll@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe464::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_VersusRockTroll
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_VersusRockTroll::GetName()
{
    return CCharString((const char*)0x0128A7E4, -1);   // "SG_VERSUS_ROCK_TROLL"
}
