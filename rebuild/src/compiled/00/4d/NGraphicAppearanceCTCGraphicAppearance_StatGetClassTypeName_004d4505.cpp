// NGraphicAppearance::CTCGraphicAppearance::StatGetClassTypeName() -- static: the class-type name "CTCGraphicAppearance" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCGraphicAppearance@NGraphicAppearance@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4505: push esi; push -1; push "CTCGraphicAppearance"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x12398dc = "CTCGraphicAppearance" = the class name; PDB name set ?StatGetClassTypeName@CTCGraphicAppearance@NGraphicAppearance@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4505::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NGraphicAppearance {
class CTCGraphicAppearance
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NGraphicAppearance::CTCGraphicAppearance::StatGetClassTypeName()
{
    return CCharString((const char*)0x012398DC, -1);   // "CTCGraphicAppearance"
}
