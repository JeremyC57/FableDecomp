// CAIStateGroup_HoverHornetQueenTakeDamage::GetName() -- static: the AI state group name "SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_HoverHornetQueenTakeDamage@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE1C9: push esi; push -1; push "SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12209ac registers this GetName with Alloc 0x902a0e (pushed before it); Alloc 0x902a0e stores the vtable of CAIStateGroup_HoverHornetQueenTakeDamage into the new object; literal "SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE"; PDB name set ?GetName@CAIStateGroup_HoverHornetQueenTakeDamage@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_HoverHornetQueenTakeDamage@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe1c9::build).
#define CAIStateGroup_HoverHornetQueenTakeDamage CAIStateGroup_HoverHornetQueenTakeDamageLayout
#include "engine/CAIStateGroup_HoverHornetQueenTakeDamage.h"
#undef CAIStateGroup_HoverHornetQueenTakeDamage

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_HoverHornetQueenTakeDamage : public CAIStateGroup_HoverHornetQueenTakeDamageLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_HoverHornetQueenTakeDamage::GetName()
{
    return CCharString((const char*)0x0128A328, -1);   // "SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE"
}
