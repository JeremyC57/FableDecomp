// CTCBuyableHouse::StatGetClassTypeName() -- static: the class-type name "CTCBuyableHouse" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBuyableHouse@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3208: push esi; push -1; push "CTCBuyableHouse"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a5ec = "CTCBuyableHouse" = the class name; PDB name set ?StatGetClassTypeName@CTCBuyableHouse@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d3208::build).
#define CTCBuyableHouse CTCBuyableHouseLayout
#include "engine/CTCBuyableHouse.h"
#undef CTCBuyableHouse

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBuyableHouse : public CTCBuyableHouseLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCBuyableHouse::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A5EC, -1);   // "CTCBuyableHouse"
}
