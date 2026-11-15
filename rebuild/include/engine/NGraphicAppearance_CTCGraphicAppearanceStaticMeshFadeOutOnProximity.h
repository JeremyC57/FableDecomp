#ifndef FABLE_ENGINE_CTCGRAPHICAPPEARANCESTATICMESHFADEOUTONPROXIMITY_H
#define FABLE_ENGINE_CTCGRAPHICAPPEARANCESTATICMESHFADEOUTONPROXIMITY_H
#ifdef FABLE_RETAIL_CTCSMFADE_PASS22
#include "engine/CTCBase.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
namespace NGraphicAppearance {
class CTCGraphicAppearance;class CGraphicAppearanceDrawInfo;class CPrimitive;
class CTCGraphicAppearanceStaticMeshFadeOutOnProximity:public CTCBase,public CAGraphicAppearanceComponent {
public:
    CPrimitive* PrimHandle;
    virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&);
};
}
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity)==20);
#endif
#endif
// Retail opt-in (Codex pass22): Original Ego_r/FableWin size20,CTCBase@0,component@12,PrimHandle@16; native0077BEC0 secondary draw.
