// CTCVillageMember::StatGetClassTypeName() -- static: the class-type name "CTCVillageMember" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCVillageMember@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D31D8: push esi; push -1; push "CTCVillageMember"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a5d8 = "CTCVillageMember" = the class name; PDB name set ?StatGetClassTypeName@CTCVillageMember@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d31d8::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCVillageMember
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCVillageMember::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A5D8, -1);   // "CTCVillageMember"
}
