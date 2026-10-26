// NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::StatGetClassTypeName() -- static: the class-type name "CTCGraphicAppearanceStaticMeshFadeOutOnProximity" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCGraphicAppearanceStaticMeshFadeOutOnProximity@NGraphicAppearance@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4475: push esi; push -1; push "CTCGraphicAppearanceStaticMeshFadeOutOnProximity"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123bc6c = "CTCGraphicAppearanceStaticMeshFadeOutOnProximity" = the class name; PDB name set ?StatGetClassTypeName@CTCGraphicAppearanceStaticMeshFadeOutOnProximity@NGraphicAppearance@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4475::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NGraphicAppearance {
class CTCGraphicAppearanceStaticMeshFadeOutOnProximity
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123BC6C, -1);   // "CTCGraphicAppearanceStaticMeshFadeOutOnProximity"
}
