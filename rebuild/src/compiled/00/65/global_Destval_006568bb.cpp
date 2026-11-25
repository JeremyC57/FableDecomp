#include "rebuild_abi.h"
// _Dest_val<...> @ 0x006568bb
// push esi; mov esi,ecx; lea ecx,[esi+0x20]; call member_dtor; lea ecx,[esi+0x10]; pop esi; jmp base_dtor
// Destroy the embedded member at +0x20, then tail-jump the base destructor at +0x10.
struct FableMember_006568bb; struct FableBase_006568bb;
extern "C" void FABLE_FASTCALL FableBaseDtor_006568bb(FableBase_006568bb*);
// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x0099B510 ??1CWideString@@QAE@XZ (was @FableMemberDtor_006568bb@4)
class CWideString { public: ~CWideString(); };
extern "C" void FABLE_FASTCALL FableDualDestval_006568bb(char *self)
{
    reinterpret_cast<CWideString*>((FableMember_006568bb*)(self + 0x20))->CWideString::~CWideString();
    FableBaseDtor_006568bb((FableBase_006568bb*)(self + 0x10));
}