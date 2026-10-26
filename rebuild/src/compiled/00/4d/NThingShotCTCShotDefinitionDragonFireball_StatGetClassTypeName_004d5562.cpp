// NThingShot::CTCShotDefinitionDragonFireball::StatGetClassTypeName() -- static: the class-type name "CTCShotDefinitionDragonFireball" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCShotDefinitionDragonFireball@NThingShot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5562: push esi; push -1; push "CTCShotDefinitionDragonFireball"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d0a8 = "CTCShotDefinitionDragonFireball" = the class name; PDB name set ?StatGetClassTypeName@CTCShotDefinitionDragonFireball@NThingShot@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d5562::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NThingShot {
class CTCShotDefinitionDragonFireball
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NThingShot::CTCShotDefinitionDragonFireball::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D0A8, -1);   // "CTCShotDefinitionDragonFireball"
}
