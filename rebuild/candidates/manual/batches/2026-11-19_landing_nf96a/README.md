# 2026-11-19 landing nf96a: naming round 8, NF96 part 1 (28 rows: NF94's three blocked families)

NF96 (advisor decision, owner delegated) took NF94's blocked families, q11204 / q11230 and the follow-ups q11443..q11467; it stopped for
context with 14 follow-ups open. Records: `naming/2026-11-19_agentNF96_part1/` (selfcheck.log: 0 problems).

## Entries: 8 done, 3 blocked, 14 open (continued by NF96 part 2)
- **done:** q11369 (the anim-component sort TU), q11427 (the particle heap), q11339 (the GetCardDef 0x008F0B20 callers: 11 binders ported
  over 25 exe call sites), q11445, q11446, q11449, q11465 (self row 0x00445960 only), q11452 (label-table fix only, queued).
- **blocked:** q11204 / q11230 (one definition: see the advisor decision below), q11443 (self row 0x004DA27F lands here; binders 0x006DBA50
  and 0x006DAEC0 still need ports: both bound it wrongly before this change, so nothing new breaks).
- 0x004DA27F's name now has two sources (the STLport _M_erase body and its CDefString-tree callers 0x004DB88A / 0x006DBA50). 0x006DAEC0's
  tree type is unproven, so that address may be a folded body: its binding is not respelled until its tree is proven.
- The particle-heap comparator is `NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>::CParticleUpdateCCompareVector<unsigned long>`
  (Ego_r.pdb / FableWin.pdb). nf94's catalog name for 0x0076A590 says `CCompareVector`; queued as a catalog fix.

## Advisor decision (owner delegated): same-TU strong definitions
Retail's callers of GetCategoryIndexForTradeType (0x0059C6B0: `mov edx,ecx ; call ; mov ecx,edx`) keep a register live across the call,
which VC7.1 does only for a strong definition compiled earlier in the same TU; the header-inline pick-any form measured DIFFER (47v45,
57v55). So a row may keep a same-TU strong copy of a callee when (1) it uses the PDB spelling, (2) its body is the owner row's body verbatim,
and (3) it carries `// Fidelity: same-TU strong definition of 0x... (retail TU-mate)`. Pick-any stays the form where retail expanded the body
(q11224) or a grade shows the bytes do not change. One TU for the callers is a build-model change (ROADMAP). Applied to q11204 / q11230 and
the GetCardDef copies (0x008F0B80, 0x008F0BD0, 0x008F0EB0, 0x008F10B0; 0x008F2280 defines the PDB symbol with a stand-in body: the defect)
in NF96 part 2.

## Rows (28): 20 self renames (RENAMES_applied.tsv, 24 file moves) + 8 respellings
Recheck from the repo after landing (`logs/recheck.log`): MATCH / RELOCATION_MATCH, every test PASS. One `RELOC_CONFLICT _$E2` warning on
0x0121E010: the TU-local `_$E` leaf collision NF92 recorded (not a conflict between real symbols).

## Headers (8 opt-ins in 6 headers; token-identical without them; asserts pinned, 0 name mismatches)
CTCTavernGameCardBase.h `_ECARDGAMETYPE_VALUES` / `_ENTERGAME` / `_ENDROUND`; CTCTavernGameCardPairs.h `_ROUND_OVERRIDES`;
CTavernGameCardBaseDef.h `_HIGHLIGHT_COLOUR_BAD`; CCategory.h `_GETPREVIOUSOCCUPIEDSLOT`; CTCBase.h `_ADDTOGLOBALTCS`;
CGamePlayerInterface.h `_CONTEXTSENSITIVEITEMBUTTONS_TYPED`. Direct-includer recheck (1,323 rows): `logs/includer_recheck.txt`.

## Checks
Synced with FullDecomp-B first (B's hand3c landings, its GameCODA b11 landing and its CMatrix4x4 claim). `naming/rebind.py`: removed 26,
added 14; `bindings_multi_check.py` at baseline; funclets 1,355 checked, 0 broken; selfcheck 0 problems.

## Queued: `land1119nf96` (5) + the 0x0076A590 catalog fix; Account B's hand4d_b11 NAME_FIXES (2).
## Rule slips (NOTES): one needless regrade; two drafts lost their Leaf line (caught before grading); one grade without env.sh (NO_RESULT).
