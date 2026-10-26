// NThingShot::CTCShotDefinitionMultiArrow::StatGetClassTypeName() -- static: the class-type name "CTCShotDefinitionMultiArrow" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCShotDefinitionMultiArrow@NThingShot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D53DB: push esi; push -1; push "CTCShotDefinitionMultiArrow"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123cd24 = "CTCShotDefinitionMultiArrow" = the class name; PDB name set ?StatGetClassTypeName@CTCShotDefinitionMultiArrow@NThingShot@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d53db::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NThingShot {
class CTCShotDefinitionMultiArrow
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NThingShot::CTCShotDefinitionMultiArrow::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123CD24, -1);   // "CTCShotDefinitionMultiArrow"
}
