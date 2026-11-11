# Chunk H40c (Fable 5.2, third pass) -- the 31 longest parked rows: hand-back notes

All grades were made with `FABLE_HDR_OVERLAY=$A/hdr` (hdr/README.md; the overlay is the H40b overlay, no header file changed in this pass).
Result: **3 rows RELOCATION_MATCH + test PASS** (g++ -std=c++03 and VC7.1 /Od /W3 through tools/tvc.py), 4 more rows worked with an
updated differ/ draft and diagnosis, 24 rows not reached or out of budget (STATUS.tsv). 12 grades in all (ATTEMPTS.log); no row went over
the 10-grade total (the three matches stand at 5 / 6 / 6). The closing `gall.sh` regrade of the three src/ rows is not in ATTEMPTS.log
(all three RELOCATION_MATCH). Nothing was written under the repo; no git command was run.

## Matched rows
| row | identity (two sources) | grades (all passes) | lever |
|---|---|---|---|
| 0x00E64FB0 NScript::CV_ChickenKickingScript::CChickenMaster::Main | Ego_r.pdb + FableWin.pdb `?Main@CChickenMaster@CV_ChickenKickingScript@NScript@@EAEXXZ`; entity vtable 0x012E747C slot 1 | 5 | the "one rotation step" at the third FadeScreenOut was a statement pair retail's source has TWICE: both arms of the first-talk walk test end with their own `ParentClass.PlayCutscene("CS_CHICKING_INTRO"); Game->FadeScreenIn();`. c2 cross-jumps the two identical tails into the copy at 0x00E6743E after code generation, but the first arm's copy (generated first) has already taken one step of the eax / ecx / edx rotation for its FadeScreenIn vptr |
| 0x00D5AE70 NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main | both PDBs; entity vtable 0x012D09D8 slot 1 | 6 | two independent causes. (1) frame slots: the first-person round has its OWN middle tick variable (the unsheathe / fire ticks are reused): the packer then creates fire tick (10), unsheathe tick (8), lock-target tick (5), and the view tick + commentCount + multiCounter join that last slot = retail's [esp+44h]; probed with tools/pk2.sh before the grade. (2) a rotation step over 0x00D5D2E6..0x00D5D76B (hidden by the slot noise in the earlier 127-line count): the grade loop index is declared and zeroed BEFORE `score` is read, so ecx (the index) is busy when the PMasterData temporary is chosen |
| 0x00D647F0 NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main | both PDBs; entity vtable 0x012D16C0 slot 1 | 6 | (1) the "lane register-allocation gap" (retail keeps the Game vtable in ebx across GetHero in the bow-round lightning line, ours spilled it) is a priority tie c2prio --blocks shows: a temporary's priority is w x K x cost with K = the number of candidates its block references; through FM_LINE the block has 11 (132) and loses to the `||` flag word (138), as the good-hit lines do in retail too; the line written out with its own conversation local (`long line = ...; conversation = line;`, coalesced, no code) has 12 (144) and wins ebx. (2) the 16 slot lines left: ONE counter variable for the three rounds (retail [esp+38h] for all three), not three |

The three sources spell the inline device `FABLE_FORCEINLINE` (fable_compiler.h) where their old drafts had a bare `__forceinline`, carry a
line-2 `// Inline-budget:` note for it, and 0x00D647F0 no longer has the predicate lever (it was byte-neutral there: graded with and without).

## Header opt-ins the matched rows need (all already in hdr/ from H40b, none new; default path = HEAD)
| row | header | opt-in |
|---|---|---|
| 0x00E64FB0 | engine/CScriptGameResourceObjectScriptedThingBase.h | `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN` (hand818 / 820, re-based in H40b) |
| 0x00E64FB0 | engine/NScript_CV_ChickenKickingScript.h | `FABLE_RETAIL_CV_CHICKENKICKINGSCRIPT_CHICKENMASTER` (chunk 818: the nested CChickenMaster, 0x20 bytes) |
| 0x00D5AE70 | engine/CQ_GuildTrainingSkillScript.h | new header (hand816 / 818), whole body under `FABLE_RETAIL_NSCRIPT` |
| 0x00D647F0 | engine/CQ_GuildTrainingWoodsDepartureScript.h | new header (hand810 / 814 / 817), whole body under `FABLE_RETAIL_NSCRIPT` |

## Names
- 0x0143E908: the landed spelling `g_UnprovenWideString_0143E908` is a placeholder; src/00e64fb0.cpp names it `g_ScriptFormattedText`
  (descriptive: the original is in no PDB name set, string or FSE entry). NAME_FIXES.tsv asks for the definer 0x01224C83, the other user
  0x00E81B40 and the thunk row 0x0122AE6A (`g_AutoTinyThunkObject_0143e908`) to be respelled to one name.
- 0x00C4F600 is CEAXManager::SetEAXProperty (chunk 878 identity; descriptive, no PDB symbol), not PropertySerializer_GetPropertyTypeCode.

## Unmatched rows worked
- **0x00E6BC50** (21 lines, 3 grades used, none here): what the H40b note called a rotation step is the allocator: the eax / edx
  loads of followers._M_start at 0x00E6C3C1 / 0x00E6C432 are pieces of one CSE candidate (c2prio --trace: #94 takes edx on a copy
  preference) and the loop pair is the tie i 100 v begin 92. The 0x00D647F0 lever is the model: one more candidate referenced in the
  loop-top block (K 2 -> 3) gives begin 100 v i 96. Five spellings probed without effect (differ line 3); the untried ones are listed there.
- **0x00C4F600** (85 md lines = three cases, 1 grade used, none here): nine 13-second rotation probes of the ALLPARAMETERS / switch-head
  spellings; the pointer stays at ecx at ROOM (retail edx). Lines 2-3 of differ/00c4f600.cpp.
- **0x00E40E80**, **0x00E4B5B0**: one grade each for the predicate lever the brief asked for: byte-neutral on both (the prologue already
  matched). Drafts unchanged, differ/ lines 2-3 updated.

## Family reading for the next pass
- Three "allocator / rotation / frame gap" parks closed here were SOURCE facts, each read from a free probe before the grade:
  a statement written in both arms and cross-jumped away (rotation listing), a variable count (frame packer), a declaration position
  (rotation listing), a block-local named value (priority shares). Before parking a script Main on colouring, run c2prio --rotation,
  --blocks and tools/pk2.sh and ask what source would give retail's numbers.
- The duplicated-and-cross-jumped statement also raises the CALLER SIZE the inliner sees without leaving a byte. The rows parked on
  "retail calls here what ours expands" (0x0061E6D0's line 3 looks for a 724..1385 size consumer; the movie ctor / dtor splits of
  0x00E3BB70, 0x00E50FD0, 0x00D13580, 0x00E40E80) should be re-read with that in mind: look for a tail retail could have had in
  both arms of an if / else (the `jmp` into the middle of the other arm, as 0x00E67141 `jmp 0x00E6743E`, is the trace it leaves).
- Out of budget, not graded: 0x00EC8300 (15), 0x00DBB310 (12), 0x00D3E2E0 (10), 0x00E2FF00 (10).

## Tools added (tools/)
`rot.sh` / `rots.py` (c2prio --rotation, entries by source line; compact runs), `vr.sh` (variant by exact replacement + rotation
entries), `c4v.sh`, `mdf.py` (the md.py hunks that are not a pure [esp+N] change: separates slot noise from the rest).
