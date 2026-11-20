# NF137 notes (2026-11-20, round 10, PRIORITY slice 9 fourth pass)

## Counts (from STATUS.tsv / APPLIED.tsv)
- Entries (5): done 3 (q5021, q4993, q5030); done-already 0; rejected 0; blocked 0; deferred 0; open 2 (q4994 / q5140, one unit, partial:
  3 of 5 pointer binders ported, self waits).
- Rows: 10 changed, all RELOCATION_MATCH + test PASS (recheck_final.log). 6 ported onto shared headers (00956fe0 009551a0 009863e0
  00978fd0 00964360 0093dc10); 4 changed in place on HEAD row-local models (009109b0 0093d3a0 0093d800 0093db10; decision 6).
  Not-a-binder 0. Blocked 0. Not attempted 2: 0x00913F30 and 0x00964CA0 (ports; STATUS lists what each needs); the self row 0x008FD000
  waits for them (no new split).
- Header opt-ins: 6 overlays (hdr/README.md).

## Leaf-only self renames (both PDBs agree; exe scan: every landed referrer moves in this hand-back)
- 0x0093D3A0 IsEnemyRunningAway QAE -> ABE (claimed lock; outside the entry list, the const callee of the q5030 self row; one referrer 0x0093D800, moved).
- 0x0093DB10 CAIStateGroup_MeleeCombatNearby2::IsStillRunnable QAE -> MBE (no landed referrer).
- 0x00956FE0 / 0x009551A0 / 0x00978FD0 / 0x009109B0 / 0x0093D800: RENAMES section 1.

## Callee respellings (new PDB spelling replaces a stand-in; the spelling count of the address does not grow; checked with pdiff + callers)
- 0x00957170, 0x009572A0, 0x00954220, 0x008FCF40, 0x0097DB70, 0x0062E900, 0x008E7790, 0x008561A0, 0x00857F30, 0x008B7FF0, 0x012944D4
  (vftable), 0x008FD000 (PDB IBEABV form, already the majority spelling), 0x00987430, 0x00986ED0, 0x004C73D0, 0x00A01B50, 0x009793D0,
  0x00A55A30, 0x00964230, 0x0093D3A0, 0x0093D800 (RENAMES section 2 lists each caller row with the old and new symbols).

## Descriptive names
- None new.

## Local classes (all header-less, checked by grepping `class X` / `struct X` across rebuild/include)
- 0x00956FE0: CCreatureAction_BriarRoseForcePush (no header; HEAD row-local; landed sibling 0x009572A0 shape; base CCombatActionBase from its header).
- 0x00978FD0: CThingFilter_CanBeUsedAsCover (no header; HEAD row-local, kept).
- 0x00964360: CThingFilter_ObjectHasProperty / CThingFilter_HasFreeActionPoint (no header; HEAD row-local, gained operator() declarations);
  stand-in CAIStateGroup_StealStealableItem_Methods (HEAD token, no header) now sits on the retail engine/CAIStateGroup_StealStealableItem.h class (narrow rule).
- 0x009551A0: the HEAD stand-ins MazeRangedMethods / CCombatActionBaseRetail / ...Retail are dropped; the row is on the real classes (MazeRangedCombat + ThrowShotObject opt-ins).
- 0x0093DC10: stand-in CMeleeCloseMethods (HEAD token, no header) now derives from the shared engine/CAIStateGroup_MeleeCombatNearbyBase.h (_MOVE); its HEAD
  local models of C3DVector / CThing / CThingCombatGeometry / CIntelligentTargetView are gone.
- 0x0093D3A0 / 0x0093D800 / 0x0093DB10: changed IN PLACE on their HEAD row-local CAIStateGroup_MeleeCombatNearbyBase / Nearby2 models (only access and const
  qualifiers on the declarations; no new class, no new base). Porting them onto the shared header is a follow-up (the header _MOVE declares the same spellings).
- 0x009109B0: in-place respell on its HEAD row-local CThing model; the operator takes `const CThingObject&` and casts to `const CThing*` once. The full
  port off the local model onto the shared headers is a follow-up.

## Defined-symbol diffs (objdump -t, landing vs HEAD; tools/strong.py)
- Every changed row: the only strong (non-COMDAT) symbol is its own leaf. Everything else is shared-header inlines, pick-any COMDATs (template
  instances, vftables), or implicit members of header-less local classes that nothing binds. 0x00964360 at HEAD also defined its
  GetPBestThing stand-in strongly; it is now a COMDAT under the PDB template spelling.

## EH funclets
- No Unwind funclet includes any of the 10 changed rows (grep of rebuild/src/compiled for each row file name). No funclet to regrade.

## Decisions for the coordinator
1. vftable COMDAT ??_7CCreatureAction_ThrowShotObject@@6B@ (0x012944D4): after this hand-back it is defined by both 0x009566C0 (a row-local model with a
   placeholder slot v00) and 0x009551A0 (shared-header class). Both are pick-any with different contents, so the linker keeps whichever comes first.
   0x009566C0 should move onto the THROWSHOTOBJECT_CLASS opt-in (its ctor is the inline one there; NAME_FIXES 0x009566C0).
2. 0x00964360 own leaf: the PDB ?FindPStealableItem@CAIStateGroup_StealStealableItem@@ABEPAVCThing@@XZ needs its only caller 0x00964DA0 to move with it,
   and 0x00964DA0 also binds 0x00964CA0. One family: 0x00964360 + 0x00964CA0 + 0x00964DA0 (NAME_FIXES).
3. q4994 / q5140 rows left: 0x00913F30 and 0x00964CA0 ports (STATUS has the recipe; 0x00964360's BESTTHING_BODY + ALLTHINGSBODY recipe matched first time).
   Then the self row 0x008FD000 can be respelled.
4. vland WARN RELOC_CONFLICT on 0x00956FE0 is NEW with this respell: the row now binds ??0CCreatureAction_BriarRoseForcePush@@QAE@AAVCThingCreatureBase@@0@Z
   to 0x009572A0 (its PDB address), while landed 0x008E8830 binds the same name to 0x008E8C50. The misbinding in 0x008E8830 is pre-existing
   (0x008E8C50 is ??0CCreatureAction_BriarRoseInvulnerableTeleport...; NAME_FIXES 0x008E8C50); respelling 0x008E8830 clears it.
5. 0x0093DC10 own PDB leaf (…AttackWhenVeryClose@@MBE_N_N@Z) needs a retail-class opt-in for that header (NAME_FIXES).
6. Four rows keep HEAD row-local models of header-backed classes, with qualifier-only edits (vland WARN SHARED_HEADER_AVAILABLE):
   0x0093D3A0 / 0x0093D800 (private const / protected virtual const on the local CAIStateGroup_MeleeCombatNearbyBase), 0x0093DB10 (`virtual`
   added on its local base's IsStillRunnable; the base already had a vptr, so no layout change) and 0x009109B0 (operator takes
   `const CThingObject&` and casts to `const CThing*` on the local CThing model). No class was added. Since the nf95b landing held rows over local
   models, please decide whether these land as they are or wait for a port onto engine/CAIStateGroup_MeleeCombatNearbyBase.h _MOVE (which already
   declares both PDB spellings) and onto the CThingObject / CThing headers.

## Needs header claim
- None. No existing shared-header member was renamed.

## Flags lines
- None.

## Locks
- 0x0093D3A0 claimed (mkdir), outside the entry list, for the q5030 family.

## Rule slips
- 0x00956FE0 went past the 3-failed-forms limit: v1 SRC_FAIL (include order), v2 SRC_FAIL (C2440 / C2039), v3 DIFFER (381 v 389,
  evaluation order); v4 (HEAD's named locals) matched. It should have been marked blocked after v3. 0x00978FD0 matched on its 2nd form
  (v1 SRC_FAIL, a missing STLport opt-in); 0x009551A0 on its 2nd (v1 leaf turned UAE on a stand-in).
- One Bash call with `sleep 60` was blocked by the harness; I used the background-task notification instead.
- q4994 rows 0x00913F30 / 0x00964CA0 were not attempted (context kept for q4993 / q5030).

## recheck_final.log (all 10 RELOCATION_MATCH + PASS) warnings
- SHARED_HEADER_AVAILABLE on 0x009109B0 / 0x0093D3A0 / 0x0093D800 / 0x0093DB10: the HEAD row-local models, changed in place (no class added). See decision 6;
  strict mode would reject them as they stand at HEAD too.
- RELOC_NAME on 0x0093DB10 (?GetRegionAtPosition@CThing@@... -> 0x00661D20, labelled CThingCreatureBase::GetNavigationLevelAt): HEAD's spelling, untouched.
- RELOC_CONFLICT on 0x00956FE0 (decision 4: new with the respell; the cause is 0x008E8830's pre-existing misbinding). STANDIN_LEAF on 0x0093DC10 (decision 5): pre-existing.
