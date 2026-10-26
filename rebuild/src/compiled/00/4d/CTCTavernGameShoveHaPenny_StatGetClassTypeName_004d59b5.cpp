// CTCTavernGameShoveHaPenny::StatGetClassTypeName() -- static: the class-type name "CTCTavernGameShoveHaPenny" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCTavernGameShoveHaPenny@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D59B5: push esi; push -1; push "CTCTavernGameShoveHaPenny"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d4b0 = "CTCTavernGameShoveHaPenny" = the class name; PDB name set ?StatGetClassTypeName@CTCTavernGameShoveHaPenny@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d59b5::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCTavernGameShoveHaPenny
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCTavernGameShoveHaPenny::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D4B0, -1);   // "CTCTavernGameShoveHaPenny"
}
