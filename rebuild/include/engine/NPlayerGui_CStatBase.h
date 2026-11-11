#ifndef FABLE_ENGINE_NPLAYERGUI_CSTATBASE_H
#define FABLE_ENGINE_NPLAYERGUI_CSTATBASE_H
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CEnginePrimitivePrepareInfo; class CGraphicDataBank; class C2DVector; class CRGBColour;
namespace NParticleEngine { class CParticleEmitter; }
namespace NPlayerGui {
// Layout: both PDB CStatBase records; two native STLport vectors give size72.
// Native0064619f reads GuiDef+4 and PrimitiveHandles+48.
class CStatBase {
public:
 const CPlayerGuiDef& GuiDef;
 bool ValueInitialised;
 float CurrentValue,OldValue,LastOldValue,RecentMaxValue,OldRecentMaxValue;
 unsigned long FrameCurrentValueLastMoved,FrameFullyOpenedOn;
 EPlayerGuiMode GuiMode;
 float FadeInTimer;
 std::vector<CEnginePrimitiveHandle> PrimitiveHandles;
 std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;
 CStatBase(const CPlayerGuiDef&); CStatBase(const CStatBase&); ~CStatBase();
 virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,const C2DVector&,const CRGBColour&,unsigned char,bool)=0;
 virtual void ClearPrimitiveHandles()=0;
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CStatBase)==72);
#endif
