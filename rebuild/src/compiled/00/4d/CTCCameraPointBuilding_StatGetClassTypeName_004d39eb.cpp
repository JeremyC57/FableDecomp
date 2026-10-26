// CTCCameraPointBuilding::StatGetClassTypeName() -- static: the class-type name "CTCCameraPointBuilding" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCameraPointBuilding@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D39EB: push esi; push -1; push "CTCCameraPointBuilding"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123aec0 = "CTCCameraPointBuilding" = the class name; PDB name set ?StatGetClassTypeName@CTCCameraPointBuilding@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d39eb::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCameraPointBuilding
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCameraPointBuilding::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123AEC0, -1);   // "CTCCameraPointBuilding"
}
