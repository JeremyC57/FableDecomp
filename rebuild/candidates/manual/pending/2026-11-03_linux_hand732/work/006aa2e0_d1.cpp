// CThingPlayerCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- hero draw pass
// Leaf: ?DrawPreparePrimitivesForRendering@CThingPlayerCreature@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CRGBCOLOUR
#include "fable_string.h"
#include "fable_game.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefString.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveText.h"
#include "engine/CIEngine.h"
#include "engine/CRGBColour.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

extern long g_HeroCreationState;                         // 0x0137F940 (established spelling: OnCreate 0x006A9EF0)
extern bool g_DebugDrawHeroNavigationLevel;              // 0x013B8640 descriptive (a console bool, setter 0x0049EF10)
extern bool g_DebugDrawStealth;                          // 0x013B8A80 descriptive (a console bool, setter 0x0051E4A0)
CWideString __fastcall GFIntToWideString(long value);    // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

bool CThingPlayerCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (!g_HeroCreationState)
        return false;
    if (!CThingCreatureBase::DrawPreparePrimitivesForRendering(info))
        return false;
    if (HasTC(TCI_COMBAT))
        GetTC(TCI_COMBAT).DrawPreparePrimitivesForRendering(info);
    if (HasTC(TCI_SPECIAL_ABILITIES))
        GetTC(TCI_SPECIAL_ABILITIES).DrawPreparePrimitivesForRendering(info);
    if (HasTC(TCI_HERO_EXPERIENCE))
        GetTC(TCI_HERO_EXPERIENCE).DrawPreparePrimitivesForRendering(info);
    if (HasTC(TCI_HERO_RECEIVE_ITEMS))
        GetTC(TCI_HERO_RECEIVE_ITEMS).DrawPreparePrimitivesForRendering(info);
    if (HasTC(TCI_BOAST_UI))
        GetTC(TCI_BOAST_UI).DrawPreparePrimitivesForRendering(info);
    if (HasTC(TCI_CREDITS_UI))
        GetTC(TCI_CREDITS_UI).DrawPreparePrimitivesForRendering(info);
    if (g_DebugDrawHeroNavigationLevel) {
        C3DVector pos = PTCGraphicAppearance->GetFocalPos(*(const float*)&info.Interpolate[4]);
        pos.Z += 2.0f;
        CWideString text = GFIntToWideString(GetNavigationLevelAt(GetPos()));
        info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), pos, 1.0f, text, PeekComponent().PeekDebugFont(),
                                                       CRGBColour(255, 255, 0, 255), true, (EEngineInterpolationSet)0));
        CTCVolumeContainmentTracker* tracker;
        if (GetTC(&tracker) && tracker->GetPThingInside() && !tracker->GetPThingInside()->IsDead()
            && !tracker->GetPThingInside()->GetPBaseDef().IsNull()) {
            pos.Z += 0.5f;
            CWideString name = tracker->GetPThingInside()->GetPBaseDef()->GetInstantiationName().ToWideString();
            info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), pos, 0.08f, name, PeekComponent().PeekDebugFont(),
                                                           CRGBColour(255, 255, 0, 255), true, (EEngineInterpolationSet)0));
        }
    }
    if (g_DebugDrawStealth && HasTC(TCI_STEALTH))
        GetTC(TCI_STEALTH).DrawPreparePrimitivesForRendering(info);
    if (HasTC(TCI_HERO_STATS))
        GetTC(TCI_HERO_STATS).DrawPreparePrimitivesForRendering(info);
    if (HasTC(TCI_BETTING))
        GetTC(TCI_BETTING).DrawPreparePrimitivesForRendering(info);
    return true;
}
