// CAIStateGroup_DeedReactionRootFromSleep::GetName() -- static: the AI state group name "SG_DEED_REACTION_ROOT_FROM_SLEEP" as a CCharString
// Leaf: ?GetName@CAIStateGroup_DeedReactionRootFromSleep@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE3A7: push esi; push -1; push "SG_DEED_REACTION_ROOT_FROM_SLEEP"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221154 registers this GetName with Alloc 0x902acc (pushed before it); Alloc 0x902acc stores the vtable of CAIStateGroup_DeedReactionRootFromSleep into the new object; literal "SG_DEED_REACTION_ROOT_FROM_SLEEP"; PDB name set ?GetName@CAIStateGroup_DeedReactionRootFromSleep@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_DeedReactionRootFromSleep@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe3a7::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_DeedReactionRootFromSleep
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_DeedReactionRootFromSleep::GetName()
{
    return CCharString((const char*)0x0128A6AC, -1);   // "SG_DEED_REACTION_ROOT_FROM_SLEEP"
}
