// CTCWobble::StatGetClassTypeName() -- static: the class-type name "CTCWobble" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCWobble@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D61B7: push esi; push -1; push "CTCWobble"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123df24 = "CTCWobble" = the class name; PDB name set ?StatGetClassTypeName@CTCWobble@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d61b7::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCWobble
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCWobble::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123DF24, -1);   // "CTCWobble"
}
