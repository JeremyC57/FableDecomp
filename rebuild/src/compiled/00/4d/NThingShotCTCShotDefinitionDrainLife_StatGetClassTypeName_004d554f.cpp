// NThingShot::CTCShotDefinitionDrainLife::StatGetClassTypeName() -- static: the class-type name "CTCShotDefinitionDrainLife" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCShotDefinitionDrainLife@NThingShot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D554F: push esi; push -1; push "CTCShotDefinitionDrainLife"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d08c = "CTCShotDefinitionDrainLife" = the class name; PDB name set ?StatGetClassTypeName@CTCShotDefinitionDrainLife@NThingShot@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d554f::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NThingShot {
class CTCShotDefinitionDrainLife
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NThingShot::CTCShotDefinitionDrainLife::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D08C, -1);   // "CTCShotDefinitionDrainLife"
}
