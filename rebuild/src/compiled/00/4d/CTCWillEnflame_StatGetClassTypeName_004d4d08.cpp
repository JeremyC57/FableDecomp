// CTCWillEnflame::StatGetClassTypeName() -- static: the class-type name "CTCWillEnflame" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCWillEnflame@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4D08: push esi; push -1; push "CTCWillEnflame"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c444 = "CTCWillEnflame" = the class name; PDB name set ?StatGetClassTypeName@CTCWillEnflame@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4d08::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCWillEnflame
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCWillEnflame::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C444, -1);   // "CTCWillEnflame"
}
