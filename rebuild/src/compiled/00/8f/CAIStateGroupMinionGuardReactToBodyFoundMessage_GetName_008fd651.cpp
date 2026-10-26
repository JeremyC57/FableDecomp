// CAIStateGroup_MinionGuardReactToBodyFoundMessage::GetName() -- static: the AI state group name "SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD651: push esi; push -1; push "SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f9bc registers this GetName with Alloc 0x8ff462 (pushed before it); Alloc 0x8ff462 stores the vtable of CAIStateGroup_MinionGuardReactToBodyFoundMessage into the new object; literal "SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE"; PDB name set ?GetName@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd651::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGuardReactToBodyFoundMessage
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGuardReactToBodyFoundMessage::GetName()
{
    return CCharString((const char*)0x01289288, -1);   // "SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE"
}
