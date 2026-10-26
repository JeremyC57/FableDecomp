// CTCCarriedActionUsePotion::StatGetClassTypeName() -- static: the class-type name "CTCCarriedActionUsePotion" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCarriedActionUsePotion@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2D4C: push esi; push -1; push "CTCCarriedActionUsePotion"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a2d0 = "CTCCarriedActionUsePotion" = the class name; PDB name set ?StatGetClassTypeName@CTCCarriedActionUsePotion@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d2d4c::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCarriedActionUsePotion
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCarriedActionUsePotion::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A2D0, -1);   // "CTCCarriedActionUsePotion"
}
