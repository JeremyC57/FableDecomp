// CTCCrateStack::StatGetClassTypeName() -- static: the class-type name "CTCCrateStack" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCrateStack@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D50D3: push esi; push -1; push "CTCCrateStack"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c8d0 = "CTCCrateStack" = the class name; PDB name set ?StatGetClassTypeName@CTCCrateStack@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d50d3::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCrateStack
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCrateStack::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C8D0, -1);   // "CTCCrateStack"
}
