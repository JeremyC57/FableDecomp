// CAIStateGroup_HoverAllAroundTest::GetName() -- static: the AI state group name "SG_HOVER_ALL_AROUND_TEST" as a CCharString
// Leaf: ?GetName@CAIStateGroup_HoverAllAroundTest@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE17E: push esi; push -1; push "SG_HOVER_ALL_AROUND_TEST"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12209fc registers this GetName with Alloc 0x900cb1 (pushed before it); Alloc 0x900cb1 stores the vtable of CAIStateGroup_HoverAllAroundTest into the new object; literal "SG_HOVER_ALL_AROUND_TEST"; PDB name set ?GetName@CAIStateGroup_HoverAllAroundTest@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_HoverAllAroundTest@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe17e::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_HoverAllAroundTest
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_HoverAllAroundTest::GetName()
{
    return CCharString((const char*)0x0128A2A0, -1);   // "SG_HOVER_ALL_AROUND_TEST"
}
