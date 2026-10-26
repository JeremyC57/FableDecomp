// CAIStateGroup_MinionReactToEnemy::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_ENEMY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToEnemy@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD4E7: push esi; push -1; push "SG_MINION_REACT_TO_ENEMY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fb4c registers this GetName with Alloc 0x90210c (pushed before it); Alloc 0x90210c -> ctor 0x8ff12e stores (last) the vtable of CAIStateGroup_MinionReactToEnemy at this+0; literal "SG_MINION_REACT_TO_ENEMY"; PDB name set ?GetName@CAIStateGroup_MinionReactToEnemy@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToEnemy@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd4e7::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToEnemy
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToEnemy::GetName()
{
    return CCharString((const char*)0x01289034, -1);   // "SG_MINION_REACT_TO_ENEMY"
}
