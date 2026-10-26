// CTCActionUseActivateQuest::StatGetClassTypeName() -- static: the class-type name "CTCActionUseActivateQuest" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCActionUseActivateQuest@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D57FA: push esi; push -1; push "CTCActionUseActivateQuest"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d2c4 = "CTCActionUseActivateQuest" = the class name; PDB name set ?StatGetClassTypeName@CTCActionUseActivateQuest@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d57fa::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCActionUseActivateQuest
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCActionUseActivateQuest::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D2C4, -1);   // "CTCActionUseActivateQuest"
}
