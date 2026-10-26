// CTCCameraPointScripted::StatGetClassTypeName() -- static: the class-type name "CTCCameraPointScripted" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCameraPointScripted@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3BEB: push esi; push -1; push "CTCCameraPointScripted"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123b2cc = "CTCCameraPointScripted" = the class name; PDB name set ?StatGetClassTypeName@CTCCameraPointScripted@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d3beb::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCameraPointScripted
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCameraPointScripted::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123B2CC, -1);   // "CTCCameraPointScripted"
}
