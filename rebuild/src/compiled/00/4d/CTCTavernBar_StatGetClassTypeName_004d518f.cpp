// CTCTavernBar::StatGetClassTypeName() -- static: the class-type name "CTCTavernBar" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCTavernBar@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D518F: push esi; push -1; push "CTCTavernBar"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c9d8 = "CTCTavernBar" = the class name; PDB name set ?StatGetClassTypeName@CTCTavernBar@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d518f::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCTavernBar
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCTavernBar::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C9D8, -1);   // "CTCTavernBar"
}
