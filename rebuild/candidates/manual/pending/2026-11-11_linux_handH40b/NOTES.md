# Chunk H40b (Fable 5.2, second pass) -- the 35 longest parked rows: hand-back notes

All grades were made with `FABLE_HDR_OVERLAY=$A/hdr` (hdr/README.md). This pass ran in two sessions (a container restart in between);
ATTEMPTS.log holds every grade of both. Result: **4 rows RELOCATION_MATCH + test PASS** (g++ -std=c++03 and VC7.1 /Od /W3), 8 more rows
worked with an updated differ/ draft and a measured diagnosis, 23 rows not reached (drafts untouched; STATUS.tsv). No row went over the
10-grade budget (most used: 0x00E3EB10, 8). The final `gall.sh` regrade of the four src/ rows is not in ATTEMPTS.log (it is the
required closing check; all four RELOCATION_MATCH).

## Matched rows
| row | identity (two sources) | grades | lever |
|---|---|---|---|
| 0x00E3EB10 NScript::CV_BordelloScript::CBordelloLady::Main | Ego_r.pdb + FableWin.pdb `?Main@CBordelloLady@CV_BordelloScript@NScript@@EAEXXZ`; entity vtable 0x012E3AC8 slot 1 | 8 | the "natural-exit ~seh_me store order" was not the scheduler: c2's late cross-jump had merged two expanded copies of the dtor. Main loop as `for (;; NSF) { if (T) return; ... }` + in-body `#pragma inline_depth(0)` at the first in-loop thread-exit `return` (retail calls ~seh_me there: 0x00E3ED00 `jne 0x00E3F740`) |
| 0x00DFE0F0 NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Main | both PDBs; entity vtable 0x012DEC68 slot 1 | 4 | the begging loop as `for (;; NSF) { if (T) return; if (talked) break; ... }` with the rescue code after the loop (no label, no extra return): retail's epilogue survivor and the whole TRADERB / TRADERC arm merge; one in-body inline_depth(0) at the thread-exit return after the loop (0x00DFEA3E `jne 0x00E005D5`) |
| 0x00D8BCD0 NScript::CQ_HerosOldHouseScript::CFishermansWife::Main | both PDBs; entity vtable 0x012D50DC slot 1 | 2 | frame-packer tie: `while (!T) { ... }` instead of `if (T) return; do { ... } while (!T);` is one ~WifeTimer reference fewer, so the four 1-byte flags sort before the timer ([esp+13h] / [esp+14h]) |
| 0x00E84B80 NScript::CV_FisticuffsClubScript::CFisticuffsMember::Main | both PDBs; entity vtable 0x012EB0F8 slot 1 | 2 | source order of the switch cases: LEAVING_FC written second (enum order), so the pads of the returns after it are laid behind it, before the shared tail |

The three sources that carried a bare `__forceinline` from their old drafts now spell it `FABLE_FORCEINLINE` (fable_compiler.h); regraded.
The matched rows still use the predicate lever of chunk H40 where their draft had it (0x00E3EB10, 0x00D8BCD0: FABLE_RETAIL_CPREDICATEBASE_NOVTABLE,
landed; neither retail body stores the base vtable 0x012C2FF4).

## Extents (SPLITS.tsv; finish.py cuts the oracle there)
- 0x00D0F640: the function is 2101 bytes (ret 0x00D0FE5A + the 0x00D0FE5B pad to 0x00D0FE75, 11 int3, next function 0x00D0FE80); the catalog row's 8944 bytes run on.
- 0x00B10B10: 4276 bytes (body + 4-entry jump table 0x00B11BB4, int3 from 0x00B11BC5); the catalog row's 9184 bytes run on.
`q.py` grades against the catalog extent and prints DIFFER(ours v catalog) for these; tools/md.py honours SPLITS.tsv.

## Unmatched rows worked (differ/<addr>.cpp, line 2 = measured residue, line 3 = next idea)
- **0x00E64FB0** (8 lines, was 461 on HEAD / 16 with its old overlay): header debt cleared. What is left is ONE expression-temporary
  rotation step: at the third FadeScreenOut (0x00E6716A) ours has the rotating pointer at eax, retail at ecx / edx (c2prio --rotation,
  file line 284). Ruled out by probe (no grade): `CRGBColour(0,0,0)` through Initialise, an inline helper returning the colour. Retail
  uses eax for the vptr right after the colour pointer in other functions (0x00F11EDA, 0x00D60BF8), so eax is not "busy": the pointer
  state differs, i.e. one more rotation temporary between 0x00E6713A and 0x00E6716A that leaves no code.
- **0x00D5AE70** (127 lines, one cause): the three tick slots. Frame-packer reading in the file's line 3: retail's slot sharing proves
  the creation order 3rd, 1st, 2nd tick (weights w3 > w1 >= w2 >= w3-2); ours counts 10, 10, 8 for 2nd, 3rd, 1st. Twenty-odd earlier
  grades permuted declarations without this measure; use tools/pk2.sh / pkw.sh (17 s, no grade) from here.
- **0x00D0F640** (229 lines at extent 2101): the ~vector<CScriptThing> call / expand split. tools/inl.sh gives every site's c1,
  frequency and score: with the real inline dtor all 14 sites score 45..68 under T 74 and expand. Retail's set (expand: the four
  loop-level exits and the loop end; call: the two nested exits and the whole massacre block) needs lower frequencies: roughly the
  outer loop without its x5 and the massacre block one more x0.5. Measured: `if (T) return; while (true) { ...; NSF; if (T) return; }`
  HALVES every frequency of the body (the first form that makes c2 call a site: the nested exit of loop 1, score 82), while
  for(;;)+bottom check, do-while(!T), do-while(true), a goto loop, if / else instead of break and do { } while (0) round the thread
  checks leave them unchanged. `/d2inlT52` is not the answer: it changes the callee (c1 29, destroy loop called).
- **0x00E6BC50** (21 lines), **0x00EC8300** (91 lines, size equal): exits solved in session 1 (the for (;;) form / an in-body
  inline_depth(0) return); what is left is register colouring (see each file's lines 2-3 for the c2prio numbers).
- **0x00DBB310** (141 lines): regraded only; per-site ~CScriptThing split (same family as 0x00D0F640: use tools/inl.sh).
- **0x00E5FD40** (1057 lines) and **0x00B10B10** (794 lines at extent 4276): header debt cleared, drafts compile on HEAD again; residue as their old chunks measured. The predicate lever is byte-neutral on 0x00E5FD40.

## Rows not reached (drafts untouched; STATUS.tsv)
0x00F0FB70, 0x00E40E80, 0x00E20600, 0x00E4B5B0, 0x00E3BB70, 0x00E50FD0, 0x00D973D0, 0x00D3E2E0, 0x00E33EA0, 0x00C4F600, 0x00E2FF00,
0x00CF68C0, 0x00D647F0, 0x00DB97A0, 0x00B03CA0, 0x00DF6010, 0x0078FD60, 0x00507C30, and the header-debt rows 0x00D13580, 0x00502E90,
0x006096E0, 0x0061E6D0, 0x00603000 (their overlays are not re-based). For the next pass: 0x00E40E80, 0x00E4B5B0 and 0x00D647F0 have a
CIsThingAlive / CIsThingConscious predicate and no base-vtable store and have not had the predicate lever yet (tools/lever.py, one grade each).

## Family reading
- Before reading any "store order", "alignment" or "arm merge" residue of a script Main, run tools/jt.py (jump-target diff) and
  tools/sz.py: the masked diff hides branch targets, and three of the four matches were a wrong jump target.
- Loop spelling decides which epilogue survives (LESSONS.md); the entry check's target (first v last epilogue) tells which form retail had.
- Call / expand splits are measurable per site with tools/inl.sh; frame-slot ties with tools/pk2.sh; rotation steps with c2prio --rotation.

## Tools (tools/)
First-pass tools are unchanged (pg.sh, md.py, sweep.sh, lever.py, hmerge.py, final.py, setnote.py, cond.sh, tvc.py). Added in H40b:
`b.sh` (grade + jump-target + size diff), `jt.py` / `jt.sh`, `sz.py` / `sz.sh`, `so.sh` / `so2.sh` / `xj.sh` / `w1.sh` (scheduler DAG,
late cross-jump inputs, tuple-link watchpoints), `pk.sh` / `pk2.sh` / `pkw.sh` (frame packer: slots, node order, weight trace),
`inl.sh` / `inl2.py` / `inlf.py` (inline decisions: hand883 inl.py through tools/sched/host.py), `prio.sh` (c2prio).
`hmerge.py` caveat: it anchors a block after the nearest unique line and can drop it INSIDE a neighbouring `#ifdef`; check the nesting.
`prio.sh` caveat (fixed): it runs from the repo root, so a relative output path landed in the repo's work/ once in this session; the
stray file (work/e64.prio, untracked scratch) was moved out at once and the script now makes the path absolute. Nothing else was written under the repo.
