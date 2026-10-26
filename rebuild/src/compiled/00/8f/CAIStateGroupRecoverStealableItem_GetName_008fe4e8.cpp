// CAIStateGroup_RecoverStealableItem::GetName() -- static: the AI state group name "SG_RECOVER_STEALABLE_ITEM" as a CCharString
// Leaf: ?GetName@CAIStateGroup_RecoverStealableItem@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE4E8: push esi; push -1; push "SG_RECOVER_STEALABLE_ITEM"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12212e4 registers this GetName with Alloc 0x901603 (pushed before it); Alloc 0x901603 -> ctor 0x9015b9 stores (last) the vtable of CAIStateGroup_RecoverStealableItem at this+0; literal "SG_RECOVER_STEALABLE_ITEM"; PDB name set ?GetName@CAIStateGroup_RecoverStealableItem@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_RecoverStealableItem@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe4e8::build).
#define CAIStateGroup_RecoverStealableItem CAIStateGroup_RecoverStealableItemLayout
#include "engine/CAIStateGroup_RecoverStealableItem.h"
#undef CAIStateGroup_RecoverStealableItem

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_RecoverStealableItem : public CAIStateGroup_RecoverStealableItemLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_RecoverStealableItem::GetName()
{
    return CCharString((const char*)0x0128A89C, -1);   // "SG_RECOVER_STEALABLE_ITEM"
}
