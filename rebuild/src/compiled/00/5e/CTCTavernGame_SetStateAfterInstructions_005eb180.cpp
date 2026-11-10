// CTCTavernGame::SetStateAfterInstructions(unsigned long instructions, CWideString okText, ETavernGameStates state) -- play the selection sound, show every text of the instruction group as an OK subtitle, then move to the requested state (or leave the game when a quit is pending)
// Leaf: ?SetStateAfterInstructions@CTCTavernGame@@IAEXKVCWideString@@W4ETavernGameStates@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB180, 536 bytes, thiscall `ret 0Ch` (the CWideString argument is destroyed here).
//   Ego_r.pdb ?SetStateAfterInstructions@CTCTavernGame@@IAEXKVCWideString@@W4ETavernGameStates@1@@Z (protected); the function-local
//   static is Ego_r's `CTCTavernGame::SetStateAfterInstructions'::`5'::`dynamic atexit destructor for 'no_text_entry''.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, PlaySoundW 0x005EA950, CWideString(const wchar_t*)
//   0x0099B6B0 / ~CWideString 0x0099B510, atexit 0x004012BC (no_text_entry 0x013B8C50, guard 0x013B8C54),
//   NGameText::CDataBank::GetPGroupEntry 0x009C8890 / GetPTextEntry 0x009C9070, NGameText::CTextEntry::GetText 0x009C7A40,
//   CSubtitleRenderer::SetText (four texts) 0x00492F69; virtual LeaveGame (slot 30, +0x78), also through the inline SetState.
// Data: the quit-request flag 0x013BAE1C, CMainGameComponent::PMainGameComponent 0x013B86A0 (PTextBank +0x14, PCutSceneMainFont
//   +0x1614C), CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass 0x013B8790 (SubtitleRenderer +0x18).
// Shape: SetState (0x005E9B20) is defined in this TU, as in retail's, and expands at the tail (quit -> LeaveGame, state 5 ->
//   GamePlayed, state 47 -> LeaveGame, else the previous / current state stores), as in the landed FrameUpdate 0x005EB9E0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_CUTSCENEFONT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_NGAMETEXT_GROUPENTRY
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CTCTavernGame.h"
#include "engine/CPlayerGui.h"
#include "engine/NGameText_CDataBank.h"
#include "fable_game.h"

extern bool g_bTavernGameQuitRequested;                  // 0x013BAE1C

void CTCTavernGame::SetState(ETavernGameStates state)
{
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        state = (ETavernGameStates)47;
    }
    if (state == 5)
        GamePlayed = true;
    else if (state == 47) {
        LeaveGame();
        return;
    }
    PreviousTavernGameState = TavernGameState;
    TavernGameState = state;
}

void CTCTavernGame::SetStateAfterInstructions(unsigned long instructions, CWideString okText, ETavernGameStates state)
{
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        LeaveGame();
        return;
    }
    {
        CCharString sound("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
        PlaySoundW(sound);
    }
    static CWideString no_text_entry(L"Missing text entry");
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    CIVCountedPointer<const NGameText::CGroupEntry> group = bank.GetPGroupEntry(instructions, true);
    if (!group.IsNull()) {
        const CArray<long>& entries = group->Entries;
        for (const long* it = entries.begin(); it != entries.end(); ++it) {
            CIVCountedPointer<const NGameText::CTextEntry> entry = bank.GetPTextEntry(*it, true);
            CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), entry->GetText(), L"",
                                                            okText, L"", 10, 0.0f, false, true);
        }
    }
    SetState(state);
}