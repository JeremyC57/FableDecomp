// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::StatGetClassTypeName() -- static: the class-type name "CTCGraphicAppearanceAnimatedMesh" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4415: push esi; push -1; push "CTCGraphicAppearanceAnimatedMesh"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123bc28 = "CTCGraphicAppearanceAnimatedMesh" = the class name; PDB name set ?StatGetClassTypeName@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4415::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NGraphicAppearance {
class CTCGraphicAppearanceAnimatedMesh
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123BC28, -1);   // "CTCGraphicAppearanceAnimatedMesh"
}
