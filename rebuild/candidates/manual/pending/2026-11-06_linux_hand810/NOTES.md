# chunk 810 notes (six gap-scan entity Mains; 0 matched, 6 parked)

All six names are proven (no descriptive names): `?Main@<Class>@<Script>@NScript@@EAEXXZ` is in both the Ego_r.pdb and FableWin.pdb
name sets, and each address is the only .rdata reference, slot 1 of the entity vtable the landed Alloc stores (LABELS.tsv).
Every parked source is in differ/<addr>.cpp (line 2 grade + residue, line 3 next idea) with a passing differ/<addr>.test.cpp.
Every attempt is in STATUS.tsv notes and ATTEMPTS.log.

## The shared residue: the CIsThingAlive / CIsThingConscious copy-inline lane gap
Retail expands the predicate wrapper's ctor/dtor (`SetPredicate(CIsThingAlive(Me))`) but CALLS the nested CScriptThing copy
0x004ABE90 / dtor 0x004AA840 inside it, while it EXPANDS the top-level by-value CScriptThing copies and temporaries elsewhere in the
same body. Our VC7.1 lane treats both alike: a visible (row-local inline or IMPLICIT_COPY) copy inlines at both levels; a declared-only
copy is called at both levels. Tried without effect on every row: named predicate local in its own block, __forceinline wrapper ctor,
FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY, definition order, definition position (VC7.1 defers inline expansion to the end of the TU).
Lowering / raising the inline budget (/d2inlT, /d2inlS, DIAGNOSTIC ONLY, never in a row) declines other sites first, never the
predicate copy: not a budget-order effect.

## Per row
- 0x00D8BCD0 CQ_HerosOldHouseScript::CFishermansWife::Main (fw_a11, 111 lines / SDIFF 20). Shape: GhostFisherman model,
  implicit movie ctor/dtor + declared-only ~CCountedPointer<MovieBase>, FORCEINLINE pause ctor, inline CTimer, forceinline GetPos,
  row-local inline CScriptThing copy + ~CScriptThing (needed for the by-value SetIsPushableByHero arg and the first-loop GetHealth
  temps). Residue: the predicate only (lane gap) + 3 loop pads.
- 0x00EC8300 CV_SickChildScript::CManInLove::Main (mil_a3, 304 / SDIFF 101). Shape: CMansLover model, MovieBase OUTOFLINE ctor + dtor
  (retail calls 0x006E7B60 / 0x006E7B80 at every site). Residue: the 5 by-value Me copies are called (retail expands); the bl_a4 lever
  (row-local inline copy) or IMPLICIT_COPY inlines the predicate copy too AND flips ebp from &Me to 0 across the whole body (1404
  lines); the talk-block first thread check exits into the expanded seh_me epilogue (retail calls 0x007E74D0); eax/edx swaps.
- 0x00E3EB10 CV_BordelloScript::CBordelloLady::Main (bl_a4, 51 / SDIFF 17). Shape: CBordelloGuard includes, ScriptedThingBase
  OUTOFLINE_ASSIGN, MovieBase FORCEINLINE_CTOR + declared-only ~CCountedPointer<MovieBase>, the text-eq opt-ins, the GetLHTSTag opt-in,
  each else arm opening on its own thread check, row-local inline CScriptThing copy (4 by-value Me args + sex-counter tail cross-jump
  now match). Residue: only the predicate copy (+16 B, lane gap).
- 0x00D1B200 CQ_BountyHuntScript::CBH_GangLeader::Main (gl_a10, 172 / SDIFF 49). Shape: CBH_GangDeputy2 model + the FW cutscene cast
  map, __forceinline STB operator= over declared-only CCountedPointer<STB>::operator= 0x008AB1E0, `HealthCurrent=HealthInitial=`
  (fst +0x24 / fstp +0x28), MOVIEBASE_FORCEINLINE, `BountyHuntRewardObject.GetString()` spelled out at both sites with
  FORCEINLINE_GETSTRING. Residue: retail expands the implicit movie dtor at all 7 sites (function-wide inline budget: the
  compiler-generated dtor runs out, 5 sites call the COMDAT); MOVIEBASE_FORCEINLINE expands every site but its user-declared dtor adds
  a MovieBase vptr store retail lacks (7 x 11 B); exit-block layout, 2 loop pads, two identical ~TeleportData exit copies in retail.
  DIAGNOSTIC ONLY: gl_a4 + /d2inlS20 (or S30) gives 129 lines (every movie dtor + GetString expand as retail) - confirms the budget.
- 0x00D56790 CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main (mo_a2, 836 / SDIFF 27; most diff lines are offset shifts). Shape: FW
  model, FORCEINLINE movie ctor + declared-only ~CCountedPointer<MovieBase>, CTimer ctor/dtor called (0x00CD4450 / 0x00CD4470),
  row-local __forceinline CTimer operator= / GetTime over pDSTGame, `HitCount = 0` before the timer set. Residue: the stage-3 counter
  is load / inc / store with the TimerIndex load scheduled between (ours `inc` mem); no spelling (x=x+1, +=1, ++x, named local,
  hoisted, declared first, CTimer operator<=) moves it.
- 0x00D62480 CQ_GuildTrainingWoodsDepartureScript::CArtifactThief::Main (at_a3, 168 / SDIFF 19). Shape: inline CIsThingConscious
  ctor + __forceinline dtor, inline ~CScriptThing, inline CTimer ctor/dtor + forceinline SetTimer/GetTimer, plain-inline movie ctor
  (the nested RETURN_QUESTION_NO site is called 0x006E7B60 as retail). Residue: the predicate's nested ~CScriptThing expands (retail
  calls 0x004AA840). at_a7 (declared-only ~CScriptThing) makes the predicate byte-exact but the hit-block ArtifactThiefRunMarker
  temporary (retail's only expanded ~CScriptThing site) is then called (599 lines). /d2inl variants worse (diagnostic only).

## Callee names (all bound spellings checked in reloc_bindings.tsv + landed sources)
- 0x004ABE90 CScriptThing copy ctor, 0x004AA840 ~CScriptThing, 0x006E7B60 / 0x006E7B80 MovieBase ctor / dtor, 0x00CD4450 / 0x00CD4470
  CTimer ctor / dtor, 0x008AB1E0 CCountedPointer<STB>::operator=, 0x00E403D0 CBordelloLady::GetLHTSTag (landed), 0x00D1C9E0
  WatchForLeaderGruntsKilled (landed), 0x00D1CAC0 WatchForLeaderChargeGruntsKilled (landed under a wrong name, NAME_FIXES.tsv).

## Labels to correct (NAME_FIXES.tsv)
- 0x00D1CAC0 landed as WatchForAllLeaderChargeGruntsKilled; PDB spelling ?WatchForLeaderChargeGruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ.
- 0x009AC700 bound as ??ACStringMap_009a8150@@ / ??ACStringMap@@ / ??ACCutsceneArgMap@@ / ??ACTextArgMap@@; PDB spelling
  ??A?$map@VCCharString@@V1@...@std@@QAEAAVCCharString@@ABV2@@Z.
- 0x0099EFB0 bound as ??4SymbolString@@ / ??4CCharString@std@@ / ??4CCharString@@QAEAAV0@ABV0@@Z; PDB spelling ??4CCharString@@QAEABV0@ABV0@@Z.

## Header debt
- None blocking. The older NScript entity headers model ParentClass as a reference where the PDB has a pointer (same codegen); the two
  new headers use the PDB pointer.
