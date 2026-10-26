// CTCStockItem::StatGetClassTypeName() -- static: the class-type name "CTCStockItem" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCStockItem@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D40EB: push esi; push -1; push "CTCStockItem"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123b734 = "CTCStockItem" = the class name; PDB name set ?StatGetClassTypeName@CTCStockItem@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d40eb::build).
#define CTCStockItem CTCStockItemLayout
#include "engine/CTCStockItem.h"
#undef CTCStockItem

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCStockItem : public CTCStockItemLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCStockItem::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123B734, -1);   // "CTCStockItem"
}
