# Chunk 583 -- NScript quest scripts (10 rows): 8 RELOCATION_MATCH, 2 parked

All ten rows are NScript script functions. Every name is proven from two sources: a vtable slot or a spawn-site name
literal, plus the Ego_r.pdb / FableWin.pdb mangled name. The catalog labels were `sub_` placeholders or invented
(`Script_Arena_Teleport_Thread`, `MayorsInvitation_OnEnter`, `Quest009_SpawnMonster_WithFade`, `Script_MakeThingsSpeakNumbered`).

## What matched (src/ + test/, g++ and VC7.1 tests PASS, vland: 8 wins, no RELOC_IDENTITY)
| addr | function | evidence |
|---|---|---|
| 00ee47b0 | NScript::CV_TourGuideScript::Main | script vtable slot 2; Ego_r ?Main@CV_TourGuideScript@NScript@@UAEXXZ; binds CTourGuideGuide::Alloc 0x00EE4BB0 / CTourGuideFollower::Alloc 0x00EE4CF0, spawns WatchForGuideKilled 0x00EE4A70 / WatchForClosingTime 0x00EE4A00 |
| 00e9c8d0 | NScript::CV_IntroductionToTrophiesScript::WatchForCrowdVolume | spawn literal "WatchForCrowdVolume" 0x012ED588 stored at 0x00E9C711 (Main); Ego_r name |
| 00ed7e00 | NScript::CV_TalentlessBardScript::CTB_Bard::Init | slot 2 of entity vtable 0x012F3494 (stored by the CTB_Bard ctor 0x00ED7D50); Ego_r ?Init@CTB_Bard@CV_TalentlessBardScript@NScript@@EAEXXZ |
| 00ebef80 | NScript::CV_ParanoidWhispersScript::Main | slot 2 of vtable 0x012F0F38; Ego_r name; bindings SecretWhisperingMan 0x00EBFB00 / SecretWhisperingFriend 0x00EC0A50 / PW_NecklaceDiggingSpot 0x00EC0900 |
| 00edaf30 | NScript::CV_TempleOfLightScript::OnPersist | slot 4 of vtable 0x012F3C68; Ego_r ?OnPersist@CV_TempleOfLightScript@NScript@@UAEXAAVCPersistContext@@@Z; 23 keys |
| 00f14300 | NScript::CQ_ArenaScript::WatchForTermination | spawn literal in CQ_ArenaScript::Main 0x00F0FB70 (vtable 0x012C7798 slot 2); Ego_r name |
| 00e1dcf0 | NScript::CQR_EscortTraderScript::OnPersist | vtable slot 4; Ego_r name; GetDefaultVal<EscortTraderQuestStage> 0x00E1DF50 / Transfer<..> 0x00E1DF60 (FableWin) |
| 00eb7370 | NScript::CV_MayorsInvitationScript::WatchForTermination | spawn literal in the quest's Main; Ego_r name |

Call targets: vland found no RELOC_IDENTITY. Its three RELOC_NAME warnings are catalog labels and are listed in NAME_FIXES.tsv.
String literals pair with retail's pushed addresses (tmp/strpair.py).

## Parked (differ/)
- **00ee91e0** NScript::CGlobal_DebugCycleThroughSpeechScript::Main, DIFFER(575v613).
  - Identity: slot 2 of vtable 0x012F54B4. The landed RegisterMain 0x00EE9120 spawns it. Ego_r and FableWin both have the name.
  - Residue: retail destroys the `std::set<unsigned long>` local in three different ways at different exits:
    - At the in-loop terminate exits it inlines ~set and ~_Rb_tree down to calls of clear 0x004CFF70 and ~_Rb_tree_base 0x004CFFB0.
    - At the pre-loop terminate exit it calls 0x004D0740 out of line.
    - At the normal scope end it calls 0x004D0E20.
  - The lane calls ~set out of line at every site, so it also tail-merges the pre-loop exit.
  - Probes that did not reproduce it: explicit or `__forceinline` ~set, the real STLport `__malloc_alloc` chain, a visible `_M_erase` body, the QFE 4035 toolset, and /GL+/LTCG.
- **00d2d1a0** NScript::CQ_EndGameFocalSitesScript::CBattleMinion::Main, DIFFER(556v620).
  - Identity: slot 1 of vtable 0x012CC304 (stored by Alloc 0x00D2C340); Ego_r name.
  - Residue: retail inlines ~CScriptGameResourceObjectScriptedThingBase, with its own epilogue, at the first early return only. The lane jumps to the shared out-of-line 0x007E74D0 call.

## Skipped
None.

## Label corrections
See NAME_FIXES.tsv:
- 0x00E1DF50 GetDefaultVal<EscortTraderQuestStage> (was ReturnZero_ret0).
- 0x00CB8930 label CTCVillage::OnInitialActivate is CScriptBase::PostAddScriptedEntities.
- 0x0099F830 GFIntToCharString (PrisonerNumberString / CCharString::FromLong bindings are wrong).
- 0x004292C0 _Rb_global<bool>::_M_increment.
- 0x004CF810 _M_erase of _Rb_tree<unsigned long, ...>.
- 0x00A394D0 is the set<unsigned long> tree ctor (landed as CUnprovenKey).
- 0x004CFF70 clear (landed as CTCCarryable::OnKill).
- 0x004CFFB0 ~_Rb_tree_base (placeholder).
- 0x004D0740 / 0x004D0E20 are ~set and ~_Rb_tree of set<unsigned long>: an identical pair, order unproven.
- 0x007E74D0 ~CScriptGameResourceObjectScriptedThingBase.
- 0x007E7450 IsPerformingScriptTask (UBE).

## Header debt
- **CSpawnedFunc<T>** (00ee47b0, 00ebef80): local template over the shared engine/CSpawnedFuncBase.h. There is no shared CSpawnedFunc header. The retail CSpawnedFunc<T> instantiations have their own vtables (e.g. 0x012F54C8). This follows the landed RegisterMain rows.
- **CPersistContext** (00edaf30, 00e1dcf0): local, with only the Transfer / GetDefaultVal template members. This is the landed CV_BeardyBaldyScript::OnPersist 0x00E4F7F0 model.
- **Opaque data models with no vtable:**
  - CScriptDef (00ed7e00, 00e9c8d0, 00f14300), with members at their retail offsets.
  - CQ_SunnyvaleMasterData (00f14300, 00eb7370).
- **Plain value classes (no vtable):** CCharString, C3DVector, CCPPointerInfo, CCountedPointer.
- **Inline bodies added after the shared CScriptThing.h:**
  - CScriptThing copy ctor (00ed7e00); this is the header's documented contract.
  - CTimer operator= / GetTime (00e9c8d0, `__forceinline` after engine/CTimer.h).
- **Parked rows only:**
  - Local polymorphic CBaseObject / CScriptGameResourceObjectBase / CScriptGameResourceObjectScriptedThingBase, because the shared headers for them are flat and non-polymorphic.
  - 00d2d1a0 uses a local CScriptThing (pre-defined include guard plus a tag inline-copy ctor, as in landed CAT_BanditAlly::Init 0x00F05F30), and a local CSpawnedFunc1<T, P>.
  - 00ee91e0 uses a local STLport set / _Rb_tree model (from the landed 0x00A394D0 text).
