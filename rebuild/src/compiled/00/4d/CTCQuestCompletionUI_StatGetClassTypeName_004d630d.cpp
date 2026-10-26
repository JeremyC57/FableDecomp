// CTCQuestCompletionUI::StatGetClassTypeName() -- static: the class-type name "CTCQuestCompletionUI" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCQuestCompletionUI@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D630D: push esi; push -1; push "CTCQuestCompletionUI"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123e0a4 = "CTCQuestCompletionUI" = the class name; PDB name set ?StatGetClassTypeName@CTCQuestCompletionUI@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d630d::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCQuestCompletionUI
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCQuestCompletionUI::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123E0A4, -1);   // "CTCQuestCompletionUI"
}
