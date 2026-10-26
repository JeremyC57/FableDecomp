// CAIStateGroup_StealStealableItem::GetName() -- static: the AI state group name "SG_STEAL_STEALABLE_ITEM" as a CCharString
// Leaf: ?GetName@CAIStateGroup_StealStealableItem@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE4D5: push esi; push -1; push "SG_STEAL_STEALABLE_ITEM"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12212bc registers this GetName with Alloc 0x901564 (pushed before it); Alloc 0x901564 -> ctor 0x90151a stores (last) the vtable of CAIStateGroup_StealStealableItem at this+0; literal "SG_STEAL_STEALABLE_ITEM"; PDB name set ?GetName@CAIStateGroup_StealStealableItem@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_StealStealableItem@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe4d5::build).
#define CAIStateGroup_StealStealableItem CAIStateGroup_StealStealableItemLayout
#include "engine/CAIStateGroup_StealStealableItem.h"
#undef CAIStateGroup_StealStealableItem

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_StealStealableItem : public CAIStateGroup_StealStealableItemLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_StealStealableItem::GetName()
{
    return CCharString((const char*)0x0128A884, -1);   // "SG_STEAL_STEALABLE_ITEM"
}
