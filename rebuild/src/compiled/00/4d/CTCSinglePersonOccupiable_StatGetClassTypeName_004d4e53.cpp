// CTCSinglePersonOccupiable::StatGetClassTypeName() -- static: the class-type name "CTCSinglePersonOccupiable" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCSinglePersonOccupiable@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4E53: push esi; push -1; push "CTCSinglePersonOccupiable"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c4f0 = "CTCSinglePersonOccupiable" = the class name; PDB name set ?StatGetClassTypeName@CTCSinglePersonOccupiable@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4e53::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCSinglePersonOccupiable
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCSinglePersonOccupiable::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C4F0, -1);   // "CTCSinglePersonOccupiable"
}
