# Chunk H40d (Fable 5.2, fourth pass) -- the 24 longest parked rows still open: hand-back notes

Every grade and probe was made with `FABLE_HDR_OVERLAY=$A/hdr` (hdr/README.md). Result: **1 row RELOCATION_MATCH + test PASS**
(g++ -std=c++03 and VC7.1 /Od /W3 through tools/tvc.py), 4 more rows worked with an updated differ/ draft and a measured diagnosis,
19 rows not reached (STATUS.tsv). 2 grades in all (ATTEMPTS.log) plus the closing `gall.sh` regrade of the one src/ row; no row is over
the 10-grade total. Nothing was written under the repo; no git command was run. differ/ still holds the H40c drafts of the four rows
that are out of budget and not in this chunk's ROWS.tsv (0x00EC8300, 0x00DBB310, 0x00D3E2E0, 0x00E2FF00): untouched.

## Matched row
| row | identity (two sources) | grades (all passes) | lever |
|---|---|---|---|
| 0x00E6BC50 NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main | Ego_r.pdb + FableWin.pdb `?Main@CCOE_AcolyteEvil@CV_ChapelOfEvilScript@NScript@@EAEXXZ`; entity vtable 0x012E8468 slot 1 | 4 | H40c's reading was right (a priority tie in the follower teleport loop: index 100 v vector base 92, K 2 in the loop-top block). The loop names the follower it compares -- `const CScriptThing& follower=followers[i]; if (!(victim==follower)) { if (T) return; Game->EntityTeleportToThing(followers[i],teleport,false); }` -- and the named reference is the third candidate of that block: base 100, index 96, so the base is coloured first (ebp) and the index gets ebx as in retail. The reference costs no instruction (it takes eax, the register retail's `lea eax,[edi+ebp]` uses), the second `followers[i]` is still re-read after the thread check (retail 0x00E6C4B3), and the three other residues of the old draft (the eax / edx pairs of the two size() reads, the register form of the thread-exit pad 0x00E6D92F, 37 bytes of pads) were consequences. Found with a 60 s c2prio table probe (tools/vt.sh), one grade |

The source spells the inline device `FABLE_FORCEINLINE` (fable_compiler.h) where the old draft had a bare `__forceinline` and carries the
line-2 `// Inline-budget:` note for it. It needs no new header opt-in: `FABLE_RETAIL_CV_CHAPELOFEVILSCRIPT_ACOLYTEEVIL` in
hdr/engine/CV_ChapelOfEvilScript.h is the chunk 814 / 835 block carried unchanged since H40b (hdr/README.md).

## Unmatched rows worked (no grade spent unless said)
- **0x00D0F640** CheckForSecondAreaMassacre (2 grades used, none here). The ~vector<CScriptThing> / ~CScriptThing call-or-expand split is
  now a solved constraint system, read from tools/inl.sh traces (22 s each): the vector dtor (c1 44) is called below site frequency
  46.4, ~CScriptThing (c1 60) below 38.1. Retail's set -- including the pair that pins it, the Gate3Guard temporary CALLED at 0x00D0FC0B
  while the leader temporaries behind the bandit loop are EXPANDED and the vectors at the same final return are CALLED -- has exactly one
  solution over for / while loop forms and 0.5-branch products: the three inner loops are `for` loops as drafted, the massacre block is
  reached with probability 0.25 (ours 0.5), and the flow in front of the first follower loop is 19.5..23.8 (ours 105.5: the main-loop
  header at 100 where every spelling we have gives 500, while (true) gives 250 and no loop gives 50). Main-loop spellings measured
  without any effect are listed in differ line 3. The sibling 0x00D0EE70 CheckForFirstAreaMassacre (parked in chunks 839 / 844 on the
  same blocker) has a DIFFERENT retail split for the same statements, with even the trivial vector<unsigned long> ctor / dtor called at
  its early sites (0x00D0F019 `call 0x00450C72`, 0x00D0F271 `call 0x0045019A`): whatever cools these two bodies is in front of the
  kill-count code and differs between the two functions. A device version is not available: the nine called returns also destroy
  `kills`, whose dtor retail expands there (0x00D0FE42), and inline_depth(0) would outline it.
- **0x00E50FD0** CBB_BeardyBaldyMan::Main (4 grades used, none here). Same family, smaller: two sites wrong (the attack scene's third
  thread exit and the phase-0 movie ctor, both expanded by us at f 7.8 / 7.0, called by retail), every other movie ctor / dtor site
  already on retail's side. A uniform 0.5 on the frequencies reproduces retail's split exactly. Five main-loop spellings measured:
  the draft's `for (; !T; NSF)` is the only one with the right order of magnitude (`while (!T) { ...; NSF; }` is NOT equivalent here:
  its sites run 4x hotter and alternate EXP / CALL). The "in-body inline_depth(0) at the movie declarations" idea of the old line 3
  is not needed.
- **0x00D13580** CQ_BountyHuntScript::Main (2 grades used, 1 here: the baseline on the re-based header). Six independent causes in
  280 lines (differ line 2). The one the H40c lead speaks to is the three exit-wait branches: retail's `call T ; je tail ; jmp exit`
  at 0x00D14E43 / 0x00D14EE4 shows that those two `return`s had their own cleanup copies at block layout (later cross-jumped into
  0x00D14FD5), which is why the three closing thread checks stay apart; ours merges them and moves the shared tail.
- **0x00C4F600** CEAXManager::SetEAXProperty (1 grade used, none here): read again, nothing to add to H40c's nine probes; note for
  the next reader: the three integer cases re-load `data` from the stack although ecx holds it from the switch head (0x00C51343),
  because ecx is needed for the `this` re-load once `setne bl` has taken ebx.

## Header debt
- Cleared: engine/CQ_BountyHuntScript.h (hand812 `_MAIN`), re-based by hand onto HEAD (HEAD's per-entity opt-ins kept, each nested
  class declared once whatever combination is on). 0x00D13580 compiles and grades again.
- Still open (not reached): the screen-builder / CWorldMap overlays of hand729 / 879 / 882 / 883 (0x00603000, 0x00502E90, 0x0061E6D0,
  0x006096E0). 0x0078FD60 and 0x00507C30 still have no source.

## What the size-consumer lead came to
Tested on 0x00E50FD0 and (same family) 0x00D0F640 with the inline trace instead of a grade. On both rows the caller-size term plays no
part (g stays under 9000; no penalty), so a statement that is cross-jumped away cannot move these sites: the sites are decided by the
static frequency alone, and what is missing is a factor on the frequency (0.5 on 0x00E50FD0, 0.2 + a 0.25 branch on 0x00D0F640).
0x0061E6D0 (where the caller is past 9000 and the lead does apply) was not reached.

## Family reading for the next pass
- c2's frequency of a script Main's body is NOT invariant under the main-loop spelling when the body has several arms that fall to the
  loop end (0x00E50FD0: `for (; !T; NSF)` 125 at the first site, `while (!T) { ...; NSF; }` 498, guarded do-while 249), but IS
  invariant when every path through the body passes thread exits in a straight line (0x00D0F640: six spellings, one number). Measure
  before assuming either.
- Two rows now need a source-level reason for a frequency factor that leaves no code (0.5; 0.25 on one branch). GOTCHAS chunk 890 s6
  documents the only known ones: a `&&` / `||` whose second operand folds, and a constant loop test. The script macros
  (thread check, speech block, start-scripting wait) are the place to look: one macro spelling would move a whole family.

## Tools added (tools/)
`vt.sh` (variant by exact replacement + the c2prio candidate table rows of a line range), `iv.sh` + `inlq.py` (variant + ordered inline
decisions: frequency, c1, score, EXP / CALL per site), `bd.py` (disassembly of the last q.py build, relocations masked), `rd.py`
(retail disassembly at a VA).
