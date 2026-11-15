#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCESPRITE_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCESPRITE_H
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCESPRITE_PASS22
// Retail opt-in (Codex pass22): both original PDBs and native primary/component bases+0/+0x0c, size0x30.
#include "engine/CTCBase.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
namespace NGraphicAppearance {
class CPrimitive;
class CTCGraphicAppearanceSprite:public CTCBase,public CAGraphicAppearanceComponent {
public:
 virtual bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&);
protected:
 float GetInterpolatedAnimStage(float)const;
 float RotationAmountPerFrame,RotationAngle,OldRotationAngle,AnimStage,LastAnimStage,AnimSpeed;
 bool FadeOutWithAnim;
 CPrimitive* PrimHandle;
};
FABLE_STATIC_ASSERT(sizeof(CTCGraphicAppearanceSprite)==0x30);
}
#endif
#endif
