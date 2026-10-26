// CAIStateGroup_MinionMotherConsoleChild::GetName() -- static: the AI state group name "SG_MINION_MOTHER_CONSOLE_CHILD" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionMotherConsoleChild@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE214: push esi; push -1; push "SG_MINION_MOTHER_CONSOLE_CHILD"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220f74 registers this GetName with Alloc 0x900dde (pushed before it); Alloc 0x900dde stores the vtable of CAIStateGroup_MinionMotherConsoleChild into the new object; literal "SG_MINION_MOTHER_CONSOLE_CHILD"; PDB name set ?GetName@CAIStateGroup_MinionMotherConsoleChild@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionMotherConsoleChild@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe214::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionMotherConsoleChild
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionMotherConsoleChild::GetName()
{
    return CCharString((const char*)0x0128A3B8, -1);   // "SG_MINION_MOTHER_CONSOLE_CHILD"
}
