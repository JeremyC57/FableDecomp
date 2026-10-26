// CTCBandit::StatGetClassTypeName() -- static: the class-type name "CTCBandit" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBandit@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3DB6: push esi; push -1; push "CTCBandit"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123b3e4 = "CTCBandit" = the class name; PDB name set ?StatGetClassTypeName@CTCBandit@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d3db6::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBandit
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCBandit::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123B3E4, -1);   // "CTCBandit"
}
