// CTCCreatureFlyInCircles::StatGetClassTypeName() -- static: the class-type name "CTCCreatureFlyInCircles" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCreatureFlyInCircles@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5BBE: push esi; push -1; push "CTCCreatureFlyInCircles"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d70c = "CTCCreatureFlyInCircles" = the class name; PDB name set ?StatGetClassTypeName@CTCCreatureFlyInCircles@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d5bbe::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCreatureFlyInCircles
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCreatureFlyInCircles::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D70C, -1);   // "CTCCreatureFlyInCircles"
}
