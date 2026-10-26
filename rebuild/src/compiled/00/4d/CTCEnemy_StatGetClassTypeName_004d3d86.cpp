// CTCEnemy::StatGetClassTypeName() -- static: the class-type name "CTCEnemy" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCEnemy@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3D86: push esi; push -1; push "CTCEnemy"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123b3d8 = "CTCEnemy" = the class name; PDB name set ?StatGetClassTypeName@CTCEnemy@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d3d86::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCEnemy
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCEnemy::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123B3D8, -1);   // "CTCEnemy"
}
