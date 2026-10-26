// CTCGamePlayer::StatGetClassTypeName() -- static: the class-type name "CTCGamePlayer" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCGamePlayer@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5ADD: push esi; push -1; push "CTCGamePlayer"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d5c0 = "CTCGamePlayer" = the class name; PDB name set ?StatGetClassTypeName@CTCGamePlayer@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d5add::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCGamePlayer
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCGamePlayer::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D5C0, -1);   // "CTCGamePlayer"
}
