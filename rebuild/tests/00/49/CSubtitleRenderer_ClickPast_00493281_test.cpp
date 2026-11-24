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
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CUserProfileManager.h"
#include <stdio.h>

static bool GameHasSoundManager;
static bool OriginalTextReady;
static bool TagPresent;
static bool NextTextPresent;
static long VisibleTextLength;
static int AudioCalls;
static int NextTextCalls;
static int GraphicCalls;
static int HighlightCalls;
static int ClearCalls;
static int ReadyCalls;

CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() {}
long CWideString::GetLength() const { return VisibleTextLength; }
bool CWideString::operator!=(const wchar_t* other) const
{
    return *other == 0 && TagPresent;
}

CUserProfileManager& CUserProfileManager::Get()
{
    static CUserProfileManager profile;
    return profile;
}

bool CSubtitleRenderer::IsTextReadyForClickPast() const
{
    ++ReadyCalls;
    return OriginalTextReady;
}

bool CSubtitleRenderer::GetTextFromTextTag(const CWideString&, CWideString&)
{
    ++NextTextCalls;
    return NextTextPresent;
}

void CSubtitleRenderer::ProcessEmbeddedGraphicTags() { ++GraphicCalls; }
void CSubtitleRenderer::ProcessEmbeddedHighlightTags() { ++HighlightCalls; }
void CSubtitleRenderer::Clear(bool whole)
{
    ClearCalls += whole ? 100 : 1;
}

static void PlayTextBoxButtonASound()
{
    if (GameHasSoundManager)
        ++AudioCalls;
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

struct Case {
    bool displaying;
    bool in_cutscene;
    bool all_visible;
    bool ready;
    bool use_new_profile;
    bool old_subtitling;
    bool new_subtitling;
    bool sound_manager;
    unsigned long flags;
    bool tag_present;
    bool next_text;
    bool should_clear;
    bool should_show_all;
    int ready_calls;
    int audio_calls;
    int next_text_calls;
    int process_calls;
};

int main()
{
    const Case cases[] = {
        {true, true,  true,  true,  false, false, false, true,  0x002, true,  true,  false, false, 0,0,0,0},
        {true, true,  false, true,  false, false, false, true,  0x040, false, false, false, true,  0,0,0,0},
        {true, true,  true,  false, false, false, false, true,  0x008, true,  true,  false, false, 1,0,0,0},
        {true, true,  true,  true,  false, false, false, true,  0x00a, true,  true,  false, false, 1,1,1,1},
        {true, false, true,  true,  false, false, false, true,  0x200, false, true,  true,  false, 1,0,0,0},
        {true, false, true,  true,  false, false, false, false, 0x202, true,  false, true,  false, 1,0,1,0},
        {true, false, true,  true,  false, false, false, true,  0x002, true,  true,  true,  false, 1,1,0,0},
        {false,false, false, true,  false, true,  false, true,  0x202, true,  true,  false, false, 0,0,0,0},
        {false,false, false, true,  true,  false, true,  true,  0x202, true,  true,  false, false, 0,0,0,0},
        {false,true,  false, true,  false, false, false, true,  0x202, true,  true,  false, false, 0,0,0,0},
        {false,false, false, true,  true,  true,  false, true,  0x202, true,  true,  false, false, 0,1,1,1},
        {false,false, false, true,  false, false, false, false, 0x202, false, true,  true,  false, 0,0,0,0},
        {false,false, false, true,  false, false, false, true,  0x200, true,  false, true,  false, 0,0,1,0},
        {false,false, false, true,  false, false, false, true,  0x002, true,  true,  true,  false, 0,1,0,0}
    };
    for (unsigned int index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        const Case& test = cases[index];
        CSubtitleRenderer renderer;
        CUserProfileManager& profile = CUserProfileManager::Get();
        renderer.DisplayingText = test.displaying;
        renderer.InCutscene = test.in_cutscene;
        renderer.AllCharactersDisplayed = test.all_visible;
        renderer.DisplayFlags = test.flags;
        renderer.TextCharactersShowing = 4;
        profile.UseNewProfileValues = test.use_new_profile;
        profile.SubtitlingEnabled = test.old_subtitling;
        profile.SubtitlingEnabledNewProfile = test.new_subtitling;
        GameHasSoundManager = test.sound_manager;
        OriginalTextReady = test.ready;
        TagPresent = test.tag_present;
        NextTextPresent = test.next_text;
        VisibleTextLength = 27;
        AudioCalls = NextTextCalls = GraphicCalls = HighlightCalls = ClearCalls = ReadyCalls = 0;
        const bool cleared = renderer.ClickPast();
        if (cleared != test.should_clear || ClearCalls != (test.should_clear ? 1 : 0)
            || renderer.TextCharactersShowing != (test.should_show_all ? VisibleTextLength : 4)
            || ReadyCalls != test.ready_calls || AudioCalls != test.audio_calls
            || NextTextCalls != test.next_text_calls || GraphicCalls != test.process_calls
            || HighlightCalls != test.process_calls)
            return index + 1;
    }
    puts("00493281_TEST PASS");
    return 0;
}