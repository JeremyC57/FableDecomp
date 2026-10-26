// CTCNamedUserActionPoints::StatGetClassTypeName() -- static: the class-type name "CTCNamedUserActionPoints" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCNamedUserActionPoints@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D34ED: push esi; push -1; push "CTCNamedUserActionPoints"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123aa10 = "CTCNamedUserActionPoints" = the class name; PDB name set ?StatGetClassTypeName@CTCNamedUserActionPoints@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d34ed::build).
#define CTCNamedUserActionPoints CTCNamedUserActionPointsLayout
#include "engine/CTCNamedUserActionPoints.h"
#undef CTCNamedUserActionPoints

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCNamedUserActionPoints : public CTCNamedUserActionPointsLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCNamedUserActionPoints::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123AA10, -1);   // "CTCNamedUserActionPoints"
}
