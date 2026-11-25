#if !defined(FABLE_ENGINE_NPLAYERGUI_CSTATBASE_H) && !defined(FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS)
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
#if defined(FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS) && !defined(FABLE_ENGINE_NPLAYERGUI_CSTATBASE_NC_H)
#define FABLE_ENGINE_NPLAYERGUI_CSTATBASE_NC_H
// Add-only per-TU opt-in (NF451, 2026-11-25; rows 0x00443190..0x00444E50, the callers of ~CStatBase 0x00442280): the class without
// its two STLport vector members (PrimitiveHandles +0x30, Effects +0x3C), so a TU that only names the class or calls its out-of-line
// destructor does not include STLport (stlport/fable_containers.h adds /DEFAULTLIB:"libcp" to the object's .drectve, which the
// callers' objects never had). Same members, names and order up to +0x30 as the full class above (the vectors are its tail); sizeof
// is 0x30 here, not 72, so a TU that lays out a derived class pads past the two vectors itself. Needs EPlayerGuiMode: define
// FABLE_RETAIL_CPLAYERGUI and FABLE_RETAIL_CPLAYERGUI_GUIMODE (CPlayerGui.h's enum-only opt-in). Declarations only; the destructor
// stays out of line. It sits below the full view, whose guard line also tests the macro, so the full view keeps its line numbers.
#ifdef FABLE_ENGINE_NPLAYERGUI_CSTATBASE_H
#error "FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS cannot be combined with the full NPlayerGui::CStatBase view"
#endif
#include "fable_compiler.h"
#include "engine/CPlayerGui.h"
class CPlayerGuiDef; class CEnginePrimitivePrepareInfo; class CGraphicDataBank; class C2DVector; class CRGBColour;
namespace NPlayerGui {
class CStatBase {
public:
 const CPlayerGuiDef& GuiDef;
 bool ValueInitialised;
 float CurrentValue,OldValue,LastOldValue,RecentMaxValue,OldRecentMaxValue;
 unsigned long FrameCurrentValueLastMoved,FrameFullyOpenedOn;
 EPlayerGuiMode GuiMode;
 float FadeInTimer;
 CStatBase(const CPlayerGuiDef&); CStatBase(const CStatBase&); ~CStatBase();
 virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,const C2DVector&,const CRGBColour&,unsigned char,bool)=0;
 virtual void ClearPrimitiveHandles()=0;
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CStatBase)==0x30);
#endif // FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS
