// std::vector<CAttackHistoryCombo>::~vector() -- STLport 4.6 _vector.h: destroy the elements (__destroy_aux 0x004E41C0, __fastcall) and free
//   the block
// Leaf: ??1?$vector@VCAttackHistoryCombo@@V?$allocator@VCAttackHistoryCombo@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E81C1, 35 bytes, size TU.
// Identity (NF92, q11222): Ego_r.pdb CHeroExperienceDef::AttackHistoryCombos is std::vector<CAttackHistoryCombo> + FableWin.pdb 0x01BC35B3
//   ??1?$vector@VCAttackHistoryCombo@@V?$allocator@VCAttackHistoryCombo@@@std@@@std@@QAE@XZ; the ported CHeroExperienceDef destructor
//   0x004EBB1C binds it so (+0x0A) and its callee 0x004E41C0 is the landed __destroy_aux<CAttackHistoryCombo*>. Explicit instantiation of
//   stlport/fable_containers.h (the 0x00F26E20 recipe; DESTROY_DISPATCH_FORCEINLINE + DESTROY_AUX_NOINLINE_FASTCALL: the size TU expands the
//   _Destroy dispatch and calls the /Gr __destroy_aux COMDAT, as 0x005BC0D3). Was the propagated stand-in CActiveFile::OnReadFinished (a local model).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CATTACKHISTORYCOMBO
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CAttackHistoryCombo.h"
template std::vector<CAttackHistoryCombo, std::allocator<CAttackHistoryCombo> >::~vector();
