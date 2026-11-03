// CThingPlayerCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- hero draw pass
// Leaf: ?DrawPreparePrimitivesForRendering@CThingPlayerCreature@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AA2E0, 1080 bytes, `ret 4`; CThingPlayerCreature vtable slot of
//   ?DrawPreparePrimitivesForRendering@CThingPlayerCreature@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z (Ego_r + FableWin).
// Floats: 0.7f @ 0x01255318, 0.2f @ 0x012305A0, 1.0f / 0.08f literals. Globals: g_HeroCreationState 0x0137F940 (established),
//   g_DebugDrawHeroNavigationLevel 0x013B8640 and g_DebugDrawStealth 0x013B8A80 (descriptive, data names not in the PDB lists).
// Grade: RELOCATION_MATCH (chunk 732, attempts d1..d6).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
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
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
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
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }   // FableWin ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z

bool CThingPlayerCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (!g_HeroCreationState)
        return false;
    if (CThingCreatureBase::DrawPreparePrimitivesForRendering(info)) {
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
        pos.Z += 0.7f;
        CWideString text = GFIntToWideString(GetNavigationLevelAt(GetPos()));
        info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), pos, 1.0f, text, PeekComponent().PeekDebugFont(),
                                                       CRGBColour(255, 255, 0, 255), true, (EEngineInterpolationSet)0));
        CTCVolumeContainmentTracker* tracker;
        if (GetTC(&tracker) && IS_THING_ALIVE(tracker->GetPThingInside())
            && !tracker->GetPThingInside()->GetPBaseDef().IsNull()) {
            pos.Z += 0.2f;
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
    return false;
}