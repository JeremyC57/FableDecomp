// CAIStateGroup_MinionBehaviourSad::GetName() -- static: the AI state group name "SG_MINION_BEHAVIOUR_SAD" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionBehaviourSad@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDFF3: push esi; push -1; push "SG_MINION_BEHAVIOUR_SAD"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220f24 registers this GetName with Alloc 0x902e25 (pushed before it); Alloc 0x902e25 -> ctor 0x902825 stores (last) the vtable of CAIStateGroup_MinionBehaviourSad at this+0; literal "SG_MINION_BEHAVIOUR_SAD"; PDB name set ?GetName@CAIStateGroup_MinionBehaviourSad@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionBehaviourSad@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdff3::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionBehaviourSad
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionBehaviourSad::GetName()
{
    return CCharString((const char*)0x0128A064, -1);   // "SG_MINION_BEHAVIOUR_SAD"
}
