#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVE_PUBLIC
#define FABLE_STLPORT_DEQUE_PUSH_BACK_AUX_NOINLINE
// Inline-budget: the native deque growth function stays out of line at 004127e0, with its STLport body visible.
// Leaf: ?GiveTutorialForCategory@CUserProfileManager@@QAE_NW4ETutorialCategory@@_N@Z
// Flags: /Gr
#define FABLE_RETAIL_TUTORIAL_GIVE_PASS29
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_DEFAULTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_STLP_QUEUE
#define FABLE_STLPORT_DEQUE_PUSH_BACK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include <string.h>
#include "fable_user_profile.h"
#include "stlport/fable_algorithm.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CHeroLogBook.h"
namespace NGlobalConsole { extern bool GEnableTutorials; }

inline bool CUserProfileManager::IsTutorialSystemEnabled() const
{
    return UseNewProfileValues ? TutorialSystemEnabledNewProfile : TutorialSystemEnabled;
}

inline bool CUserProfileManager::IsTutorialCategoryGiven(ETutorialCategory category) const
{
    return std::find(GivenTutorialCategories.begin(), GivenTutorialCategories.end(), category) != GivenTutorialCategories.end();
}

bool CUserProfileManager::GiveTutorialForCategory(ETutorialCategory category, bool ignore_fading)
{
    if (NGlobalConsole::GEnableTutorials && IsTutorialSystemEnabled() && !IsTutorialCategoryGiven(category)) {
        CMainGameComponent* pcomponent = CMainGameComponent::Get();
        const CWorld& world = pcomponent->PeekWorld();
        if ((!world.GetGameScriptInterface().IsInMovieSequence() || DisplayTutorialsDuringCutscenes)
            && !CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()
            && CPlayerGui::Get().GetSubtitleRenderer().IsAllowedToSetText(ignore_fading)) {
            const CPlayerGuiDef& gui_def = CPlayerGui::Get().PeekGuiDef();
            const CCharString& tutorial_text_tag = gui_def.TutorialText[category];
            if (tutorial_text_tag != "") {
                long guild_seal_graphic = gui_def.TutorialGuildSealGraphics[category];
                if (guild_seal_graphic > 0)
                    CPlayerGui::Get().AddGuildSealMessage(tutorial_text_tag, guild_seal_graphic, true);
                else
                    CPlayerGui::Get().GetSubtitleRenderer().SetText(pcomponent->PeekCutSceneMainFont(),
                        tutorial_text_tag, "", "", "", "", 10, 0.0f, true, true);
            }
            CCharString log_book_tag = gui_def.TutorialLogBookText[category];
            if (log_book_tag != "") {
                CCharString log_book_title_tag = log_book_tag;
                if (log_book_title_tag.EndsWith("_PC"))
                    log_book_title_tag = log_book_title_tag.GetSubString(0, log_book_title_tag.GetLength()-3) + "_TITLE";
                else
                    log_book_title_tag += "_TITLE";
                CPlayer& player = pcomponent->GetPlayerManager().GetMainPlayer();
                CHeroLogBook& log_book = player.GetPlayerHeroLogBook();
                log_book.AddEntry(log_book_title_tag.ToWideString(), log_book_title_tag.ToWideString(),
                    log_book_tag.ToWideString(), CHeroLogBook::CHeroLogBookEntry::CATEGORY_TUTORIAL, true);
            }
            GivenTutorialCategories.push_back(category);
            Save();
            return true;
        }
        PendingTutorialCategories.push(category);
    }
    return false;
}