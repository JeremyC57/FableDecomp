// CMiniMapDisplay::DrawMarkers(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha, bool front) -- protected
//   (Ego_r.pdb). Clears and resizes the marker primitive handles to the marker count; unless the world is loading a region or the
//   screen is faded out: (when not paused) opens the marker flash fade if inactive, updates it and closes it once fully open; then
//   draws the hero marker(s), the plain markers, then the auto markers, numbering them; if no hero marker was drawn and a region is
//   set, adds an auto marker for the living hero with the hero marker graphic (MiniMapMarkerGraphics[0]).
// Retail 0x0082B7E0, 541 bytes, `ret 0x10`, default /O2 TU (no pragma, no extra flags).
// Callees: vector<CEnginePrimitiveHandle>::erase(first,last) 0x00440D70, vector<CEnginePrimitiveHandle>::resize(n) 0x00443F90,
//   CFadeInFadeOutBase::Update 0x00434870 (Open 0x00434840 inline here), CPlayerGui::PeekGuiDef 0x00438570,
//   CMiniMapDisplay::DrawMarker 0x0082A570, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CMiniMapDisplay::AddAutoMarker 0x0082A500. Global CMainGameComponent::PMainGameComponent 0x013B86A0.
// Shape: CMarkerInfo::IsHeroMarker (out-of-line copy 0x0082A210) is inline in the loops, its gui def bound to a const reference
//   local (that spelling folds [def+0x958]; the tail's direct `PeekGuiDef().MiniMapMarkerGraphics[0]` leaves retail's dead
//   `add eax,958h`). Each loop binds `const CMarkerInfo& marker = *it` (retail `lea ebp,[ebx+8]`).
// Headers: overlay opt-ins FABLE_RETAIL_CMINIMAPDISPLAY_DRAWMARKERS, FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN,
//   FABLE_RETAIL_CPLAYERGUIDEF_MINIMAPMARKERGRAPHICS (see hdr/README.md).
// Leaf: ?DrawMarkers@CMiniMapDisplay@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC2DBoxF@@E_N@Z
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_DRAWMARKERS
#define FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CPLAYERGUIDEF_MINIMAPMARKERGRAPHICS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CPLAYERMANAGER
#include "fable_string.h"
#include "engine/CMiniMapDisplay.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"

inline bool CMiniMapDisplay::CMarkerInfo::IsHeroMarker() const
{
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    return GraphicIndex == def.MiniMapMarkerGraphics[0];
}

void CMiniMapDisplay::DrawMarkers(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha, bool front)
{
    MarkerPrimitiveHandles.clear();
    MarkerPrimitiveHandles.resize(Markers.size());
    CWorld& world = CMainGameComponent::Get()->GetWorld();
    if (!world.IsPerformingRegionLoad() && !CMainGameComponent::Get()->PeekDisplayEngine().IsScreenFadeOut()) {
        if (!world.IsPaused()) {
            if (!MarkerFlashFade.Active)
                MarkerFlashFade.Open(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
            MarkerFlashFade.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
            if (MarkerFlashFade.IsOpen())
                MarkerFlashFade.Close();
        }
        long index = 0;
        bool hero_drawn = false;
        std::list<CMarkerInfo>::const_iterator it;
        for (it = Markers.begin(); it != Markers.end(); ++it) {
            const CMarkerInfo& marker = *it;
            if (marker.IsHeroMarker()) {
                DrawMarker(info, marker, index, box, alpha, front);
                ++index;
                hero_drawn = true;
            }
        }
        for (it = Markers.begin(); it != Markers.end(); ++it) {
            const CMarkerInfo& marker = *it;
            if (!marker.IsHeroMarker() && !marker.AutoMarker) {
                DrawMarker(info, marker, index, box, alpha, front);
                ++index;
            }
        }
        for (it = Markers.begin(); it != Markers.end(); ++it) {
            const CMarkerInfo& marker = *it;
            if (!marker.IsHeroMarker() && marker.AutoMarker) {
                DrawMarker(info, marker, index, box, alpha, front);
                ++index;
            }
        }
        if (!hero_drawn && PRegion) {
            CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (hero && !hero->IsDead())
                AddAutoMarker(*hero, CPlayerGui::PeekGuiDef().MiniMapMarkerGraphics[0]);
        }
    }
}