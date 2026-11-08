#ifndef FABLE_ENGINE_NUISYSTEM_CACTIONPARAMEBOOL_H
#define FABLE_ENGINE_NUISYSTEM_CACTIONPARAMEBOOL_H
#ifndef FABLE_RETAIL_CACTIONPARAMEBOOL
#error "engine/NUISystem_CActionParamEBOOL.h is a retail model: define FABLE_RETAIL_CACTIONPARAMEBOOL"
#endif
// Retail model (new, chunk 880, row 0x0058AF10 CTCInventory::OpenPCOptionsMenu): NUISystem::CActionParamEBOOL, the boolean UI action
// parameter, the twin of engine/NUISystem_CActionParamULONG.h. FableWin.pdb ??0CActionParamEBOOL@NUISystem@@QAE@_N@Z (inline in
// retail: 0x0058C8B3 stores ActionParamType 8 = UI_ACTION_PARAM_TYPE_EBOOL at +4, vptr 0x012366FC (rebuild/manifest/rtti-vtables.tsv
// ??_7CActionParamEBOOL@NUISystem@@6B@) and the byte value at +8 after `new(0xc)`), ??1CActionParamEBOOL@NUISystem@@UAE@XZ (the only
// virtual; out of line). sizeof 0xC (operator new size).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
class CActionParamEBOOL : public CActionParam {
public:
    CActionParamEBOOL(bool value) : CActionParam(UI_ACTION_PARAM_TYPE_EBOOL), Value(value) { }
    virtual ~CActionParamEBOOL();
    bool Value;                                                       // +0x08
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CActionParamEBOOL) == 0xc);
#endif
