# Naming pass, wave 6 (one agent; the chunk-562 authoring agent shares the lane)

Landed: operator=, waves 1, 2A, 2B, 3, LowerBound, 4A, 4B, 5 (wave 5 is applied in the repo WORKING TREE, being committed:
fable_string.h now has GetAsCharArray / FindCharPos / Sprintf / SetLength, AllocStringData protected). Records:
`rebuild/candidates/manual/naming/2026-10-2*`. Owner decision (2026-10-29): wide text is `unsigned short` (`G`); never
`/Zc:wchar_t`. `rebuild/oracles/reloc_bindings.tsv` no longer has target-less rows: the stock loader works again, so
RELOC_CONFLICT checks are live in your rechecks.

## Scope, in this order (stop at ~500 edited rows)
1. Existing RELOC_CONFLICTs between landed rows (one symbol, two targets: one side is misnamed). Settle each from
   evidence (PDB name sets, vtables/RTTI, call sites), fix the wrong side's source, and say which binding row changes:
   - `?Apply@CUserProfileManager@@QAEXXZ` 0x0040BC80 (row 004189c2) vs 0x0040A740 (row 0040b050; chunk 561 proved 0x0040A740
     is CUserProfileManager::Save)
   - `??1CComponent@NUISystem@@UAE@XZ` 0x0052CCF0 (row 0053b7f0; chunk 561: 0x0052CCF0 is ~CChangingStateComponent) vs
     0x00532D90 (row 0052d770)
   - `??0CAction@NUISystem@@QAE@XZ` 0x0042BED4 (row 0059de76) vs 0x0042BE50 (row 0056f615)
   - `?GetPlayerManager@CTCBase@@QAEPAVCPlayerManager@@XZ` 0x00686D50 (row 007b0950) vs 0x00686D40 (row 007fbc90)
   - `?GetPlayer@CThing` 0x004C7A10 vs 0x004C7A30; `?GetRegion@CWorldMap` 0x004FB490 vs 0x004FC180;
     `?GetInstance@CFrontEndManager@NUISystem@@SAPAV12@XZ` -> 0x0041E5F2 from 00594f94 (should be 0x00595582)
   - `_$E2` (compiler static-init name) bound to 0x0122A940 by 009f0240 while 005eb3a0 / 007e5ff0 reach 0x01229170 /
     0x01229DE0: decide whether `_$E2` should be bound at all (it is TU-local); propose the bindings change, edit nothing
     for it.
2. Chunk 561's 55 NAME_FIXES (`naming/w6/QUEUE_561.tsv`: chunk, address, correct_symbol, evidence, wrong_spellings_seen):
   merge into DECISIONS like wave 1 did, check each against the PDB name lists, apply the proven ones.
3. `naming/w5/DEFERRED.tsv` (122 rows, each with a reason): the cheap ones; CCharString::AllocStringData 0x0099EA60 `IAE`
   (header change only if every includer is rechecked); `engine/CDataOutputStream.h` member-function section via
   `tools/decomp_pipeline/gen_class_headers.py` (patch + regenerated header, every includer rechecked) so 00993f00 can
   include it.

## Rules and hand-back
As `naming/BRIEF_W4.md` / `BRIEF_W5.md` (idempotent `fam_*.py` / hand scripts + verified file lists, a composed overlay
re-made from the CURRENT repo before the final recheck, BINDINGS.tsv with RETIRE / KEEP / RETARGET / ADD and "not yet
bound" notes, LABELS.tsv with proven names only, DEFERRED.tsv, RENAMES.tsv, recheck logs; final message = report). Work
only in `naming/w6/`. Never run git; never write under /home/user/FableDecomp; never kill a process; no recursive
find/glob over `/` or the whole scratchpad.
