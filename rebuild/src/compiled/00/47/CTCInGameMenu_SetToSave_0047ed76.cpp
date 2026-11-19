// CTCInGameMenu::SetToSave(long save_index) -- queue a save: remember the slot, wait two frames, open the "save in progress" dialog (with the hero-save text between quest start and finish screens), centre it and stamp the time
// Leaf: ?SetToSave@CTCInGameMenu@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047ED76, 366 bytes, thiscall `ret 4`; size-optimised TU with a frame pointer
//   (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed same-TU CTCInGameMenu::CloseInGameMenuPC 0x0047DBB8).
// Locals (Ego_d): pos (C2DVector, function scope); ptext in the quest-screen block; ptext_real nested.
// Shape: pos is built from (half back-buffer size) - UI scale * dialog half-extent, Y evaluated first (constructor arguments).
// Inferred: the grouping parentheses around the halves are FITTED, not seen in any donor: without them the X subtraction issues one
//   slot later (`push ecx ; fsubp st(1)` at 0x0047EEBB, retail `fsubp st(1) ; push ecx`; 100+ earlier spellings, flags and visible
//   GetBackBuffer* bodies left it). Measured over all 16 groupings: only the X left group `(GetBackBufferWidth() * 0.5f)` is
//   byte-required (a no-code PAREN tuple moves VC7.1's list scheduler, MATCHING_PLAYBOOK); the Y group is byte-neutral and kept for
//   the symmetric spelling. Cross-checked (hand4a, 2026-11-19): the same bracket is byte-required in PCSetQuitToFrontEnd 0x0047D296
//   and PCSetToLoad 0x0047DD9D (both halves) and the X half in 0x0047E651 / 0x0047E9AE / 0x0047F30F; still a fitted PAREN tuple (no
//   donor shows it -- FableWin divides by a double 2.0). Advisor decision (owner delegated) 2026-11-19; owner review.
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CQuestManager.h"
#include "engine/GFGetTime.h"
using namespace NUISystem;

void CTCInGameMenu::SetToSave(long save_index)
{
    SaveGameIndex = save_index;
    FramesToWaitToSave = 2;
    Components.push_back(CManager::GetInstance()->CreateComponent("PC_SAVE_IN_PROGRESS_DIALOG", false));
    if (CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
        CComponent* ptext = Components[Components.size() - 1]->FindChild(CCharString("PC_SAVE_IN_PROGRESS_TEXT", -1));
        if (ptext) {
            CText* ptext_real = static_cast<CText*>(ptext);
            ptext_real->SetValue("TEXT_GUI_SAVE_HERO_IN_PROGRESS_PC", true, true);
        }
    }
    Components[Components.size() - 1]->Initialise();
    PSaveInProgress = Components[Components.size() - 1];
    C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 204.0f,
                  (CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f) - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PSaveInProgress->ChangePosition(pos, 0.0f, false);
    SaveInProgressTime = GFGetTime();
}