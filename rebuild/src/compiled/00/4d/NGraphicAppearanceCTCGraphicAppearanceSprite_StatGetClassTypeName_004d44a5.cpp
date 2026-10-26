// NGraphicAppearance::CTCGraphicAppearanceSprite::StatGetClassTypeName() -- static: the class-type name "CTCGraphicAppearanceSprite" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCGraphicAppearanceSprite@NGraphicAppearance@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D44A5: push esi; push -1; push "CTCGraphicAppearanceSprite"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123bca0 = "CTCGraphicAppearanceSprite" = the class name; PDB name set ?StatGetClassTypeName@CTCGraphicAppearanceSprite@NGraphicAppearance@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d44a5::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NGraphicAppearance {
class CTCGraphicAppearanceSprite
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NGraphicAppearance::CTCGraphicAppearanceSprite::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123BCA0, -1);   // "CTCGraphicAppearanceSprite"
}
