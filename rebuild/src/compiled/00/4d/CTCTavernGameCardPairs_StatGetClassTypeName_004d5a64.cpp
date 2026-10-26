// CTCTavernGameCardPairs::StatGetClassTypeName() -- static: the class-type name "CTCTavernGameCardPairs" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCTavernGameCardPairs@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5A64: push esi; push -1; push "CTCTavernGameCardPairs"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d56c = "CTCTavernGameCardPairs" = the class name; PDB name set ?StatGetClassTypeName@CTCTavernGameCardPairs@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d5a64::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCTavernGameCardPairs
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCTavernGameCardPairs::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D56C, -1);   // "CTCTavernGameCardPairs"
}
