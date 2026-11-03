#ifndef FABLE_ENGINE_NUISYSTEM_CACTIONPARAMSLONG_H
#define FABLE_ENGINE_NUISYSTEM_CACTIONPARAMSLONG_H
#ifndef FABLE_RETAIL_CACTIONPARAMSLONG
#error "engine/NUISystem_CActionParamSLONG.h is a retail model: define FABLE_RETAIL_CACTIONPARAMSLONG"
#endif
// Retail model (new, chunk 735): NUISystem::CActionParamSLONG, the signed-long UI action parameter. Ego_r.pdb / FableWin.pdb
// ??0CActionParamSLONG@NUISystem@@QAE@J@Z (inline in retail: CTCInventoryExperience::AddStatToList 0x005729D8 stores vptr
// 0x012366F4, ActionParamType 3 = UI_ACTION_PARAM_TYPE_SLONG at +4 and the value at +8 after `new(0xc)`),
// ??1CActionParamSLONG@NUISystem@@UAE@XZ (the only virtual; out of line). Value +8 (PDB), sizeof 0xC (operator new size).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
class CActionParamSLONG : public CActionParam {
public:
    CActionParamSLONG(long value) : CActionParam(UI_ACTION_PARAM_TYPE_SLONG), Value(value) { }
    virtual ~CActionParamSLONG();
    long Value;                                                       // +0x08
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CActionParamSLONG) == 0xc);
#endif
