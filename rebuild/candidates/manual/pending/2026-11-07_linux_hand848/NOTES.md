# Hand chunk 848 (CThingManager .. CV_KnotholeGladeGatesScript): 4 of 6 byte-exact

All four matched rows are RELOCATION_MATCH (none plain MATCH) with tests PASS under g++ and VC7.1 (gall.sh with the final overlay:
logs/grades.txt). Two rows are parked in differ/ with their tests. Every attempt is in STATUS.tsv. The header overlay is in hdr/
(see hdr/README.md); 33 landed includers were rechecked with it (SIBLINGS.tsv).

## What matched (4)

- 0x00B2D9F0 CEngineSurfaceCompositionManager::UpdateDiskCache (807 bytes, Codex pass 12 body rebased on our headers). Shape: the
  limit-set filter is a scoped `const long` key and a named iterator compared `== end()` before `continue` (the key is stored after
  the null test, find before end()). The STLport set find is a member template (owner decision D1), FABLE_RETAIL_STLP_SET_FIND, and
  inline GetGraphicBank / CGraphicFrame ctor opt-ins. Header debt: the bank / graphic-info / surface-cache classes are local models
  (engine/CBankFile.h is HOLD; CGraphicDataBank.h and CEngineSurfaceCompositionManager.h exist but have opaque bases). The landing
  review should decide whether to fold them in.
- 0x009BC120 CPersistContext::TransferVectorOfSubComponentsLoadText (1639, Codex body rebased). Shape: the inline CleanOutLine
  indexes `[ecx+eax]` only with GetAsCharArray spelled preset-then-`if` (FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT). The
  nested single-transfer CPersistContext ctor and CleanOutLine bodies are in the TU (the landed 0x009BB110 model).
- 0x00EA2720 NScript::CV_KnotholeGladeGatesScript::WatchForCutscene1 (1334; was the placeholder _global::KnotHoleGates_OnScript).
  Shape: one single-site `do { } while (0);` round the three cast-map assignments expands the implicit STB operator= (CBaseObject::
  operator= 0x0099A3B0 + the PImp copy) as retail does. Also: row-local inline ~CScriptThing / IsAlive, a declared-only movie PImp
  dtor spec and a map dtor spec. The map ctor is expanded.
- 0x00D321C0 NScript::CQ_EndGameBossBattleScript::JackOfBladesBossFightCutscene (1136; was the placeholder
  _global::CScriptGameResourceObject_LoadEndGameSister). Shape: a plain `while (!Start...) { NSF; if (T) return; }` sister loop with
  a row-local inline ~CScriptThing keeps the temp dtor expanded and the loop unrotated. There is a declared-only
  `CCountedPointer<STB>::operator=` spec (0x008AB1E0), plus a single-site `do { } while (0);` round `mymap["Hero"] = seh_Hero;`, which
  expands the STB operator= there.

Name proofs (two sources each): WatchForCutscene1 = DoMission 0x00EA1090's CSpawnedFunc("WatchForCutscene1", member pointer 0x00EA2720)
plus the FableWin / Ego_r mangled name. JackOfBladesBossFightCutscene = the sole direct caller 0x00D31358 in JackOfBladesBossBattle
plus the FableWin source order (Main, JackOfBladesBossBattle, JackOfBladesBossFightCutscene, JackOfBladesPhaseWatcher) plus
CS_JACK_BOSS_INTRO.

## Parked for Fable 5.1 (2)

- 005253f0 CThingManager::LoadGameState DIFFER(2263v2263): text loop only -- retail caches the UID low dword in ebx (`mov ebx,eax` after GFCharStringToUHUGE, `push ebx`, this reloaded into esi before SetNameForThing / NPCNameMap), ours hoists this into ebx before the loop and reloads the UID from its slot; one loop-align pad | tried: header opt-ins (decls, typed NPCNameMap, implicit ReadSection dtor, ReadLineAsString, ReadPresizedString, ReadUHUGE), __forceinline Replace with the bottom strstr from `found`, nested ifs, `X = true; Y = X;`, uid declared outside the loop, named default UID, signed uid, const uid, value/uid split in the binary path (made it exact) and in the text path (219), 16 grades | next: a text-path UID spelling that is a register candidate whose high dword still comes from the key slot, or the loop shape that stops VC7.1 hoisting this.
- 00dd4ad0 NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main DIFFER(3467v3467): only the predicate -- retail expands ~CIsThingAlive (vptr 0x012C32F0 in ebx, stored twice, ebx pushed in the prologue) and calls ~CScriptThing 0x004AA840; ours calls ~CIsThingAlive (plain inline) or also expands ~CScriptThing (__forceinline / implicit dtor); the rest is the loop-pad shift | tried: shared-header model (h1), forceinline / implicit / temporary predicate, late ~CScriptThing definition, declared-only ~CScriptThing (fixes the predicate, breaks the 0x00DD5385 expansion: md78), forceinline CIsThingAlive ctor, 7 grades | next: lane gap: VC7.1 per-site inline budget (GOTCHAS 2099 / 2389): our TU's ~CScriptThing cost is just under the f=1 threshold where retail's is just over.

Shapes of the parked rows worth keeping:
- LoadGameState: `CurrentlyLoading = true; SerialisingGameState = CurrentlyLoading;`. The SAVED_NPC_NAMES ReadSection test sits
  outside the !IsSerialisingHeroState test (nested ifs). CCharString::Replace is inline four times (FableWin member). The binary
  UID is `unsigned __int64 value = ReadUHUGE(); unsigned __int64 uid = value;` with SetNameForThing(value) and NPCNameMap[uid].first
  = def_name. The ReadSection dtor is implicit in this TU.
- CPrisonerAccomplice::Main: the talk / cell-door body is a `while (!ReleasedAndFollowing)` loop with the release branch after it.
  Every exit inside the movie block is `{ game->PauseAllNonScriptedEntities(false); return; }` through a `game = Game` local. The
  cell door's MsgIsUsedBy is devirtualised and inlined through PImp (`if (!PImp.Data) return false; return PImp.Data->...`).
  Speak(hero, text, 0, false, true, false) is gated by GetHealth(seh.GetScriptThing()) > 0.0f.

Callee names with evidence: see the header comments of each source. New names this chunk binds:
- ?LoadFromGameStateBinary / ?LoadFromGameStateScript@CEntityRuntimePersistence@NThingManager (0x00524EC0 / 0x005248B0);
- ?LoadHeroStateScript@CThingManager (0x00520570);
- ?SetNameForThing@CThingManager@@QAEX_KABVCWideString@@@Z (0x00521A90);
- NPCNameMap operator[] (0x0052B8B0);
- ?ReadLineAsString@CStringParser (0x009BA390);
- ?GFCharStringToUHUGE@@YI_KABVCCharString@@@Z (0x0099E870);
- ?ReadNullTerminatedWString@CMemoryDataInputStream (0x00994960);
- the set<long> tree find template (0x00B2EB20).

All are Ego_r / FableWin spellings except where NAME_FIXES.tsv says otherwise. No descriptive (invented) names in the four src rows.
The one name used on Codex evidence alone is NEngineInternal::GPEngine / GPResourceManager.

The read-only vland check (logs/vland.log): WINS 4, all RELOCATION_MATCH PASS. Its warnings are the renamed placeholder catalog
rows (STANDIN_LEAF), the 0x009FD0F0 label above, the shared-header note for the 0x00B2D9F0 local models, and two COMDAT-folded
addresses that already carry several landed bindings (0x004046B0 list / _List_base dtor, 0x008AB1E0 operator=).

## Skipped

None.

## Label corrections

LABELS.tsv lists them: the two placeholder rows, 0x00524EC0 (ghidra CDrawQuestInfo_LoadDeadCreatures), 0x005248B0, the
??1CPersistContext binding of 0x009BBBF0 (it is CheckUnknownSymbols) and 0x009FD0F0 (catalog _global::SetMember_dword0x268, it is
CGraphicDataBank::SetUnloadFrameDelay). NAME_FIXES.tsv holds 18 spellings for the coordinator to
respell (bindings and landed callers).

## Header debt

- 0x00B2D9F0 local models of CBankFile / CBankFileAsync / CGraphicDataBank / CGraphicInfo / CEngineTextureMorph / CManagedTexture /
  CCompositeSurfaceCache / CEngineSurfaceCompositionManager. A full fix needs engine/CBankFile.h released from HOLD (retail vptr)
  and CGraphicDataBank.h given its real CBankFileAsync / CResourceBank bases.
- 0x00DD4AD0 defines CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice in the row (the header only forward-declares the nested
  entity classes; the landed siblings do the same).
