// CAIStateGroup_MinionGuardHeardYellForHelp::GetName() -- static: the AI state group name "SG_MINION_GUARD_HEARD_YELL_FOR_HELP" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGuardHeardYellForHelp@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD619: push esi; push -1; push "SG_MINION_GUARD_HEARD_YELL_FOR_HELP"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f96c registers this GetName with Alloc 0x8ff3d8 (pushed before it); Alloc 0x8ff3d8 stores the vtable of CAIStateGroup_MinionGuardHeardYellForHelp into the new object; literal "SG_MINION_GUARD_HEARD_YELL_FOR_HELP"; PDB name set ?GetName@CAIStateGroup_MinionGuardHeardYellForHelp@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGuardHeardYellForHelp@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd619::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGuardHeardYellForHelp
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGuardHeardYellForHelp::GetName()
{
    return CCharString((const char*)0x01289200, -1);   // "SG_MINION_GUARD_HEARD_YELL_FOR_HELP"
}
