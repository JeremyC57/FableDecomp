// Prepare the persistent primitives used to render the active tavern game.
// Leaf: ?DrawPreparePrimitivesForRendering@CTCTavernGame@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_DRAW_PASS27
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CARRAY_PUSHBACK_DEFAULT_DECL
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#include "fable_string.h"
#include "fable_assert.h"
#include "engine/CThing.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTavernGameDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CFontManager.h"
#include "engine/CFontBank.h"
#include "engine/GFGetTime.h"

extern CDisplayManager* GPDisplayManager;

bool CTCTavernGame::IsCurrentOrUpcomingState(ETavernGameStates state)
{
    return TavernGameState == state || YesState == state || AltState == state;
}

void CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    CDefPointer<const CTavernGameDef> pdef;
    GetDef(&pdef);
    Handles.clear();
    float time_step = (float)(GFGetTime() - LastRenderTime);
    LastRenderTime = GFGetTime();
    if (IsBeingPlayed) {
        CWideString current_score = GetCurrentScore();
        CWideString best_score = GetBestScore();
        CWideString required_score = GetRequiredScore();
        CWideString additional_info = GetAdditionalInfo();
        CWideString bet = GetBet();
        CWideString players_money = GetPlayersMoney();
        CWideString total_winnings = GetTotalWinnings();
        if (!(IsCurrentOrUpcomingState(ETGS_GREETING) || IsCurrentOrUpcomingState(ETGS_OPTIONS) ||
              IsCurrentOrUpcomingState(ETGS_INSTRUCTIONS) || IsCurrentOrUpcomingState(ETGS_FAREWELL) ||
              IsCurrentOrUpcomingState(ETGS_LEAVE) || IsCurrentOrUpcomingState(ETGS_GAME)) ||
            PreviousTavernGameState == ETGS_UPDATE_VALUES) {
            bool betting = TavernGameState == ETGS_WAIT_FOR_YES_NO && YesState == ETGS_GAME;
            RenderText(info, pdef->ScoreX, pdef->ScoreY, current_score, *PScoreFont, CRGBColour(255, 255, 255, 255));
            RenderText(info, pdef->TargetX, pdef->TargetY, required_score, *PTargetFont, CRGBColour(255, 255, 255, 255));
            RenderText(info, pdef->BestX, pdef->BestY, best_score, *PStatsFont, CRGBColour(255, 255, 255, 255));
            if (!betting)
                RenderText(info, pdef->AdditionalX, pdef->AdditionalY, additional_info, *PStatsFont, CRGBColour(255, 255, 255, 255));
            if (pdef->DisplayErrata || GetGameState() != ETGS_CARDS_MAIN_GAME)
                DrawBetAndMoney(info, pdef->BetX, pdef->BetY, pdef->MoneyX, pdef->MoneyY, pdef->WinningsX, pdef->WinningsY);
            else
                ClearBetAndMoney();
            DrawBox(info);
        } else {
            if (YesState == ETGS_GAME) {
                DrawBetAndMoney(info, pdef->MainBetX, pdef->MainBetY, pdef->MainMoneyX, pdef->MainMoneyY, 0.0f, 0.0f);
                ClearBox();
            } else {
                ClearBetAndMoney();
                ClearBox();
            }
        }
        if (GetGameState() == ETGS_WAIT_FOR_YES_NO && YesState == ETGS_QUIT_MID_GAME) {
            C2DExtentsI extents = GPDisplayManager->GetRenderTargetDimensions();
            C2DBoxF clip_box(0.0f, 0.0f, (float)extents.X, (float)extents.Y);
            CEnginePrimitive2DSprite background_primitive(0, clip_box, ENGINE_2D_LAYER_6, 0, 0,
                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, 255, 0, 0, 0, 0,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            Handles.push_back();
            info.Engine.SetPersistentPrimitive(Handles.back(), background_primitive, 0x40);
        }
        float boundary = PFontManager->GetFont("ENG_ARIAL_16")->GetFontMaxHeight();
        float dist_between_boxes = (float)(boundary * 4.0);
        float base_dist = (float)(boundary * 6.0);
        CWideString game_state = GetWinPhrase();
        game_state != L"";
        GetGameState();
        float control_box_y = (float)(480.0 - base_dist);
        switch (GetGameState()) {
        case ETGS_WAIT_FOR_CLICK:
            FABLE_RETAIL_ASSERT(0);
            break;
        }
    }
}