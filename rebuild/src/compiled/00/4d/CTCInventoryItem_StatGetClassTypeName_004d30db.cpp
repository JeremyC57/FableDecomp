// CTCInventoryItem::StatGetClassTypeName() -- static: the class-type name "CTCInventoryItem" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCInventoryItem@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D30DB: push esi; push -1; push "CTCInventoryItem"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a4f0 = "CTCInventoryItem" = the class name; PDB name set ?StatGetClassTypeName@CTCInventoryItem@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d30db::build).
#define CTCInventoryItem CTCInventoryItemLayout
#include "engine/CTCInventoryItem.h"
#undef CTCInventoryItem

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCInventoryItem : public CTCInventoryItemLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCInventoryItem::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A4F0, -1);   // "CTCInventoryItem"
}
