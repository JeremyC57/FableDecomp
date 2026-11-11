// CTCOracleMinigame::FrameUpdate() -- run the oracle minigame's state machine: tint the symbols, trim the on-screen input symbols, then per stage wait for the subtitle, start a round, fire the outputs, check the player's sequence and leave the quit stage
// Leaf: ?FrameUpdate@CTCOracleMinigame@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E6750, 832 bytes with its 5-entry jump table (0x007E6A7C), thiscall, plain `ret`;
//   vtable 0x012707B4 slot 9 (cell 0x012707D8).
// Identity (two sources): Ego_r.pdb ?FrameUpdate@CTCOracleMinigame@@UAEXXZ 0x007E1C60 behind UpdateTints 0x007E1910, as retail
//   0x007E6750 behind 0x007E6450; CTCBase slot 9 = FrameUpdate (engine/CTCBase.h).
// Extent: the catalog row (883 bytes) ran on through three small functions at 0x007E6A90 / 0x007E6AB0 / 0x007E6AC0; SPLITS.tsv.
// Callees: CTCOracleMinigame::UpdateTints 0x007E6450, OutputsFiring 0x007E5D00, SetText 0x007E6230, FireOutput 0x007E61B0,
//   PlaySoundW 0x007E60F0, Quit 0x007E62D0; NPlayerGui::CDrawQuestInfo::OracleSymbolSize 0x00647308, OracleSymbolPopFront 0x006463F0,
//   DisplayMiniGameInfo 0x00654593; CSubtitleRenderer::SetButton2OverrideText 0x0048F881; CWorld::GetFrame 0x0049D870;
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; __ftol2 0x00BFEA70.
// Globals: CPlayerGui singleton 0x013B8790, CMainGameComponent::ConstantFPS 0x01375550.
// Shape: the three names are implicit const char* -> CCharString conversions at the call (retail gives each temporary its own frame
//   slot and passes `lea reg,[slot] ; push reg`, 0x007E6873 / 0x007E68AF / 0x007E69CE); a named block-scoped CCharString shares a
//   slot with the frame / fps temporaries (frame 0x0C instead of 0x14) and an explicit CCharString("...") pushes the ctor's eax.
// Headers (overlay, FABLE_HDR_OVERLAY=$A/hdr): engine/CTCOracleMinigame.h FABLE_RETAIL_CTCORACLEMINIGAME_CLASS,
//   engine/NPlayerGui_CDrawQuestInfo.h FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLS, engine/CSubtitleRenderer.h
//   FABLE_RETAIL_CSUBTITLERENDERER_BUTTONOVERRIDE.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCORACLEMINIGAME_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONOVERRIDE
#define FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "fable_string.h"
#include "engine/CTCOracleMinigame.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"

void CTCOracleMinigame::FrameUpdate()
{
    CSubtitleRenderer& subtitles = CPlayerGui::Get().GetSubtitleRenderer();
    if (Stage != 0)
        UpdateTints();
    // keep one on-screen symbol per pending input (plus the one being entered); with nothing pending, drop the leftovers
    if ((unsigned long)CPlayerGui::Get().GetQuestInfo().OracleSymbolSize() > InputsPending.size() + 1
        || (InputsPending.size() == 0 && CPlayerGui::Get().GetQuestInfo().OracleSymbolSize() != 0 && !OutputsFiring()))
        CPlayerGui::Get().GetQuestInfo().OracleSymbolPopFront();
    switch (Stage) {
    case 1:                                             // offered: wait for the subtitle to go
        if (!subtitles.IsDisplayingText()) {
            Stage = 2;
            PostWaitStage = 3;
            SetText(PDef->TextWatch);
            long framesPerSecond = CMainGameComponent::GetConstantFPS();
            const COracleMinigameDef* def = PDef;
            WaitTime = (long)(CWorld::GetFrame() + framesPerSecond * def->WaitBetweenRoundsSeconds);
            CPlayerGui::Get().GetSubtitleRenderer().SetButton2OverrideText("TEXT_GUI_FISHING_STOP");
            CPlayerGui::Get().GetQuestInfo().DisplayMiniGameInfo(true, MINIGAME_ORACLE, 0);
            PlaySoundW("SND_MM_GLYPH_MINIGAME_START");
        }
        break;
    case 2:                                             // waiting
        if (WaitTime < CWorld::GetFrame()) {
            Stage = PostWaitStage;
            SetText(PDef->TextWatch);
            NextOutputFrame = CWorld::GetFrame();
            NumOutputsFired = 0;
            if (Stage == 4)
                SetText(PDef->TextRepeat);
            if (Stage == 3)
                SetText(PDef->TextWatch);
        }
        break;
    case 3:                                             // the oracle shows the sequence
        if (NextOutputFrame <= CWorld::GetFrame()) {
            if (NumOutputsFired < Level) {
                FireOutput();
                long framesPerSecond = CMainGameComponent::GetConstantFPS();
                NextOutputFrame = (long)(CWorld::GetFrame() + framesPerSecond * TimeBetweenOutputs);
            } else {
                PostWaitStage = 4;
                WaitTime = 0;
                NumOutputsFired = 0;
                Stage = 2;
            }
        }
        break;
    case 4:                                             // the player repeats it
        if (NumOutputsFired == Level && !OutputsFiring()) {
            PlaySoundW("SND_MM_EXPRESSION_SEQUENCE_COMPLETE");
            const COracleMinigameDef* def = PDef;
            ++Level;
            if (Level > def->MaxLevel) {
                Success = true;
                Quit();
            } else {
                Stage = 2;
                PostWaitStage = 3;
                long framesPerSecond = CMainGameComponent::GetConstantFPS();
                WaitTime = (long)(CWorld::GetFrame() + framesPerSecond * def->WaitBetweenRoundsSeconds);
                TimeBetweenOutputs = PDef->PauseBetweenOutputsLevelMultiplier * TimeBetweenOutputs;
                SetText(PDef->TextWonRound);
            }
        }
        break;
    case 5:                                             // quitting
        if (WaitTime < CWorld::GetFrame())
            Stage = 0;
        break;
    }
}