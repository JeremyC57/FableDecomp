#ifndef FABLE_ENGINE_NUISYSTEM_CACTIONPARAMULONG_H
#define FABLE_ENGINE_NUISYSTEM_CACTIONPARAMULONG_H
#ifndef FABLE_RETAIL_CACTIONPARAMULONG
#error "engine/NUISystem_CActionParamULONG.h is a retail model: define FABLE_RETAIL_CACTIONPARAMULONG"
#endif
// Retail model (new, chunk 867, row 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList): NUISystem::CActionParamULONG,
// the unsigned-long UI action parameter, the twin of engine/NUISystem_CActionParamSLONG.h. Ego_r.pdb / FableWin.pdb
// ??0CActionParamULONG@NUISystem@@QAE@K@Z (inline in retail: 0x005F5CEA stores ActionParamType 5 = UI_ACTION_PARAM_TYPE_ULONG at
// +4, vptr 0x0122F844 (rebuild/manifest/rtti-vtables.tsv ??_7CActionParamULONG@NUISystem@@6B@) and the value at +8 after
// `new(0xc)`), ??1CActionParamULONG@NUISystem@@UAE@XZ (the only virtual; out of line). Value +8 (Ego_r.pdb struct_layouts_egor.tsv:
// CActionParamULONG 12 bytes, `ulong Value` at 8), sizeof 0xC (operator new size).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
class CActionParamULONG : public CActionParam {
public:
#ifdef FABLE_RETAIL_CACTIONPARAMULONG_BODY_ASSIGN
    // Retail opt-in (row 0x00601310): the value stored in the constructor body (same code; more inline IL per site).
    CActionParamULONG(unsigned long value) : CActionParam(UI_ACTION_PARAM_TYPE_ULONG) { Value = value; }
#else
    CActionParamULONG(unsigned long value) : CActionParam(UI_ACTION_PARAM_TYPE_ULONG), Value(value) { }
#endif
    virtual ~CActionParamULONG();
    unsigned long Value;                                              // +0x08
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CActionParamULONG) == 0xc);
#endif
