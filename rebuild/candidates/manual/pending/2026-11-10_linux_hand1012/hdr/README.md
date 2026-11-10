# Chunk 1012 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every change sits under its own new `#ifdef FABLE_RETAIL_*`. Forward declarations are included. Without the macro, each header
compiles exactly as the repo copy does. Evidence (PDB spelling plus retail address) is in the comment above each block.

| Header | Opt-in macro | What it adds | Rows |
|---|---|---|---|
| engine/CDefinitionManager.h | `FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED` | `long GetNoInstantiatedDefsOfType(const CCharString&) const` 0x009AD3B0 | 0x006B1A20 |
| engine/CDefinitionManager.h | `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_BODY` | same-TU noinline body of `GetDef<T>(type_name, index, CDefPointer<const T>*)` (index <= 0 false; GetPDefFromClassIndex 0x009ADE10; copy into *out). Under `_MSC_VER`, `#pragma auto_inline(off)` | 0x006B1A20 |
| engine/CEnvironment.h | `FABLE_RETAIL_CENVIRONMENT_RAINSTRENGTH` | `float GetRainStrength() const` 0x006B2BD0 | 0x006B2260 |
| engine/CThingMarker.h | `FABLE_RETAIL_CTHINGMARKER_CREATE` | `static CThingMarker* FABLE_FASTCALL Create(long, const C3DVector&, const CCharString&)` 0x0083D460 | 0x006B2260 |
| engine/CThingSearchTools.h | `FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY` | noinline body of `GetPBestThing<Filter,Compare,T>` (instance 0x004AC2C0); needs ALLTHINGSBODY and CTHING_STATTYPEFLAGS | 0x0049FFD0 |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_SETKICKPOS` | `void SetKickPos()` | 0x0049FFD0 |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_SAVEGAMESTATE` | `SaveGameState(const CWideString&, bool, bool)` 0x004A05C0, SaveGameStateManual 0x004A0780, AutoSaveRegionLeaving 0x004A0180, AutoLoadRegionLeaving 0x004A34F0 (LoadGameState / AutoLoadRespawn / AutoLoadInternal come from the existing chunk 734 opt-in); protected `CopyFileInternal(...) const` 0x0049DD50 and `bool SaveHeroStateInternal(const CWideString&, bool)` 0x004A00E0 | 0x004A0780, 0x004A34F0, 0x004A5E90 |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_EVENTACTIONS` | EAOpenHeroInformationScreen / EAControlledCreatureStrafe / EAUseProjectileWeapon(const CGameEvent*); protected `CPlayer* GetPlayer(unsigned long)` 0x0049D850 | 0x0062BD80, 0x0062BEF0 |
| engine/CTCHeroStats.h | `FABLE_RETAIL_CTCHEROSTATS_KICKPOSITION` | `void SetHeroKickPositionForCurrentRegion(const C3DVector&, float, bool)` 0x0057D990 | 0x0049FFD0 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_BINARY_COMPOSE` | STLport `logical_or` (struct), `binary_compose` (class, `_M_fn1/2/3`), `compose2`. Ego_r mangles the 0x004AC2C0 instance with these | 0x0049FFD0 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE` | `vector(const vector&)` declared in class; body out of class, `__declspec(noinline)` under `_MSC_VER`. VC7.1 calls it (0x008AC540) but, seeing the body, keeps the copy's start/finish in ebp/edi across the loop | 0x008A8260 |
| fable_user_profile.h | `FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVEPATHS` | GetAutoSavePathName 0x00406F70, GetAutoSaveQuestStartPathName 0x00407FD0, GetAutoSaveRegionLeavingPathName 0x004080B0, GetAutoSaveRegionLeavingFileName 0x00406730 | 0x004A0780, 0x004A34F0 |
| engine/CGameEvent.h | `FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE` | `void GetDataPackage(CGameEventDataPackage*) const` 0x009F17A0; inline `signed char GetPlayerNumber() const` | 0x0062BD80 |
| engine/CGameEventDataPackage.h (new) | `FABLE_RETAIL_CGAMEEVENTDATAPACKAGE` | Ego_r layout Data[32] / Length +0x20 / StartPos +0x24 / EndPos +0x28; the inline ctor zeroes the three words; `GetData(long, void*)` 0x009F18F0 | 0x0062BD80 |
| engine/CThingPlayerCreature.h | `FABLE_RETAIL_CTHINGPLAYERCREATURE_RELATIVEMOVEMENT` | forward `enum EGameAction;` and `ApplyRelativeMovementAcceleration(const C3DVector&, EGameAction)` 0x006AD3C0 | 0x0062BD80 |
| engine/CPlayer.h | `FABLE_RETAIL_PLAYER_REMOVEMODE` | `void RemoveMode(EPlayerMode)` 0x00634190 | 0x0062BEF0 |
| engine/CTCHeroRangedAttacks.h | `FABLE_RETAIL_CTCHERORANGEDATTACKS_PROJECTILEMODE` | `IsToUseFirstPersonTargetingMode() const` 0x00789AB0, `IncrementProjectileWeaponMode()` 0x00789C90 | 0x0062BEF0 |
| engine/CGameCameraManager.h | `FABLE_RETAIL_CGAMECAMERAMANAGER_CAMERASOUND` | `PlayCameraSoundThroughObservedThing(const CCharString&) const` 0x00698B20 | 0x0062BEF0 |
| engine/CTCSpecialAbilities.h (new) | `FABLE_RETAIL_CTCSPECIALABILITIES` | `CTCSpecialAbilities : CTCBase`, TCI_SPECIAL_ABILITIES, inline `GetAbilityMutexLock()` (AbilityMutexLock +0x0C, UsingBed +0x0D) | 0x0062BEF0 |
| engine/CTCSummonSpell.h | `FABLE_RETAIL_CTCSUMMONSPELL_MINION` | members PDef +0x0C, JustReleased +0x10, Minion `CIntelligentPointer<CThingAICreature>` +0x11; inline `GetSummonedCreature() const` (needs CINTELLIGENTPOINTER_COPY) | 0x008A8260 (and 0x008A7A70 next) |
| engine/CTCGhostSword.h (new) | `FABLE_RETAIL_CTCGHOSTSWORD` | `CTCGhostSword : CTCBase`, TCI_GHOST_SWORD, PDef +0x0C, Minions `vector<CIntelligentPointer<CThingAICreature> >` +0x10, inline `GetSummonedCreatures() const` (by-value copy) | 0x008A8260 (and 0x008A7A70 next) |

## Include-order gotcha
fable_string.h includes `"stlport/fable_containers.h"` with quotes. That resolves next to the repo's fable_string.h, so the repo
copy is found before the overlay. A TU that needs an overlay stlport opt-in must `#include "stlport/fable_containers.h"` itself,
before fable_string.h (0x0049FFD0, grade 1 SRC_FAIL).
