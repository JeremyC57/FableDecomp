// CWorld::EAClickPastText(const CGameEvent* event) -- game-event action: tell the player GUI's subtitle renderer to click past the current text
// Leaf: ?EAClickPastText@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x00629F30, 17 bytes, thiscall `ret 4`.
// Identity (proven): registered by the static initialiser 0x0121B9B0 as the handler of game event 25 in the dispatch table
//   0x013B9288 (no direct caller); it sits between EAAnswerQuestion 0x00629DE0 and EASkipCutScene 0x00629F50, exactly as
//   the Ego_r.pdb donor build orders EAAnswerQuestion, EAClickPastText, EASkipCutScene; its only call is
//   CSubtitleRenderer::ClickPast() on the player GUI's embedded SubtitleRenderer (+0x18).
// Callees: CSubtitleRenderer::ClickPast() 0x00493281 (Ego_r.pdb ?ClickPast@CSubtitleRenderer@@QAE_NXZ; the bool result is
//   dropped, so `call ; ret 4` instead of a tail jump); global CPlayerGui pointer 0x013B8790.
// Layout: CPlayerGui (Ego_r.pdb, retail-equal up to here): vptr, World +4, PlayerManager +8, InputInterface +0xC,
//   Component +0x10, Graphics +0x14, SubtitleRenderer +0x18. engine/CPlayerGui.h flattens SubtitleRenderer to bytes, so
//   CPlayerGui is a local partial view (header debt).
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld

enum EPlayerMode { PLAYER_MODE_NULL = 0 };

class CThing {
public:
    void Kill(bool b);                                     // 0x004C9B80 (Ego_r.pdb ?Kill@CThing@@QAEX_N@Z)
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject { };
class CThingPlayerCreature : public CThingCreatureBase { };

class CGameEvent {                                         // 0x28 bytes (Ego_r.pdb = retail)
public:
    long          Type;                                    // +0x00
    signed char   Player;                                  // +0x04
    unsigned char Data[32];                                // +0x05
    unsigned char EndPos;                                  // +0x25
    bool          Valid;                                   // +0x26
    bool          Replacement;                             // +0x27
};

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;  // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                     // 0x00634190
};

class CWorldMap;
class CGamePlayerInterface;
class CMainGameComponent;
class CGraphicDataBank;
class CSubtitleRenderer {
public:
    bool ClickPast();                                      // 0x00493281
};
class CPlayerGui {                                         // partial view
public:
    virtual ~CPlayerGui();
    CWorldDonor* World;                                    // +0x04
    CPlayerManager* PlayerManager;                         // +0x08
    CGamePlayerInterface* InputInterface;                  // +0x0C
    CMainGameComponent* Component;                         // +0x10
    CGraphicDataBank* Graphics;                            // +0x14
    CSubtitleRenderer SubtitleRenderer;                    // +0x18
};
extern CPlayerGui* g_pPlayerGui;                           // 0x013B8790

class CWorld : public CWorldDonor {
public:
    void EAClickPastText(const CGameEvent* event);
};

void CWorld::EAClickPastText(const CGameEvent* event)
{
    g_pPlayerGui->SubtitleRenderer.ClickPast();
}