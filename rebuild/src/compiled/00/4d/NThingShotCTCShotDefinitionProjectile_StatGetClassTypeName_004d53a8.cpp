// NThingShot::CTCShotDefinitionProjectile::StatGetClassTypeName() -- static: the class-type name "CTCShotDefinitionProjectile" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCShotDefinitionProjectile@NThingShot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D53A8: push esi; push -1; push "CTCShotDefinitionProjectile"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123cd08 = "CTCShotDefinitionProjectile" = the class name; PDB name set ?StatGetClassTypeName@CTCShotDefinitionProjectile@NThingShot@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d53a8::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NThingShot {
class CTCShotDefinitionProjectile
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NThingShot::CTCShotDefinitionProjectile::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123CD08, -1);   // "CTCShotDefinitionProjectile"
}
