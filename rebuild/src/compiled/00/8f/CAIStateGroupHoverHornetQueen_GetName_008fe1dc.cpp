// CAIStateGroup_HoverHornetQueen::GetName() -- static: the AI state group name "SG_HOVER_HORNET_QUEEN" as a CCharString
// Leaf: ?GetName@CAIStateGroup_HoverHornetQueen@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE1DC: push esi; push -1; push "SG_HOVER_HORNET_QUEEN"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12209d4 registers this GetName with Alloc 0x903351 (pushed before it); Alloc 0x903351 -> ctor 0x90310e stores (last) the vtable of CAIStateGroup_HoverHornetQueen at this+0; literal "SG_HOVER_HORNET_QUEEN"; PDB name set ?GetName@CAIStateGroup_HoverHornetQueen@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_HoverHornetQueen@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe1dc::build).
#define CAIStateGroup_HoverHornetQueen CAIStateGroup_HoverHornetQueenLayout
#include "engine/CAIStateGroup_HoverHornetQueen.h"
#undef CAIStateGroup_HoverHornetQueen

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_HoverHornetQueen : public CAIStateGroup_HoverHornetQueenLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_HoverHornetQueen::GetName()
{
    return CCharString((const char*)0x0128A34C, -1);   // "SG_HOVER_HORNET_QUEEN"
}
