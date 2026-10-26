// CAIStateGroup_Guard::GetName() -- static: the AI state group name "SG_GUARD" as a CCharString
// Leaf: ?GetName@CAIStateGroup_Guard@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD6D5: push esi; push -1; push "SG_GUARD"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121ffac registers this GetName with Alloc 0x8ff620 (pushed before it); Alloc 0x8ff620 stores the vtable of CAIStateGroup_Guard into the new object; literal "SG_GUARD"; PDB name set ?GetName@CAIStateGroup_Guard@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_Guard@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd6d5::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_Guard
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_Guard::GetName()
{
    return CCharString((const char*)0x0128935C, -1);   // "SG_GUARD"
}
