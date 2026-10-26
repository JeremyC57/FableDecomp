// CTCBlacksmiths::StatGetClassTypeName() -- static: the class-type name "CTCBlacksmiths" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBlacksmiths@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D40AC: push esi; push -1; push "CTCBlacksmiths"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123b6b0 = "CTCBlacksmiths" = the class name; PDB name set ?StatGetClassTypeName@CTCBlacksmiths@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d40ac::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBlacksmiths
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCBlacksmiths::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123B6B0, -1);   // "CTCBlacksmiths"
}
