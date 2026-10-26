// CAIStateGroup_MinionBehaviourGreetCreature::GetName() -- static: the AI state group name "SG_MINION_BEHAVIOUR_GREET_CREATURE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionBehaviourGreetCreature@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDFE0: push esi; push -1; push "SG_MINION_BEHAVIOUR_GREET_CREATURE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220efc registers this GetName with Alloc 0x90280f (pushed before it); Alloc 0x90280f -> ctor 0x90095d stores (last) the vtable of CAIStateGroup_MinionBehaviourGreetCreature at this+0; literal "SG_MINION_BEHAVIOUR_GREET_CREATURE"; PDB name set ?GetName@CAIStateGroup_MinionBehaviourGreetCreature@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionBehaviourGreetCreature@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdfe0::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionBehaviourGreetCreature
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionBehaviourGreetCreature::GetName()
{
    return CCharString((const char*)0x0128A040, -1);   // "SG_MINION_BEHAVIOUR_GREET_CREATURE"
}
