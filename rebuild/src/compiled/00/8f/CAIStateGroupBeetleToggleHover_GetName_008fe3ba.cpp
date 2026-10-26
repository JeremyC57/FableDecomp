// CAIStateGroup_BeetleToggleHover::GetName() -- static: the AI state group name "SG_BEETLE_TOGGLE_HOVER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BeetleToggleHover@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE3BA: push esi; push -1; push "SG_BEETLE_TOGGLE_HOVER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220ed4 registers this GetName with Alloc 0x9011e6 (pushed before it); Alloc 0x9011e6 stores the vtable of CAIStateGroup_BeetleToggleHover into the new object; literal "SG_BEETLE_TOGGLE_HOVER"; PDB name set ?GetName@CAIStateGroup_BeetleToggleHover@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BeetleToggleHover@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe3ba::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BeetleToggleHover
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BeetleToggleHover::GetName()
{
    return CCharString((const char*)0x0128A6D0, -1);   // "SG_BEETLE_TOGGLE_HOVER"
}
