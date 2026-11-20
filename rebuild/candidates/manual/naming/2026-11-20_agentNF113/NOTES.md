# NF113 notes (round 10, PRIORITY reconciliation slice 4; 32 queue lines over 30 addresses)

Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log.
- Entries (32): 18 done, 3 done-already, 1 rejected, 3 blocked (family, each with a graded staged self row or binders: q4272, q4716, q4950),
  4 deferred (held by NF111: q4187, q4188, q4189, q4192), 3 open (q4468, q4719 analysed with no form graded, q5201).
- Rows: 32 ported (rows/), 14 not-a-binder, 19 blocked (7 string-family rows held by NF111, 2 staged self renames, 10 others; see STATUS).
- recheck_final.log: one vland --recheck-landed run over the 32 rows/ rows: 7 MATCH + 25 RELOCATION_MATCH, every test PASS. selfcheck.log: 0 problems.
- Final-log WARNs (all reviewed): 73 SHARED_HEADER_AVAILABLE on local models the landed rows already had (in-place respells), 10 RELOC_NAME on
  callees whose catalog labels RENAMES s1 / NAME_FIXES change, 3 STANDIN_LEAF on the section-1 renames whose catalog name changes at landing.
  No RELOC_CONFLICT, no "graded the largest function".
- 0x0057BC6E (q4716 / q4950) is a binder-only hand-back: Died 0x006D0020 and Unmarry 0x006CFF10 move to the claim; the self row is staged
  in work/; Divorce 0x006D0180 is left.
- One header opt-in: engine/CTCWeapon.h `FABLE_RETAIL_CTCWEAPON_ISPROJECTILEWEAPONLOADED` (hdr/README.md). It sits in the GENERATED default
  branch: a header regeneration (gen_class_headers.py) would drop it, so the coordinator should carry it over (or move the row to the retail branch).

## Claims settled from the evidence column
The land0511 entries (q6234, q6390, q6392, q6397, q6460) carry the wrong spellings in correct_symbol and the claim in evidence. q6390:
`GetAnimationTransitionInTime` is in no name source; Ego_r.pdb 0x0063BE20 / FableWin.pdb 0x01FA0DA0 `?GetDelayTime@CAnimationEntry@@QBEJXZ`,
and the body returns the CAnimComponentDelayTime Delay (+0x08).

## Claims corrected or rejected (REJECTED.tsv)
- q4773: `list<CSliderAnimator>::clear` is FableWin's Dinkumware member. Retail STLport 4.6 defines clear() in _List_base, and list::clear is
  the inline `_Base::clear()`. The out-of-line COMDAT is `_List_base<CSliderAnimator>::clear`, the spelling both header-built callers already
  bind. That spelling was applied. The third REFS row, 0x005792DA (descriptive `list_T_005792da` dtor), was ported to
  `_List_base<CSliderAnimator>::~_List_base`, the spelling its caller 0x0057846F already binds.
- q6172: `@4@` back-reference typo; FableWin.pdb and every object spell `@5@` (done-already).
- q5230: the body is ReleaseData, not a destructor. NF75 (q10944, landed) settled `CCountedPointer<CScrollableList>::ReleaseData` from Ego_d.pdb.

## Other findings applied in passing
- 0x006556D3 was the stand-in `Destroy_range` / `Pair::destroy_with`, but its body is an assignment loop. Re-authored as the STLport
  `fill<CIntelligentPointer<CParticleEmitter>*>` instance, the spelling its only caller 0x006568F2 binds. It needs `// Flags: /Gr`.
- 0x005B4EBA CalculateArmorValues: 005b5cf8 / 005b62fe now bind the private const PDB spelling 005b6767 binds, so all 3 refs agree.
- 0x0057929A insert: 0x0057955A now binds the spelling 0x0057953E binds.
- The TransferVector self rows' callee bindings moved to the float / EOnlineScoreboardActivity instances (RENAMES s2). Each callee is
  referenced only by that row, or already bound that way by 0x00464CBF.

## Flags lines
- 0x006556D3: `// Flags: /Gr` (new; its build entry has none). Retail passes first / last in ecx / edx, as the landed 0x00BBD380 fill with
  `// Flags: /Gr`. The coordinator must carry it into CompilerFlags. fixflags.py patched my payloads.

## Staged, not handed back (no-new-split rule: a self rename waits for every REFS row)
- work/0057bc6e_ready.cpp (SetMarriageState, in-place private/const respell, RELOCATION_MATCH). It waits for 0x006D0180 CTCWife::Divorce.
  That row models a dozen classes locally. Recipe: the 0x006D1E30 preamble (engine/CTCCreatureOpinionOfHero.h already befriends CTCWife).
- work/005eb080_ready.cpp (SetStateAfterOK, nested enum + protected, RELOCATION_MATCH, no callee change). It waits for 0x007D62D0.
  That row's derived stand-in declares the base members, so it needs a port onto the CTCTavernGame / CardBase / CardPairs headers
  (recipe: 0x007D6870).
- q4719: the self rename (a 3-line row on CTCPerceiveThing.h AWARE_CALLED) waits for 0x0091DB50. That row's CTCTargeting stand-in is
  really CTCPerceiveThing: GetPerceivedThing 0x007446A0, CanSeeOrSmellPerceivedThing 0x00661600, IsAwareOfThing 0x00643B3B.
  0x00864600 pushes 0x00643B3B as a cast immediate (old row, no Leaf line, binds nothing).

## Open
- q4468: 17 stand-in binders (list in APPLIED). Each must include engine/CTCInventoryItem.h.
- q5201: possible fold. 0x00677F05 (not landed) may use the EInputKey instance; see APPLIED.

## Descriptive names
None new. Library spellings (descriptive-by-library, STLport 4.6 instance names): 0x0057955A push_back, 0x00578FCE _List_base::clear,
0x005792DA ~_List_base, 0x006556D3 fill. Kept the landed descriptive index helper `GetHeroStatIndex` in 0x0057A8DA unchanged.

## Funclets
No EH funclet row includes any changed row (grep of rebuild/src/compiled for the changed file names found none). The self renames move
these parent files, and none has funclets: 0x00633830 / 880 / A60 / BB0, 0x005EDE93, 0x00569F20 / FE0, 0x0056E545, 0x0057955A,
0x00578FCE, 0x005792DA, 0x0057A8DA, 0x0057AC89, 0x005B336A, 0x0065533C, 0x006556D3, 0x005DA980.

## Rule slips
- Three Bash calls exited non-zero: the selfsym read ending in `pgrep` (exit 1) and the b1 / b2 log checks ending in `ls -d wd*` (exit 2).
- Many Bash calls skipped the lane preamble (`source $S/wine/env.sh`); the grading calls went through tools/g.sh / pdiff.sh, which source it.
- One heredoc wrote an unexpanded `$S` into hdr/README.md; fixed before hand-back.
- g.sh ignores an `S=` override, so three gate-row probes compiled into pS instead of a separate directory. Harmless: every row is recompiled
  in recheck_final.log.
