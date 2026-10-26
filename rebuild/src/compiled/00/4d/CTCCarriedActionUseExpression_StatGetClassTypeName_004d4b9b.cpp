// CTCCarriedActionUseExpression::StatGetClassTypeName() -- static: the class-type name "CTCCarriedActionUseExpression" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCarriedActionUseExpression@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4B9B: push esi; push -1; push "CTCCarriedActionUseExpression"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c348 = "CTCCarriedActionUseExpression" = the class name; PDB name set ?StatGetClassTypeName@CTCCarriedActionUseExpression@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4b9b::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCarriedActionUseExpression
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCarriedActionUseExpression::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C348, -1);   // "CTCCarriedActionUseExpression"
}
