// CTCSwitchableNavigation::StatGetClassTypeName() -- static: the class-type name "CTCSwitchableNavigation" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCSwitchableNavigation@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D482C: push esi; push -1; push "CTCSwitchableNavigation"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c010 = "CTCSwitchableNavigation" = the class name; PDB name set ?StatGetClassTypeName@CTCSwitchableNavigation@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d482c::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCSwitchableNavigation
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCSwitchableNavigation::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C010, -1);   // "CTCSwitchableNavigation"
}
