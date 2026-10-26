// CAIStateGroup_EatAtTavern::GetName() -- static: the AI state group name "SG_EAT_AT_TAVERN" as a CCharString
// Leaf: ?GetName@CAIStateGroup_EatAtTavern@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE5F0: push esi; push -1; push "SG_EAT_AT_TAVERN"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122149c registers this GetName with Alloc 0x902c83 (pushed before it); Alloc 0x902c83 stores the vtable of CAIStateGroup_EatAtTavern into the new object; literal "SG_EAT_AT_TAVERN"; PDB name set ?GetName@CAIStateGroup_EatAtTavern@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_EatAtTavern@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe5f0::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_EatAtTavern
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_EatAtTavern::GetName()
{
    return CCharString((const char*)0x0128A9EC, -1);   // "SG_EAT_AT_TAVERN"
}
