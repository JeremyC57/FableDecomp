// CTCSingleUserActionPoints::StatGetClassTypeName() -- static: the class-type name "CTCSingleUserActionPoints" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCSingleUserActionPoints@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D34DA: push esi; push -1; push "CTCSingleUserActionPoints"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a9f4 = "CTCSingleUserActionPoints" = the class name; PDB name set ?StatGetClassTypeName@CTCSingleUserActionPoints@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d34da::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCSingleUserActionPoints
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCSingleUserActionPoints::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A9F4, -1);   // "CTCSingleUserActionPoints"
}
