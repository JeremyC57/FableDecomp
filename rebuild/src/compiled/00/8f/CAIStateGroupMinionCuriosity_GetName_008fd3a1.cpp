// CAIStateGroup_MinionCuriosity::GetName() -- static: the AI state group name "SG_MINION_CURIOSITY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionCuriosity@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD3A1: push esi; push -1; push "SG_MINION_CURIOSITY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fd2c registers this GetName with Alloc 0x901fad (pushed before it); Alloc 0x901fad stores the vtable of CAIStateGroup_MinionCuriosity into the new object; literal "SG_MINION_CURIOSITY"; PDB name set ?GetName@CAIStateGroup_MinionCuriosity@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionCuriosity@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd3a1::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionCuriosity
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionCuriosity::GetName()
{
    return CCharString((const char*)0x01288E08, -1);   // "SG_MINION_CURIOSITY"
}
