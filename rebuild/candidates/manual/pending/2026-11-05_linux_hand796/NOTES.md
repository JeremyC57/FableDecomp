# Chunk 796 notes

Tally: 3 RELOCATION_MATCH (0x008315C0, 0x00E80F10, 0x00E55CE0), 3 parked DIFFER (0x008E18D0, 0x008E9B70, 0x00951FB0).
All six row names are proven (Ego_r.pdb mangled name + a retail caller/vtable or string). Two oracle labels are placeholders and are
wrong: 0x008315C0 `CThingAICreature::vfunc_4` = ConstructFromLevelScript, 0x008E9B70 `NCreatureMode::CModeStrafe::vfunc_2` = AnimUpdate.
Every source's header comment (Identity / Callees / Layout / Shape) carries the full evidence; this file is the summary.

## 0x008315C0 CThingAICreature::ConstructFromLevelScript -- RELOCATION_MATCH (13 attempts)
- Shape: the CThingTrackNode::ConstructFromLevelScript 0x00837E40 parser loop up to EndThing; the Save 0x008304A0 key vocabulary read back.
- Key spellings: each flag key uses its own block-scope CPersistContext and a block-scope named `CPersistTraits<bool>` into one
  function-scope bool; the CPersistTraits<bool>::TransferIn body is compiled in the TU (landed 0x006C9490 precedent) so every traits
  object shares the temp slot [esp+14h]. `MaxHealth = Health = ...` (Health stored first).
- Callees (PDB): CThingCreatureBase::ConstructFromLevelScript 0x00662930 (by name), CCountedPointer<CRelationshipsInit>::Reset 0x00834D30
  (out of line, declaration only), CStringParser / CPersistContext / CPersistTraits<bool>::TransferIn 0x00403FF0.
- Header: hdr/ CThingAICreature.h LEVELSCRIPT (nested CRelationshipsInit, RelationshipsInit +0x160), CThingCreatureBase.h LEVELSCRIPT.

## 0x00E80F10 NScript::CV_FishingCompetitionScript::CFishCompOrganiser::AskForFish -- RELOCATION_MATCH (10 attempts)
- Shape: the entry answer defaults to 1 and is nested under `answer == 1` so the refusal is the last block; `if (!CheckForRod())`
  negated (retail falls into the no-rod arm); `weight == BiggestFish`; prize band loop = VC's four-way unroll.
- FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ is required (without it: 1216 difflines).
- Callees (PDB): PlayCutscene 0x00E80D60, CheckForFish 0x00E81AD0, CheckForRod 0x00E81A60, GetBiggestFishWeight 0x00E81B40,
  CV_FishingCompetitionScript::RefreshCompetition 0x00E7FD50 (NScript namespace; one landed spelling misses it -> NAME_FIXES).
- pGlobalScriptDef 0x0143E90C: landed rows disagree (148 `CScriptDef*`, 15 `CDefPointer<const CScriptDef>`); the src keeps the
  majority spelling. Coordinator decision (NAME_FIXES row).

## 0x00E55CE0 NScript::CV_BookCollectingScript::CBS_Teacher::AskForBook -- RELOCATION_MATCH (8 attempts)
- Shape: one function-scope result byte; `X; if (answer == 1) {...} else {...}`; the cut-scene objects (hero handle, cast map, args map)
  in an inner block closed before the spawn; the region name an explicit `CCharString("")` argument (built after the new-expression).
- Both maps out of line via explicit-specialisation declarations (landed 0x00DC1770 spelling).
- Callees (PDB): GetBookSpecificArgs 0x00E57020, AdjustHeroReputation 0x00E56D10, BookReaction 0x00E566F0, CScriptBase::AddSpawnedFunction
  0x00CB7E50, std::map<CCharString,CCharString> ctor/dtor 0x009AC2D0 / 0x009AC310 (all misspelled or unbound in the repo -> NAME_FIXES).

## 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams -- DIFFER(1833v2073), 671 difflines, 12 attempts
- Residue: retail expands CVectorMap::LowerBound, the vector overflow copy and ~CParticleUpdateParameters only in the sky-beam block;
  the two beam loops call 0x0040F020 / 0x0076ADA0 / 0x006E2500. A fully inline LowerBound opt-in expands all three sites (d9).
- Header: CVectorMap.h INSERT, fable_containers.h STLP_SORT_FASTCALL (retail std::sort instantiations 0x0076C000 / 0x0076C1A0 are __fastcall).

## 0x008E9B70 NCreatureMode::CModeStrafe::AnimUpdate -- DIFFER(1699v1705), 85 difflines, 18 attempts
- Residue: eax/edx/ecx rotation at the three CAnimInit + AddCombinationAnim sites; retail spills PullBackAmount with fst, VC7.1 reloads it.
- Levers that worked: do { } while (0) around the aim-site CAnimInit (retail expands the ctor only there), split interrupted / not-loaded
  remove branches.
- Header: CThingCreatureBase.h ANIMATION, CCreatureActionBase.h INTERRUPTION, CTCWeapon.h LOADING.

## 0x00951FB0 CAIStateGroup_JackDragon::UpdateHover -- DIFFER(2229v2272), 528 difflines, 13 attempts
- Residue: VC7.1 sinks the to-enemy distance products (x*x, y*y, sqrt) below the landing-marker GetPItem/GetPos calls; retail computes
  and spills them first (the 12-byte frame gap). The first route push_back is not expanded (retail expands it, calls 0x0066E630 after),
  and the exit blocks after the hover-out check sit after the GoLeft arm.
- Levers that worked: same-TU GetSubDef<T> template body (landed 0x008E3300) under auto_inline(off) -> def in ebp, releases lose their
  null test (1127 -> 629); `__forceinline` HoverOutOf ctor and IS_THING_ALIVE; `bool valid = ...; if (!valid) { Terminate(); return; }`
  for the first exit only.
- Header: CCombatActionBase.h SOUNDCTOR, CTCDPhysicalObstruction.h CREATE.
- Local models (header debt, not done here): CJackDragonDef (MaxFirebeamDistance +0x30, MaxHoverLoops +0x44, HoverObstructionRadius
  +0x60), NCreatureMode::CModeDragonHover (0x80), the CCreatureAction_DragonFire* / DragonHoverOutOf action classes, CSoundPair, and
  a CTCPhysicsBase slot view (63 GetFacingAngleXY, 70 GetFacingVector). These belong in generated engine/ headers with retail branches.

## Names
- Proven (Ego_r.pdb + caller/vtable): all six rows and every callee listed in LABELS.tsv / NAME_FIXES.tsv.
- Descriptive: none in src/ or differ/ beyond the CAIStateGroup_JackDragon::EDragonState enum's empty body (the type name is Ego_r's);
  the test model structs (Mode, Dragon, Comp, Teacher) are test-only.
- 0x0122DEDC is the compiler literal `__real@00000000`, not a named global (five wrong landed spellings in NAME_FIXES).

## Header debt
- See 0x00951FB0 local models above. No default-path header edit was made.
