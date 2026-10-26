// CTCCreatedEntity::StatGetClassTypeName() -- static: the class-type name "CTCCreatedEntity" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCreatedEntity@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3576: push esi; push -1; push "CTCCreatedEntity"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123aa60 = "CTCCreatedEntity" = the class name; PDB name set ?StatGetClassTypeName@CTCCreatedEntity@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d3576::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCreatedEntity
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCreatedEntity::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123AA60, -1);   // "CTCCreatedEntity"
}
