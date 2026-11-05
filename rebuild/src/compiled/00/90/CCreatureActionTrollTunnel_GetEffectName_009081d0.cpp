// Leaf: _CCreatureAction_TrollTunnel_GetEffectName@4
#include "engine/CCreatureAction_TrollTunnel.h"
#include "rebuild_abi.h"

struct FableEffectResult_009081d0 { void* ctor(const char* text); };
extern "C" char FABLE_STDCALL CCreatureAction_TrollTunnel_GetEffectName(FableEffectResult_009081d0* result)
{
    result->ctor((const char*)19456924);
    return 1;
}
// Stand-in (owner rule 2026-11-05): CCreatureAction_TrollTunnel_GetEffectName for CCreatureAction_TrollTunnel::GetEffectName; RTTI vtable slot ?GetEffectName@CCreatureAction_TrollTunnel@@MBE_NAAVCCharString@@@Z + PDB name set (fablewin).
