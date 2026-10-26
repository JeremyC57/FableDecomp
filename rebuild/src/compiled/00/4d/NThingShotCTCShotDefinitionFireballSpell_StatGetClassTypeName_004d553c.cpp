// NThingShot::CTCShotDefinitionFireballSpell::StatGetClassTypeName() -- static: the class-type name "CTCShotDefinitionFireballSpell" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCShotDefinitionFireballSpell@NThingShot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D553C: push esi; push -1; push "CTCShotDefinitionFireballSpell"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d06c = "CTCShotDefinitionFireballSpell" = the class name; PDB name set ?StatGetClassTypeName@CTCShotDefinitionFireballSpell@NThingShot@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d553c::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NThingShot {
class CTCShotDefinitionFireballSpell
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NThingShot::CTCShotDefinitionFireballSpell::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D06C, -1);   // "CTCShotDefinitionFireballSpell"
}
