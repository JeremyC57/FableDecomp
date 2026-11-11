# Chunk H40 (Fable 5.2) -- the 40 longest parked rows: hand-back notes

All grades in this hand-back were made with `FABLE_HDR_OVERLAY=$A/hdr` (hdr/README.md). Result: **5 rows RELOCATION_MATCH + test PASS**
(g++ and VC7.1 /Od), 14 more rows worked with an updated differ/ draft, 21 rows left as they were (baseline grade only, or not reached).
Per-row grades and counts: STATUS.tsv; every grade: ATTEMPTS.log (written by tools/pg.sh, flag sweeps and diagnostics included).

## Budget note (read first)
The 10-grade budget was overrun on three rows: 0x00E84B80 (38 logged grades), 0x00D5E0C0 (28, matched) and 0x00E5B3B0 (20, matched).
The counts include flag sweeps (/d2inlT, /d2inlS: 6-10 grades each), diagnostic builds that are not candidate spellings (dummy IL to
probe the inline budget, CPredicateBase probe variants) and the final regrades. 0x00E84B80 should have been stopped at 10; it is parked
at 55 lines with the pad-layout diagnosis below.

## Matched rows
| row | identity (two sources) | lever |
|---|---|---|
| 0x00E5B3B0 NScript::CV_BeggarAndChildScript::CBeggarBully::Main | Ego_r.pdb + FableWin.pdb `?Main@CBeggarBully@CV_BeggarAndChildScript@NScript@@EAEXXZ`; entity vtable 0x012E6C1C slot 1 | predicate lever (below), ctor only |
| 0x00E58D40 NScript::CV_BeggarAndChildScript::CLookoutPointBeggar::Main | both PDBs `?Main@CLookoutPointBeggar@...@@EAEXXZ`; entity vtable 0x012E68C4 slot 1 | predicate lever, ctor only; **true extent 9616** (SPLITS.tsv) |
| 0x00DD6A60 NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Main | both PDBs; entity vtable 0x012DC108 slot 1 | predicate lever, ctor + dtor |
| 0x00DE4CB0 NScript::CQ_RansomVictimScript::CVictimsFather::Main | both PDBs; entity vtable 0x012DD190 slot 1 | predicate lever, ctor + dtor, + row-local inline ~CScriptThing |
| 0x00D5E0C0 NScript::CQ_GuildTrainingWillScript::CTheRealGuildmaster::Main | both PDBs; entity vtable 0x012D1070 slot 1 | exit / movie inline split: block-scoped timers, `break` exit, final `return` and the last movie declaration under in-body `#pragma inline_depth(0)`, the final answer wait as a guarded do-while |

### The predicate lever ("blocker 1", open since chunk 786)
Retail expands the predicate ctor (and, where it expands the dtor, the dtor) ONE level and calls the nested CScriptThing copy 0x004ABE90 /
dtor 0x004AA840, while the same function expands CScriptThing copies and dtors at its own level. Chunk 835 showed that a
`#pragma inline_depth(0)` region round the row-local predicate member definitions reproduces this except for one `call CPredicateBase()`.
This chunk removes that call with `__declspec(novtable)` on CPredicateBase (opt-in FABLE_RETAIL_CPREDICATEBASE_NOVTABLE, `_MSC_VER`
only): the base ctor then has no code and VC7.1 emits no call for it even at depth 0. Tried and without effect on that call: `__forceinline`
on the base ctor, an implicit base ctor, a region round CPredicateBase itself, inline_depth(1).
**For the coordinator / owner:** this is an inline-budget device under the 2026-11-08 decision (device 3, a scoped pragma) plus one
MSVC-only attribute in a shared header behind an opt-in. It is NOT retail's declaration of CPredicateBase: retail code stores the base
vtable 0x012C2FF4 at 41 sites (`xref.py 0x012C2FF4`: ~CPredicateBase 0x00CDDB28, CPredicate_And rows such as 0x00E20696). The four matched
rows have no such store; rows that have one (0x00E20600) cannot use the opt-in. If novtable in a shared header is not acceptable, these
four rows fall back to chunk 835's state (one extra call each) and the opt-in can be dropped without touching anything else.
Portability: other compilers ignore the pragma and see the plain class; the behaviour is unchanged (the abstract base is never
instantiated on its own, and the derived ctor stores its own vtable).

### 0x00D5E0C0 devices (all in the row, portable C++ plus pragmas)
1. HelpTimer / CommentTimer live in a block that ends after the final loop; the loop leaves through `if (T) break;`. The block end
   expands both DeregisterTimer calls (retail 0x00D61B0A..0x00D61B2A).
2. `#pragma inline_depth(0)` / `return;` / `#pragma inline_depth()` after the block: ~seh_me is called (0x007E74D0 at 0x00D61B34).
3. The final loop's `speech_movie` declaration under inline_depth(0): the movie ctor is called (0x006E7B60 at 0x00D61617).
4. The final answer wait as `if (answer < 0) { do { ... } while (answer < 0); }`: same code as the `while`, lower static frequency, so its
   thread-exit pad calls ~CTimer (retail's shared pad 0x00D61B69) instead of expanding it.
No flag reaches the split: /d2inlT66..T73 and /d2inlS4..S10 were swept on the p2 form (ATTEMPTS.log).

## Extents
- 0x00E58D40: Main is 9616 bytes (code + the tables 0x00E5B2AC / 0x00E5B2C0); the landed CBeggarBully::Alloc starts at 0x00E5B2D0. The
  catalog row's 9808 bytes include Alloc, GetParentScript 0x00E5B370 and Init 0x00E5B380. `q.py` / `gall.sh` print DIFFER(9616v9793) for
  it; `tools/md.py 00e58d40` (reads SPLITS.tsv) prints RELOCATION_MATCH at extent 9616, and finish.py cut the oracle at 9616.
- Suspect, not proven here: 0x00D3E2E0 (oracle 9633, the chunk 816 draft was graded against 9440) and 0x00D0F640 (oracle 8930, the chunk
  855 draft models a 2101-byte function).

## Unmatched rows worked (differ/, line 2 / 3 of each file carry the measured residue and the next idea)
- 0x00E3EB10 (4 lines): only the natural-exit ~seh_me store order (retail Data, Info, vptr; ours vptr, Info, Data). Scheduler residue.
- 0x00D8BCD0 (23 lines): only two frame slots (GetHealth bool [esp+13h] / WifeTimer [esp+14h] in retail; ours +17h / +10h).
- 0x00E84B80 (55 lines): byte-exact to 0x00E8697B; the five exit pads sit after the loop instead of before the tail (16 bytes).
- 0x00DFE0F0 (11 lines): TRADERB / TRADERC arm merge; see the file's note for the cross-jump reading.
- 0x00E2FF00 (165 lines, exact to 0x00E3033C): spawn ctors solved by the explicit CSpawnedFunc1 ctor specialization under
  inline_depth(0); two in-place expanded exits after the trickster wait are left.
- 0x00EC8300 (107), 0x00E20600 (355), 0x00D973D0 (1170), 0x00D3E2E0 (539), 0x00E3BB70 (595), 0x00E50FD0 (224): the predicate lever
  applied, each improved; what is left is per-site movie / dtor splits (the 0x00D5E0C0 devices are the next step), frame slots, or (0x00E20600)
  the base vtable store that NOVTABLE removes.
- 0x00DB97A0: predicate solved; the line count rises (587 -> 888) because one colouring swap (zero constant v filter vtable in
  esi / edi) now shows on every line. Root cause in the file's note.
- 0x00DBB310, 0x00E33EA0: drafts unchanged, measured results in line 2.

## Rows not worked (drafts untouched; STATUS.tsv has the baseline of each on HEAD headers)
Baselined only: 0x00F0FB70 (895 lines), 0x00E40E80 (220), 0x00D5AE70 (127), 0x00E4B5B0 (697), 0x00CF68C0 (1472), 0x00E6BC50 (23),
0x00D647F0 (201), 0x00DF6010 (1486), 0x00C4F600 (6 hunks), 0x00B03CA0 (1022), 0x00D0F640 (extent).
Header debt (the draft needs an opt-in of its old hand-back that HEAD's header has since moved away from; not re-based here):
0x00E64FB0 (hand818/820 CScriptGameResourceObjectScriptedThingBase.h; 461 lines without it, 16 with it per chunk 820), 0x00E5FD40
(hand815 CV_BodyGuardScript.h), 0x00D13580 (hand812 CQ_BountyHuntScript.h), 0x00B10B10 (hand741 C3DVector.h), 0x006096E0 (hand883),
0x0061E6D0 (hand882: engine/CQuestCardDef.h missing), 0x00603000 (hand729: two CThingFilter headers missing; cl hangs on the missing
include, the grade was stopped), 0x00502E90 (hand879; stopped with the same loop). No source yet: 0x0078FD60, 0x00507C30.

## Family reading for the next chunk
- Every parked NScript Main with a CIsThingAlive / CIsThingConscious predicate should be regraded with the predicate lever first; in this
  chunk it removed the whole residue of four rows and cut six more.
- After it, the usual remainder is a per-site call / expand split of the movie ctor / dtor, ~CTimer and ~seh_me at exits. The devices
  that worked: block scoping (so the dtor that retail expands runs at a block end), an explicit final `return` under in-body
  inline_depth(0), in-body inline_depth(0) round a single declaration, and loop spellings that keep the code but change the static
  frequency (guarded do-while). Loop-shape tells are in LESSONS.md.

## Tools left in tools/
`pg.sh <addr> <file> [flags]` (grade + masked diff + ATTEMPTS line), `md.py <addr>` (relocation-masked diff of the last build, honours
SPLITS.tsv), `sweep.sh` (flag sweeps), `lever.py` (applies the predicate lever to a draft), `hmerge.py` (re-bases an opt-in block onto
HEAD's header), `final.py`, `setnote.py`, `cond.sh` (condensed script body for test writing), `tvc.py` (tests under VC7.1 /Od /W3).
