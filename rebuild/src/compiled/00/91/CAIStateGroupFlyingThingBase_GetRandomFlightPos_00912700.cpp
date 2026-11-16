// CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector&, float, C3DVector&, bool) const -- find a same-region wander position with the creature's navigator switched to the flying type, then raise it above the local ground and the theme height.
// Leaf: ?GetRandomFlightPos@CAIStateGroup_FlyingThingBase@@IBE_NABVC3DVector@@MAAV2@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00912700, 309 bytes, thiscall `ret 10h`; protected const (Ego_r.pdb 0x00936E50 + FableWin.pdb
//   0x02B8A0E0 ?GetRandomFlightPos@CAIStateGroup_FlyingThingBase@@IBE_NABVC3DVector@@MAAV2@_N@Z, reference out-parameter). The final
//   bool is unused by retail.
// Callees: CAIStateGroupBase::PeekWorldMap 0x008FCFF0, CWorldMap::GetMapNumberAt (virtual slot 16), CThing::GetPos 0x004C73D0,
//   CTCCreatureNavigation::GetNavigatorType 0x006CB6A0 / SetNavigatorToType 0x006CBA80 / GetNearbyWanderPosition 0x006CB6E0,
//   CThingCreatureBase::GetHeightZBelowAt 0x00666D40, CWorldMap::GetThemeSizeZAt 0x0063A390; FlyingNavigatorType 0x0129CB44,
//   HeightProbeOffset 0x0138C500.
// NF41 (2026-11-16): moved off its FlyingMethods / CNavigationMethods / CThingRetail / CWorldMapMethods stand-ins onto the shared
//   engine/CAIStateGroup_FlyingThingBase.h (the include chain of the landed sibling 0x00912840) plus engine/CTCCreatureNavigation.h
//   (opt-in FABLE_RETAIL_CTCCREATURENAVIGATION_SETNAVIGATORTOTYPE) and engine/CWorldMap.h (GetThemeSizeZAt).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINESEGMENT
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_SETNAVIGATORTOTYPE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorldMap.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CAIStateGroup_FlyingThingBase.h"
extern unsigned long FlyingNavigatorType;          // 0x0129CB44
extern const C3DVector HeightProbeOffset;          // 0x0138C500

bool CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector& centre, float range, C3DVector& out, bool allowGround) const
{
    if (PeekWorldMap().GetMapNumberAt(centre) != PeekWorldMap().GetMapNumberAt(GetCreature().GetPos()))
        return false;
    CTCCreatureNavigation* nav;
    GetCreature().GetTC(TCI_NAVIGATION, &nav);
    unsigned long oldType = nav->GetNavigatorType();
    nav->SetNavigatorToType(FlyingNavigatorType);
    if (!nav->GetNearbyWanderPosition(centre, range, out, true))
        return false;
    nav->SetNavigatorToType(oldType);
    out.Z = GetCreature().GetHeightZBelowAt(HeightProbeOffset + out, true, true) + MinHeightToKeepAboveGround;
    float ground = PeekWorldMap().GetThemeSizeZAt(out);
    if (ground > out.Z)
        out.Z = ground;
    return true;
}
