// NScript::CQ_GuildTrainingScript::Init() -- reset the quest state (GameState, HeroWarnings, StartedTesting, the will-help timer and four flags) and set up the quest's gossip
// Leaf: ?Init@CQ_GuildTrainingScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3B3D0, 55 bytes, thiscall, no `ret`: ends in a tail `jmp` to GossipSetup 0x00D3B410.
// Identity: outer script vtable 0x012CD458 slot 3 (.rdata ref 0x012CD464; slot 0 scalar dtor 0x00D50650, 1 RegisterMain 0x00D3BB50,
//   2 Main 0x00D3BC60, 4 OnPersist 0x00D3BC10; engine/CQ_GuildTrainingScript.h lists the same slots); Ego_r.pdb / FableWin.pdb
//   ?Init@CQ_GuildTrainingScript@NScript@@UAEXXZ. The tail callee is ?GossipSetup@CQ_GuildTrainingScript@NScript@@QAEXXZ (landed).
// Body: stores in member order GameState +0x48, HeroWarnings +0x50, DomeCutsceneStart +0x4e, StartedTesting +0x58,
//   StartedMeleeTesting +0x5c, then WillHelpTimer (+0x60) = 0 through the expanded CTimer::operator= (pDSTGame 0x0143E8F8 slot 89
//   SetTimer(TimerIndex, 0), `call [eax+164h]`), FightFinished +0x64, DisplayBirdKilledMessage +0x65.
// Class: engine/CQ_GuildTrainingScript.h with the chunk 858 opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_INIT (GossipSetup declaration).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_INIT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_GuildTrainingScript.h"

extern CGameScriptInterfaceBase* pDSTGame;                          // 0x0143E8F8
FABLE_FORCEINLINE CTimer& CTimer::operator=(long n) { pDSTGame->SetTimer(TimerIndex, n); return *this; }

void NScript::CQ_GuildTrainingScript::Init()
{
    GameState = 0;
    HeroWarnings = 0;
    DomeCutsceneStart = false;
    StartedTesting = 0;
    StartedMeleeTesting = false;
    WillHelpTimer = 0;
    FightFinished = false;
    DisplayBirdKilledMessage = false;
    GossipSetup();
}