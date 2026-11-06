# chunk 798 notes

Result: 3 RELOCATION_MATCH, 3 parked. All six names are proven (two sources each); there are no descriptive names among the rows.

## 0x00D35090 NScript::CQ_FireHeartScript::Main (RELOCATION_MATCH, src/00d35090.cpp)
- Shape: the AT-Main binding model with ten CEntityScriptBinding<CQ_FireHeartScript> objects, flag 0 at +0x18.
- Then WAIT loops on IsLevelLoaded / ProphetsScripted / MissionSucceeded||MissionFailed, and three spawned threads (RunThemeOverriding, RunVillagerComments, CheckGameEnding).
- Key spellings:
  - `VillagerEscapeMarker = Game->GetThingWithScriptName(...)` goes through a declared-only CCountedPointer<CScriptThing>::operator= specialization (0x00704580 called) and an inline CScriptThing::operator=.
  - The JackBossBattleResult choice is an if/else with two PlayCutscene calls (tail-merged).
  - Prisoner rewards come from pGlobalScriptDef FHKillAllPrisoners* / FHFreeAllPrisoners*.
- Identity: vtable slot .rdata 0x012CCB98; Ego_r + FableWin ?Main@CQ_FireHeartScript@NScript@@UAEXXZ.

## 0x00D39330 NScript::CQ_FireHeartScript::PlayCutscene (RELOCATION_MATCH, src/00d39330.cpp)
- Shape: the WaspBoss cutscene model. Cast HERO / GUEST=seh_Prophet[Guest] / PROPHET1-5 / ROSE / SCYTHE / GM, then RunCutsceneMacro(name, cast, &CutsceneFlags, &csargs, false, true) and csargs.clear() through the declared-only map<CCharString,CCharString>::clear (0x009AACE0).
- Label fix: the second label CScriptBase::PlayCutscene at 0x00D39330 is wrong.

## 0x00ED5830 NScript::CV_SwordInTheStoneScript::SwordPullCutscene (RELOCATION_MATCH, src/00ed5830.cpp)
- Stats via (EHeroTrainableStatType)0/2/1 (physique, toughness, health).
- Success / fail cutscenes, and the crowd member names the first stat that fell short.
- Ends with `hero.Release(); CameraDefault(); SwordCutscene = false; SwordRemovalAttemptCount++;`.
- Identity: caller 0x00ED5814 + PDB.

## 0x00F0C140 NScript::CQ_AmbushTradersScript::PhaseTwo (parked DIFFER(2208v2285) 247, differ/00f0c140.cpp)
- Uses the PHASETWO and VECTOR_SWAP opt-ins and a noinline template Compare.
- Residue: the exit-block placement and inline choices of the vector / CScriptThing dtors at the pre-loop and loop-bottom terminate exits, the post-loop ~vector, and TeleportInfo.size() being called.

## 0x00D7B510 NScript::CQ_HeroSoulsThunderScript::CHeroThunder::Main (parked DIFFER(3533v3524) 21, differ/00d7b510.cpp)
- Residue: only the closing idle loop's duplicated head test (retail jumps to one bottom test), as in parked 0x00D82640.
- Retail string crossing kept as found: the WhisperKilled && !Married branch speaks ..._KILLEDWHISPER_MARRIEDGREY_ONHIT_10, and so on.

## 0x00E27E90 NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Main (parked DIFFER(3595v3595) 37, differ/00e27e90.cpp)
- Uses FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY.
- Residues are per-site inline choices:
  - the CIsThingAlive member copy (called in retail);
  - the hit-branch seh_hero ctor (expanded in retail, called 0x007E72A0 here).

## Callee names (evidence)
- SetPredicate 0x00F35B10 (Ego_r/FableWin). The label DB also carries RegisterFilter; that is the wrong one.
- ~CIsThingAlive 0x00E241B0.
- AddLogbookStoryEntry 0x00CBE87F (__fastcall).
- IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall; spelling in NAME_FIXES.tsv).
- vector<TeleportData>::swap 0x00F0CA30 (landed as SThreeWordRecord::Swap: NAME_FIXES.tsv).
- map<CCharString,STB>::~map 0x00CDBFB0 and TeleportData::TeleportData 0x00F0D470: label fixes, both from vland RELOC_NAME warnings (LABELS.tsv).
- 0x00704580: proposal only, since it may be ICF-folded (NAME_FIXES.tsv).

## Header debt
- EMissionHistory in CQ_FireHeartScript.h has no enumerators; it is not needed by these rows.
