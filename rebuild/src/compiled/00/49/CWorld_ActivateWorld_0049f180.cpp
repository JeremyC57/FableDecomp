// CWorld::ActivateWorld(bool loadedGame) -- staged world start-up: make sure the hero character exists, refresh the
// environment's spell-effect screen stack, activate the player GUI, activate the initial quests, then post the
// "world activated" game event for the main player and stamp the 3D-rendering start frame
// Leaf: ActivateWorld
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049F180, 334 bytes, `ret 4`, /GS (cookie 0x0139C8A8, check 0x00BFE9F9).
//   PDB ?ActivateWorld@CWorld@@QAEX_N@Z.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the loading-stage marker 0x009D8240
//   (__fastcall, string + 0; empty in retail) and its closing marker 0x009D8250 (__fastcall, 0; empty);
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CPlayerManager::
//   InitPlayerCharacter(bool) 0x00449D90, the spell-effect interpolator's screen-effect refresh 0x006B8410 (its
//   ScreenEffectPriorityStack at +0x90), CPlayerGui::InitialActivate 0x0043A380 on g_pPlayerGui 0x013B8790,
//   CQuestManager::ActivateMultipleQuests(CArray<CCharString>*, bool, bool) 0x004B4260 and PostBoastTransfer 0x004B2890
//   on CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass 0x013B89FC, CGameEvent::AddData(long, const void*) 0x009F1760, CPlayerManager::GetMainPlayerNumber
//   0x00449700, CMainGameComponent::AddGameEvent(const CGameEvent&) 0x0041649C on CMainGameComponent::Get() 0x013B86A0;
//   the world frame global 0x013B89BC.
// Layout: rebuild/include/engine/CWorld.h (PlayerManager +0x0c, PEnvironment +0x18, FrameStarted3DRendering +0x8c,
//   InitialActiveQuests +0xac -- retail matches the header); CGameEvent 0x28 bytes (Type, Player +4, Data +5, EndPos
//   +0x25, Valid +0x26, Replacement +0x27; Ego_r.pdb); CThing Dead bit +0x91 (as the landed 0x0083BE90); the
//   environment's spell-effect interpolator pointer is at retail +0x1964.
#define CWorld CWorld_PDB
#include "engine/CWorld.h"
#undef CWorld
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/CArray.h"
namespace NProfileTimer {
void __fastcall StartProfile(const CCharString& name, long level);      // 0x009D8240 (empty in retail)
void __fastcall EndProfile(long level);                                 // 0x009D8250 (empty in retail)
}

class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                           // 0x00487DC0
};
class CPlayerManager {
public:
    long GetMainPlayerNumber() const;   // 0x00449700 (Ego_r.pdb ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ)
private: public:
    void InitPlayerCharacter(bool loadedGame);           // 0x00449D90
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
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:
    bool ActivateMultipleQuests(CArray<CCharString>* quests, bool fromSave, bool showMessages);   // 0x004B4260
    void PostBoastTransfer();                            // 0x004B2890
};
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
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    void AddGameEvent(const CGameEvent& event);          // 0x0041649C
};
extern long g_WorldFrame;                                // 0x013B89BC
enum { GAME_EVENT_WORLD_ACTIVATED = 9 };
class CWorld : public CWorld_PDB { public:
    void ActivateWorld(bool loadedGame);
};
void CWorld::ActivateWorld(bool loadedGame)
{
    NProfileTimer::StartProfile("Init Characters", 0);
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        PlayerManager->InitPlayerCharacter(loadedGame);
    PEnvironment_ptr->SpellEffectInterpolator->RefreshScreenEffects();
    NProfileTimer::StartProfile("Init GUI", 0);
    g_pPlayerGui->InitialActivate();
    NProfileTimer::StartProfile("Init Quests", 0);
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().ActivateMultipleQuests((CArray<CCharString>*)InitialActiveQuests, false, true);
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PostBoastTransfer();
    NProfileTimer::EndProfile(0);
    CGameEvent event(GAME_EVENT_WORLD_ACTIVATED);
    event.Add(false);
    event.Player = (signed char)((CPlayerManager*)PlayerManager)->GetMainPlayerNumber();
    CMainGameComponent::Get()->AddGameEvent(event);
    FrameStarted3DRendering = g_WorldFrame;
}