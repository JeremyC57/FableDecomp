// CGameScriptInterface::HeroPlayOracleMinigame() const -- Dispatch to the live main-player character component.
// Leaf: ?HeroPlayOracleMinigame@CGameScriptInterface@@UBEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own virtual const PDB API, component263 and CTCOracleMinigame::PlayGame callee007e5d20. Player accessor00487dc0 retains established GetPPlayerCharacter binding, distinct from duplicate GetPControlledCreature00487dd0.
#define CGameScriptInterface CGameScriptInterface_HeaderLayout
#include "engine/CGameScriptInterface.h"
#undef CGameScriptInterface
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
#include "engine/CTCBase.h"
#include "engine/CTCTargeted.h"
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
class CCPPointerInfo;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CArray.h"

// Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.

// Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
class CTCEnemy:public CTCBase {public:bool FriendsWithEverythingFlag,EnableFollowersEnemyProxy;};
#define CPlayer CPlayer_HeaderLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CPlayerManager CPlayerManager_HeaderLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#define CTCOracleMinigame CTCOracleMinigame_HeaderLayout
#include "engine/CTCOracleMinigame.h"
#undef CTCOracleMinigame
// Layout: shared player/player-manager/oracle headers supply external receiver layout only. No instance fields are read on them; genuine methods restore original symbols.
class CThingPlayerCreature:public CThing {};
class CPlayer:public CPlayer_HeaderLayout {public:CThingPlayerCreature* GetPPlayerCharacter()const;};
class CPlayerManager:public CPlayerManager_HeaderLayout {public:CPlayer& GetMainPlayer();};
class CTCOracleMinigame:public CTCOracleMinigame_HeaderLayout {public:void PlayGame();};
// Layout: no active CTCSpecialAbilities header; external method receiver only, no instance fields accessed.
class CTCSpecialAbilities:public CTCBase {public:void CancelAbilitiesForCutscene();void ResumeAbilitiesForCutscene();};
// Layout: genuine script vptr+0, PlayerManager+0x14 from shared header; no other owner fields read. CThing Dead bit0 at+0x91 is retail, donor+0xa1.
class CGameScriptInterface {public:virtual void HeroPlayOracleMinigame()const;unsigned char Prefix[0x10];CPlayerManager* PlayerManager;unsigned char RemainingOwnerFields[0x38];};
void CGameScriptInterface::HeroPlayOracleMinigame()const{CThingPlayerCreature* thing=PlayerManager->GetMainPlayer().GetPPlayerCharacter();if(thing && !thing->IsDead()){CTCOracleMinigame* component;if(thing->GetTC(TCI_ORACLE_MINIGAME, &component))component->PlayGame();}}