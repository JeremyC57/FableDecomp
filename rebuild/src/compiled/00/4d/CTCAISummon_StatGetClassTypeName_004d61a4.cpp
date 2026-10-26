// CTCAISummon::StatGetClassTypeName() -- static: the class-type name "CTCAISummon" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCAISummon@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D61A4: push esi; push -1; push "CTCAISummon"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123df18 = "CTCAISummon" = the class name; PDB name set ?StatGetClassTypeName@CTCAISummon@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d61a4::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCAISummon
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCAISummon::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123DF18, -1);   // "CTCAISummon"
}
