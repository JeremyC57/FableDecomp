#include "rebuild_abi.h"
// std::_Dest_val (variant) @ 0x008d6a30
// push esi; mov esi,ecx; call h1; mov ecx,[esi+4]; xor dl,dl; pop esi; jmp h2
struct FableDestValV_008d6a30_t
{
    void* field_0;   // +0x00
    void* field_4;   // +0x04
};
struct FableDestValV_008d6a30_inner
{
    void FABLE_FASTCALL h2(char flag);
};
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x008621A0 ?OnActivate@CModeStandBase@@UAEXXZ (was ?h1@FableDestValV_008d6a30_t@@QAIXXZ)
class CModeStandBase { public: virtual void OnActivate(); };
extern "C" void FABLE_FASTCALL FableDestValV_008d6a30(FableDestValV_008d6a30_t* mode)
{
    reinterpret_cast<CModeStandBase*>(mode)->CModeStandBase::OnActivate();
    ((FableDestValV_008d6a30_inner*)mode->field_4)->h2(0);
}