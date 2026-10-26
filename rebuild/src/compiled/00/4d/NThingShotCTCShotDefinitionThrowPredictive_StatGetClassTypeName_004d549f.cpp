// NThingShot::CTCShotDefinitionThrowPredictive::StatGetClassTypeName() -- static: the class-type name "CTCShotDefinitionThrowPredictive" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCShotDefinitionThrowPredictive@NThingShot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D549F: push esi; push -1; push "CTCShotDefinitionThrowPredictive"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123ce74 = "CTCShotDefinitionThrowPredictive" = the class name; PDB name set ?StatGetClassTypeName@CTCShotDefinitionThrowPredictive@NThingShot@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d549f::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NThingShot {
class CTCShotDefinitionThrowPredictive
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

CCharString __fastcall NThingShot::CTCShotDefinitionThrowPredictive::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123CE74, -1);   // "CTCShotDefinitionThrowPredictive"
}
