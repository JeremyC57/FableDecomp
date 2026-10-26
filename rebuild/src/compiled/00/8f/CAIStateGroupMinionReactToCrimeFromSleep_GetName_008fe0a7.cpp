// CAIStateGroup_MinionReactToCrimeFromSleep::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_CRIME_FROM_SLEEP" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToCrimeFromSleep@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE0A7: push esi; push -1; push "SG_MINION_REACT_TO_CRIME_FROM_SLEEP"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221064 registers this GetName with Alloc 0x9028e3 (pushed before it); Alloc 0x9028e3 stores the vtable of CAIStateGroup_MinionReactToCrimeFromSleep into the new object; literal "SG_MINION_REACT_TO_CRIME_FROM_SLEEP"; PDB name set ?GetName@CAIStateGroup_MinionReactToCrimeFromSleep@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToCrimeFromSleep@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe0a7::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToCrimeFromSleep
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToCrimeFromSleep::GetName()
{
    return CCharString((const char*)0x0128A158, -1);   // "SG_MINION_REACT_TO_CRIME_FROM_SLEEP"
}
