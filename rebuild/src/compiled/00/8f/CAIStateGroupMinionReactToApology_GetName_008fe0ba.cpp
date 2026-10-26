// CAIStateGroup_MinionReactToApology::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_APOLOGY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToApology@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE0BA: push esi; push -1; push "SG_MINION_REACT_TO_APOLOGY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221014 registers this GetName with Alloc 0x900baf (pushed before it); Alloc 0x900baf stores the vtable of CAIStateGroup_MinionReactToApology into the new object; literal "SG_MINION_REACT_TO_APOLOGY"; PDB name set ?GetName@CAIStateGroup_MinionReactToApology@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToApology@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe0ba::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToApology
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToApology::GetName()
{
    return CCharString((const char*)0x0128A17C, -1);   // "SG_MINION_REACT_TO_APOLOGY"
}
