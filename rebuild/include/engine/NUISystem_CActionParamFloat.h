#ifndef FABLE_ENGINE_NUISYSTEM_CACTIONPARAMFLOAT_H
#define FABLE_ENGINE_NUISYSTEM_CACTIONPARAMFLOAT_H
// Retail model (new, chunk 752): NUISystem::CActionParamFloat, the float UI action parameter (the twin of the chunk-735
// CActionParamSLONG model). Ego_r.pdb ??0CActionParamFloat@NUISystem@@QAE@M@Z, ??1CActionParamFloat@NUISystem@@UAE@XZ,
// ??_GCActionParamFloat@NUISystem@@UAEPAXI@Z; ActionParamType UI_ACTION_PARAM_TYPE_FLOAT (6). Value +8: NUISystem::CSlider's
// ProcessEvent 0x0054AC90 / ~CSlider 0x0054B360 store the slider's float Value.X at the first UI parameter's +8.
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
class CActionParamFloat : public CActionParam {
public:
    CActionParamFloat(float value) : CActionParam(UI_ACTION_PARAM_TYPE_FLOAT), Value(value) { }
    virtual ~CActionParamFloat();
    float Value;                                                      // +0x08
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CActionParamFloat) == 0xc);
#endif
