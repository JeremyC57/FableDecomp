// CTCBattleCharge::StatGetClassTypeName() -- static: the class-type name "CTCBattleCharge" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBattleCharge@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4DC1: push esi; push -1; push "CTCBattleCharge"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c4c0 = "CTCBattleCharge" = the class name; PDB name set ?StatGetClassTypeName@CTCBattleCharge@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4dc1::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBattleCharge
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCBattleCharge::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C4C0, -1);   // "CTCBattleCharge"
}
