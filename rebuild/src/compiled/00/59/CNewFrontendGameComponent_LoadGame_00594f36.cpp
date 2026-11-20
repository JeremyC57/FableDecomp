// CNewFrontendGameComponent::LoadGame(CWideString& game) @ 0x00594F36 -- start the game (CGameComponent slot 4 SetQuit, m_bCreateGame)
//   and remember the save to load
// Leaf: ?LoadGame@CNewFrontendGameComponent@@QAEXAAVCWideString@@@Z
// Retail prepares the component through vtable slot 0x10, marks the pending load at +0x29, then assigns the requested name into +0x84.
// Naming (NF129, 2026-11-20, q4933): Ego_r.pdb 0x0060184D + FableWin.pdb 0x02012EEB ?LoadGame@CNewFrontendGameComponent@@QAEXAAVCWideString@@@Z,
//   the spelling its landed caller NUISystem::CFrontEndManager::Update 0x00599E3F binds; the stand-ins CNewFrontendGameComponent_00594f36 /
//   FrontEndWideString_00594f36 are replaced by engine/CNewFrontendGameComponent.h (FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN: SaveGame +0x84;
//   _STARTGAME: the inline StartGame = SetQuit(); m_bCreateGame = true, and the LoadGame declaration) and fable_string.h, so the assignment
//   binds CWideString::operator=(const CWideString&) 0x0099B7D0 (??4CWideString@@QAEABV0@ABV0@@Z).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_STARTGAME
#include "fable_string.h"
#include "engine/CNewFrontendGameComponent.h"

void CNewFrontendGameComponent::LoadGame(CWideString& game)
{
    StartGame();
    SaveGame = game;
}

#pragma optimize("s", off)
