// CTCGhostSword::StatGetClassTypeName() -- static: the class-type name "CTCGhostSword" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCGhostSword@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D567D: push esi; push -1; push "CTCGhostSword"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d1a8 = "CTCGhostSword" = the class name; PDB name set ?StatGetClassTypeName@CTCGhostSword@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d567d::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCGhostSword
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCGhostSword::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D1A8, -1);   // "CTCGhostSword"
}
