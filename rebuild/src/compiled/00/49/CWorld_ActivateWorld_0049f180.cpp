// CWorld::ActivateWorld(bool loadedGame) -- staged world start-up: make sure the hero character exists, refresh the
// environment's spell-effect screen stack, activate the player GUI, activate the initial quests, then post the
// "world activated" game event for the main player and stamp the 3D-rendering start frame
// Leaf: ActivateWorld
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049F180, 334 bytes, `ret 4`, /GS (cookie 0x0139C8A8, check 0x00BFE9F9).
//   PDB ?ActivateWorld@CWorld@@QAEX_N@Z.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the loading-stage marker 0x009D8240
//   (__fastcall, string + 0; empty in retail) and its closing marker 0x009D8250 (__fastcall, 0; empty);
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CPlayerManager::
//   InitPlayerCharacter(bool) 0x00449D90, the spell-effect interpolator's screen-effect refresh 0x006B8410 (its
//   ScreenEffectPriorityStack at +0x90), CPlayerGui::InitialActivate 0x0043A380 on g_pPlayerGui 0x013B8790,
//   CQuestManager::ActivateMultipleQuests(CArray<CCharString>*, bool, bool) 0x004B4260 and PostBoastTransfer 0x004B2890
//   on g_pQuestManager 0x013B89FC, CGameEvent::AddData(long, const void*) 0x009F1760, CPlayerManager::GetMainPlayerID
//   0x00449700, CMainGameComponent::AddGameEvent(const CGameEvent&) 0x0041649C on g_pMainGameComponent 0x013B86A0;
//   the world frame global 0x013B89BC.
// Layout: rebuild/include/engine/CWorld.h (PlayerManager +0x0c, PEnvironment +0x18, FrameStarted3DRendering +0x8c,
//   InitialActiveQuests +0xac -- retail matches the header); CGameEvent 0x28 bytes (Type, Player +4, Data +5, EndPos
//   +0x25, Valid +0x26, Replacement +0x27; Ego_r.pdb); CThing Dead bit +0x91 (as the landed 0x0083BE90); the
//   environment's spell-effect interpolator pointer is at retail +0x1964.
#define CWorld CWorld_PDB
#include "engine/CWorld.h"
#undef CWorld
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
template<class T> class CArray { public: T* First; T* Last; T* End; long Pad; };
void __fastcall LoadingStage(const CCharString& stage, long level);     // 0x009D8240 (empty in retail)
void __fastcall LoadingStageDone(long level);                           // 0x009D8250 (empty in retail)
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                              // +0x91 bit 0
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                           // 0x00487DC0
};
class CPlayerManager { public:
    void InitPlayerCharacter(bool loadedGame);           // 0x00449D90
    long GetMainPlayerID();                              // 0x00449700
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CEnvironmentSpellEffectInterpolator { public:
    void RefreshScreenEffects();                         // 0x006B8410
};
class CEnvironment { public:
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffectInterpolator* SpellEffectInterpolator;   // +0x1964 (retail)
};
class CPlayerGui { public:
    void InitialActivate();                              // 0x0043A380
};
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790
class CQuestManager { public:
    bool ActivateMultipleQuests(CArray<CCharString>* quests, bool fromSave, bool showMessages);   // 0x004B4260
    void PostBoastTransfer();                            // 0x004B2890
};
extern CQuestManager* g_pQuestManager;                   // 0x013B89FC
class CGameEvent { public:
    long          Type;                                  // +0x00
    signed char   Player;                                // +0x04
    unsigned char Data[32];                              // +0x05
    unsigned char EndPos;                                // +0x25
    bool          Valid;                                 // +0x26
    bool          Replacement;                           // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddData(long size, const void* data);           // 0x009F1760
    template<class T> void Add(const T& value) { AddData(sizeof(T), &value); }
};
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);          // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
extern long g_WorldFrame;                                // 0x013B89BC
enum { GAME_EVENT_WORLD_ACTIVATED = 9 };
class CWorld : public CWorld_PDB { public:
    void ActivateWorld(bool loadedGame);
};
void CWorld::ActivateWorld(bool loadedGame)
{
    LoadingStage("Init Characters", 0);
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->Dead)
        PlayerManager->InitPlayerCharacter(loadedGame);
    PEnvironment_ptr->SpellEffectInterpolator->RefreshScreenEffects();
    LoadingStage("Init GUI", 0);
    g_pPlayerGui->InitialActivate();
    LoadingStage("Init Quests", 0);
    g_pQuestManager->ActivateMultipleQuests((CArray<CCharString>*)InitialActiveQuests, false, true);
    g_pQuestManager->PostBoastTransfer();
    LoadingStageDone(0);
    CGameEvent event(GAME_EVENT_WORLD_ACTIVATED);
    event.Add(false);
    event.Player = (signed char)PlayerManager->GetMainPlayerID();
    g_pMainGameComponent->AddGameEvent(event);
    FrameStarted3DRendering = g_WorldFrame;
}