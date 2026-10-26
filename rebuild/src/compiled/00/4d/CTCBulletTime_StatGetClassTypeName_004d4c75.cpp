// CTCBulletTime::StatGetClassTypeName() -- static: the class-type name "CTCBulletTime" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBulletTime@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4C75: push esi; push -1; push "CTCBulletTime"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c408 = "CTCBulletTime" = the class name; PDB name set ?StatGetClassTypeName@CTCBulletTime@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4c75::build).
#define CTCBulletTime CTCBulletTimeLayout
#include "engine/CTCBulletTime.h"
#undef CTCBulletTime

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBulletTime : public CTCBulletTimeLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCBulletTime::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C408, -1);   // "CTCBulletTime"
}
