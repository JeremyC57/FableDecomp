// CAIStateGroup_HoverAttackArea::GetName() -- static: the AI state group name "SG_HOVER_ATTACK_AREA" as a CCharString
// Leaf: ?GetName@CAIStateGroup_HoverAttackArea@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE1B6: push esi; push -1; push "SG_HOVER_ATTACK_AREA"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220984 registers this GetName with Alloc 0x9029ea (pushed before it); Alloc 0x9029ea stores the vtable of CAIStateGroup_HoverAttackArea into the new object; literal "SG_HOVER_ATTACK_AREA"; PDB name set ?GetName@CAIStateGroup_HoverAttackArea@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_HoverAttackArea@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe1b6::build).
#define CAIStateGroup_HoverAttackArea CAIStateGroup_HoverAttackAreaLayout
#include "engine/CAIStateGroup_HoverAttackArea.h"
#undef CAIStateGroup_HoverAttackArea

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_HoverAttackArea : public CAIStateGroup_HoverAttackAreaLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_HoverAttackArea::GetName()
{
    return CCharString((const char*)0x0128A310, -1);   // "SG_HOVER_ATTACK_AREA"
}
