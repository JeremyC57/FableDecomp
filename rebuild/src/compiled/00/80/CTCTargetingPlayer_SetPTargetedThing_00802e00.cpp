// CTCTargetingPlayer::SetPTargetedThing()
// Leaf: ?SetPTargetedThing@CTCTargetingPlayer@@UAEXPAVCThing@@@Z
// Flags: default
#define FABLE_RETAIL_TARGETING_PLAYER_SET_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCOFLEAFTYPE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCACTIONUSEREADABLE_PASS24
#define FABLE_RETAIL_CREADABLEDEF_USEINFO_PASS24
#define FABLE_RETAIL_CTCHASNAME_NPCSHORTNAME
#define FABLE_RETAIL_CTCREGIONDISPLAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_TOGGLE_ZTARGET
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_NPCNAMEDISPLAY
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#include "engine/CThing.h"
#include "engine/CTargetingDef.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTCActionUseReadable.h"
#include "engine/CTCRegionDisplay.h"
#include "engine/CTCHasName.h"
#include "engine/ThingAlive.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CMainGameComponent.h"
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"

void CTCTargetingPlayer::SetPTargetedThing(CThing* pthing)
{
    if (CMainGameComponent::Get()->GetPPlayerInterface()->IsToggleZTarget()) {
        if (!pthing || pthing != GetPTargetLockedThing()) {
            if (GetParentThing().GetPlayer().HasMode(PLAYER_MODE_Z_TARGET))
                GetParentThing().GetPlayer().RemoveMode(PLAYER_MODE_Z_TARGET);
        }
    }

    if (pthing != PTargetedThing.Get()) {
        if (IS_THING_ALIVE(PTargetedThing)) {
            const CTCActionUseReadable* ptc_readable;
            if (PTargetedThing->GetTCOfLeafType(&ptc_readable)) {
                if (ptc_readable->PeekDef().UseInfoDisplay) {
                    CTCRegionDisplay* ptc_region_display;
                    GetParentThing().GetTC(&ptc_region_display);
                    if (ptc_region_display)
                        ptc_region_display->SetCurrentAreaName("", -1.0f, false, false, true);
                }
            }
        }

        if (pthing != GetPTargetLockedThing()) {
            PTargetLockedThing = CIntelligentPointer<CThing>(0);
            LockedOntoEnemy = false;
        }
        CTCTargetingBase::SetPTargetedThing(pthing);

        if (IS_THING_ALIVE(PTargetedThing)) {
            const CTCActionUseReadable* ptc_readable;
            if (PTargetedThing->GetTCOfLeafType(&ptc_readable)) {
                if (ptc_readable->PeekDef().UseInfoDisplay) {
                    CTCRegionDisplay* ptc_region_display;
                    GetParentThing().GetTC(&ptc_region_display);
                    if (ptc_region_display)
                        ptc_region_display->SetCurrentAreaName(ptc_readable->GetTextTag(), 0.0f, true, false, true);
                }
            }
        }

        if (IS_THING_ALIVE(PTargetedThing) && !CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()) {
            CTCHasName* ptc_has_name;
            if (PTargetedThing->GetTC(&ptc_has_name))
                CPlayerGui::Get().SetNPCNameDisplay(ptc_has_name->GetNPCNameAndFullTitle());
            else
                CPlayerGui::Get().SetNPCNameDisplay(CWideString());
        } else {
            CPlayerGui::Get().SetNPCNameDisplay(CWideString());
        }
    }
}