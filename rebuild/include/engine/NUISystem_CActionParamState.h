#ifndef FABLE_ENGINE_NUISYSTEM_CACTIONPARAMSTATE_H
#define FABLE_ENGINE_NUISYSTEM_CACTIONPARAMSTATE_H
#ifndef FABLE_RETAIL_CACTIONPARAMSTATE
#error "engine/NUISystem_CActionParamState.h is a retail model: define FABLE_RETAIL_CACTIONPARAMSTATE"
#endif
// Retail model (new, chunk 867, row 0x005D6748 CTCInventoryStats::ConstructExperienceList): NUISystem::CActionParamState, the
// UI-state action parameter (the state id a CHANGESTATE action switches its component to), the twin of
// engine/NUISystem_CActionParamSLONG.h. FableWin.pdb ??0CActionParamState@NUISystem@@QAE@K@Z (inline in retail: 0x005D687C stores
// ActionParamType 2 = UI_ACTION_PARAM_TYPE_STATE at +4, vptr 0x012366EC (rebuild/manifest/rtti-vtables.tsv
// ??_7CActionParamState@NUISystem@@6B@) and the value at +8 after `new(0xc)`), ??1CActionParamState@NUISystem@@UAE@XZ (the only
// virtual; out of line). Value +8 (Ego_r.pdb struct_layouts_egor.tsv: CActionParamState 12 bytes), sizeof 0xC (operator new size).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
class CActionParamState : public CActionParam {
public:
    CActionParamState(unsigned long state) : CActionParam(UI_ACTION_PARAM_TYPE_STATE), State(state) { }
    virtual ~CActionParamState();
    unsigned long State;                                              // +0x08
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CActionParamState) == 0xc);
#endif
