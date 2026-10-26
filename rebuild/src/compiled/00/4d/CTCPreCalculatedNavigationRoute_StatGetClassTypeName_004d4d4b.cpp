// CTCPreCalculatedNavigationRoute::StatGetClassTypeName() -- static: the class-type name "CTCPreCalculatedNavigationRoute" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCPreCalculatedNavigationRoute@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4D4B: push esi; push -1; push "CTCPreCalculatedNavigationRoute"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c468 = "CTCPreCalculatedNavigationRoute" = the class name; PDB name set ?StatGetClassTypeName@CTCPreCalculatedNavigationRoute@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4d4b::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCPreCalculatedNavigationRoute
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCPreCalculatedNavigationRoute::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C468, -1);   // "CTCPreCalculatedNavigationRoute"
}
