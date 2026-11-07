# Chunk 825 notes

Three of the six rows match (RELOCATION_MATCH, relocations paired by hand with relpair825.py, no reloc_bindings.tsv conflict) and
three are parked in differ/. No header changed.

## 0x00E44980 NScript::CV_BordelloScript::WatchForHeroLeavingRegionWithBeer: RELOCATION_MATCH (new row, name proven)
- Name, two sources: (1) CMagicman::Main builds the spawned-thread name "ParentClass." (0x012C9934) + "WatchForHeroLeavingRegionWithBeer"
  (0x012E48E0) and stores 0x00E44980 as the CSpawnedFunc<CV_BordelloScript> member pointer (0x00E411E2, 0x00E42B1D; object ParentClass
  at +0x38, vtable 0x012E38C4). (2) The Ego_r.pdb / FableWin.pdb name sets list ?WatchForHeroLeavingRegionWithBeer@CV_BordelloScript@NScript@@QAEXXZ
  directly before IsHeroWearingBeard, as retail places 0x00E44980 directly before 0x00E44A40. engine/CV_BordelloScript.h already declares it.
- Shape: the landed CheckLampSlot 0x00E76CA0 loop: a named bool `IsRegionLoaded("Bordello") && BeerSetToDPad` (its CCharString
  temporary under a construction flag in ebx), NewScriptFrame, terminate check; after the loop a terminate check, clear BeerSetToDPad
  (+0x70), SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, -1) (slot 312).
- Label: the manifest has the gapscan placeholder sub_00e44980 (LABELS.tsv).

## 0x0092FAB0 CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction: RELOCATION_MATCH (attempt d2)
- Chunk 795's residue was frame slots only (sub esp 1Ch v 24h). Two spellings fix it:
  1. The SetText button strings are implicit conversions, `SetText(font, confirmation, L"", L"TEXT_GUI_YES", L"TEXT_GUI_NO", 4, ...)`.
     VC7.1 builds implicit conversion temporaries before the argument pushes (no, yes, empty), each in its own frame slot. Named
     locals share slots (one landed in the dead `player` parameter slot), and explicit `CWideString(L"..")` temporaries are built
     interleaved with the pushes. The landed CTCTavernGame / CTCTrophy SetText callers use the same implicit spelling.
  2. In case 0, `phrase` lives in its own block and a `spoke` flag is tested after it
     (`if (AIAction_SpeakToTarget(...) == SUCCESS) spoke = true;` / `if (spoke) { NextReactionFrame[0] = ...; break; }`). The
     GetConstantFPS int temporary then reuses phrase's slot, as in retail. `spoke = (... == SUCCESS);` leaves a sete.
- Callee labels still wrong in the repo (NAME_FIXES.tsv): 0x006588CF GetCostToBribe, 0x006CEAE0 ResetLastOpinionReactionFrame,
  0x00798D30 GetHint (chunk 795 evidence, rebound by this row).

## 0x00F0C140 NScript::CQ_AmbushTradersScript::PhaseTwo: RELOCATION_MATCH (attempt a6)
- The region loop is `for (;;) { if (IsActiveThreadTerminating()) return; if (QuestFinished || QuestFailed) break; ...; NewScriptFrame(); }`.
  It replaces the separate terminate check after the bandit line (a2: 243 -> 19 lines).
- `long i = 0;` comes before the `if (count > 0)` guard, so the index store comes before the jle (a5). Dropping the guard instead
  gave 421 lines (a4).
- `long teleport_count = TeleportInfo.end() - TeleportInfo.begin();` VC7.1 expands it at this site, while `TeleportInfo.size()` was
  called out of line (`lea ecx,[ebp+1C4h]; call size`). The two are the same STLport arithmetic.
- 0x00F0CA30 is still landed as SThreeWordRecord::Swap (NAME_FIXES.tsv; the row binds the PDB vector<TeleportData>::swap spelling).
  0x00CDD0D0 ~vector<TeleportData> and 0x00F0CA30 are not yet in reloc_bindings.tsv.

## 0x007C4430 CTCHealLife::StartHealingEffect: DIFFER(1980v1952), 222 lines (parked, base source)
- Residue (unchanged from chunk 794): the loop's `HealthIncreaseAccum.push_back(0.0f)` is expanded where retail calls 0x00568850. Then
  the list end stays in a register (retail spills it to [esp+44h]), esi/edi swap, and the switch-case CCharString slots differ.
- 13 attempts. The loop forms (for(;;) with break, while, for(;;++it), it++, end declared before the for), the implicit
  push_back(0), push_back(float()) and `&thing == target` all compile to identical bytes.
- Probes b13-b15: a condition between the two loop push_backs that VC7.1 folds away (`if (!target) continue;`, target is known
  non-null there) outlines the float push_back and gives retail's exact 1952 bytes. It also moves `this` from ebp to edi and reshuffles
  the frame (254 lines), so the outline really is a static-frequency effect. The genuine retail condition is not known. Explicit
  CCharString case temporaries on top of that made it worse (b16).

## 0x00EE57B0 NScript::CV_TourGuideScript::CTourGuideGuide::Main: DIFFER(4356v4246), 235 lines (parked, attempt a3)
- a2 (the main loop as `for (;; NewScriptFrame()) { if (T) return; ... }`) and a3 (a row-local inline ~CScriptThing) give retail's
  pad structure (271 -> 235).
- What is left is register allocation. Retail keeps both CTimer indices in ebx/ebp and caches -1 in ebx; ours caches 0 / 0x12 in
  ebp. So two DeregisterTimer pads differ in register order and are not cross-jumped into retail's single 0x00EE65D9.
- a1 (inline CTimer ctor/dtor, 304), a4, f1 (Initialise wait as for(;;){if(Start) break;...}) and f2 (closing-time block as a break to
  a post-loop tail; identical bytes) did not move it. The test differ/00ee57b0.test.cpp is new.

## 0x00DEA3F0 NScript::CQ_RansomVictimScript::CVictimsBrother::Main: DIFFER(3056v3076), 161 lines (parked, base source)
- This is the brief's CIsThingAlive copy-inline blocker (GOTCHAS chunk 822). Retail expands ~CIsThingAlive for the predicate
  temporary but calls the nested ~CScriptThing 0x004AA840. No spelling splits them: /d2inlT60 gives neither and T80 both (diagnostics
  only, never kept).
- Ideas tried that are not in GOTCHAS: __forceinline ~CIsThingAlive (a1, 172: right slot, but ~CScriptThing expands) and a named
  block-scoped CIsThingAlive (e1, identical bytes). Also tried: second-loop forms (a4 identical, a5 298) and no row-local ~CScriptThing (185).

## vland (read-only, final overlay)
- 0x0092FAB0 and 0x00F0C140: RELOCATION_MATCH + PASS. 0x00E44980: OUTSIDE_MANIFEST (the manifest has no entry for the gapscan row; q.py grades it RELOCATION_MATCH and tall.py PASS). RELOC_NAME warnings only for the label corrections in LABELS.tsv.

## Header debt
None.
