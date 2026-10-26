// CTCWallMountEffects::StatGetClassTypeName() -- static: the class-type name "CTCWallMountEffects" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCWallMountEffects@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5C1E: push esi; push -1; push "CTCWallMountEffects"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d734 = "CTCWallMountEffects" = the class name; PDB name set ?StatGetClassTypeName@CTCWallMountEffects@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d5c1e::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCWallMountEffects
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCWallMountEffects::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D734, -1);   // "CTCWallMountEffects"
}
