// CTCTavernTable::StatGetClassTypeName() -- static: the class-type name "CTCTavernTable" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCTavernTable@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D515F: push esi; push -1; push "CTCTavernTable"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c9c8 = "CTCTavernTable" = the class name; PDB name set ?StatGetClassTypeName@CTCTavernTable@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d515f::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCTavernTable
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCTavernTable::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C9C8, -1);   // "CTCTavernTable"
}
