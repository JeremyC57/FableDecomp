// CTCCarriedActionUseWallMount::StatGetClassTypeName() -- static: the class-type name "CTCCarriedActionUseWallMount" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCarriedActionUseWallMount@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2D5F: push esi; push -1; push "CTCCarriedActionUseWallMount"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a2ec = "CTCCarriedActionUseWallMount" = the class name; PDB name set ?StatGetClassTypeName@CTCCarriedActionUseWallMount@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d2d5f::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCarriedActionUseWallMount
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCarriedActionUseWallMount::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A2EC, -1);   // "CTCCarriedActionUseWallMount"
}
