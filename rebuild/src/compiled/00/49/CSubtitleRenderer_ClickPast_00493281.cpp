// CSubtitleRenderer::ClickPast() -- reveals or advances subtitle text on a continue input.
// Leaf: ?ClickPast@CSubtitleRenderer@@QAE_NXZ
// Flags: /O1 /Oy-
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_CLICKPAST_ORIGINAL
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXTINFO
#define FABLE_RETAIL_CSUBTITLERENDERER_CLEAR
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SUBTITLING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CUserProfileManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CASoundManager.h"
#include "engine/CSoundInit.h"

extern CASoundManager* PSoundManager;

// Descriptive: the game build plays TextBoxButtonASoundCriteria in both arms.
// Inline-budget: Both retail sound arms are expanded in the size-optimised subtitle TU.
static FABLE_FORCEINLINE void PlayTextBoxButtonASound()
{
    if (PSoundManager) {
        const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
        CSoundInit sound_init;
        sound_init.GainFaderBits = 0x8010;
        PSoundManager->Get2DEmitter().PlaySoundEvent(
            CMainGameComponent::Get()->GetSampleBank(), gui_def.TextBoxButtonASoundCriteria, true, sound_init, false);
    }
}
bool CSubtitleRenderer::ClickPast()
{
    if (IsDisplayingText()) {
        if (!IsInCutscene() || IsFlagSet((ESubtitleFlag)0x40) || IsFlagSet((ESubtitleFlag)8)) {
            if (AllCharactersDisplayed) {
                if (IsTextReadyForClickPast()) {
                    if (IsFlagSet((ESubtitleFlag)2))
                        PlayTextBoxButtonASound();
                    if (IsFlagSet((ESubtitleFlag)8) || (IsFlagSet((ESubtitleFlag)0x200))) {
                        if (TextGroupTag != L"" && GetTextFromTextTag(TextGroupTag, Text)) {
                            ProcessEmbeddedGraphicTags();
                            ProcessEmbeddedHighlightTags();
                            return false;
                        }
                    }
                    Clear(false);
                    return true;
                }
            } else {
                TextCharactersShowing = Text.GetLength();
            }
        }
    } else {
        if (!CUserProfileManager::Get().IsSubtitlingEnabled() && !IsInCutscene()) {
            if (IsFlagSet((ESubtitleFlag)2))
                    PlayTextBoxButtonASound();
            if (IsFlagSet((ESubtitleFlag)0x200)) {
                if (TextGroupTag != L"" && GetTextFromTextTag(TextGroupTag, Text)) {
                    ProcessEmbeddedGraphicTags();
                    ProcessEmbeddedHighlightTags();
                    return false;
                }
            }
            Clear(false);
            return true;
        }
    }
    return false;
}