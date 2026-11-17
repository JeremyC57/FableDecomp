// std::vector<NPlayerTargeting::CTargetPoint>::~vector() -- destroy the elements through the out-of-line __destroy_aux, then free the block
// Leaf: ??1?$vector@VCTargetPoint@NPlayerTargeting@@V?$allocator@VCTargetPoint@NPlayerTargeting@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1AAF, 35 bytes, thiscall (size-optimised TU: `pop ecx` after free).
// Retail shape: push the tag address, edx = finish, ecx = start, call 0x004DBE58 (the __fastcall range destructor), then
//   `if (start) free(start)` (0x00BFEA14).
// Callers: CTCTargetingPlayer::~CTCTargetingPlayer 0x004E37BF (0x004E37F1 on +0x74 RejectedTargetPoints, 0x004E37F9 on +0x68 Targets;
//   Ego_r.pdb / the shared engine/CTCTargetingPlayer.h: both vector<NPlayerTargeting::CTargetPoint>).
// Name fix (NF59, queue q10543): the landed row was the stand-in free function OnReadFinished(AutoTinyActiveFile*) (catalog
//   StdList_Dtor_004e1aaf; propagated CActiveFile::OnReadFinished). Same shape as the landed ~vector<CEngineTextureMorph> 0x005BC0D3:
//   the header's STLport ~vector explicitly instantiated, the range destructor the /Gr out-of-line COMDAT (FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL,
//   declaration only), the _Destroy -> __destroy dispatch expanded (FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE). FableWin.pdb /
//   Ego_r.pdb carry the Dinkumware vector<NPlayerTargeting::CTargetPoint> members (copy ctor 0x008039F0, operator= 0x00804B50 ...).
#pragma optimize("s", on)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/NPlayerTargeting.h"

template std::vector<NPlayerTargeting::CTargetPoint>::~vector();
