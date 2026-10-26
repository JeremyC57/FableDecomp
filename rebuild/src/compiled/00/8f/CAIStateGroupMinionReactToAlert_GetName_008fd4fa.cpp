// CAIStateGroup_MinionReactToAlert::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_ALERT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToAlert@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD4FA: push esi; push -1; push "SG_MINION_REACT_TO_ALERT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fb24 registers this GetName with Alloc 0x8ff178 (pushed before it); Alloc 0x8ff178 stores the vtable of CAIStateGroup_MinionReactToAlert into the new object; literal "SG_MINION_REACT_TO_ALERT"; PDB name set ?GetName@CAIStateGroup_MinionReactToAlert@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToAlert@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd4fa::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToAlert
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToAlert::GetName()
{
    return CCharString((const char*)0x01289050, -1);   // "SG_MINION_REACT_TO_ALERT"
}
