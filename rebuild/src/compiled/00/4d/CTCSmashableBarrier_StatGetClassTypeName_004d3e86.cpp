// CTCSmashableBarrier::StatGetClassTypeName() -- static: the class-type name "CTCSmashableBarrier" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCSmashableBarrier@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3E86: push esi; push -1; push "CTCSmashableBarrier"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123b4a0 = "CTCSmashableBarrier" = the class name; PDB name set ?StatGetClassTypeName@CTCSmashableBarrier@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d3e86::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCSmashableBarrier
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCSmashableBarrier::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123B4A0, -1);   // "CTCSmashableBarrier"
}
