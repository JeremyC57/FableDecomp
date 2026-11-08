# Codex 2026-11-08 handoff v our best -- one comparator, regraded here

Received from the owner (Google Drive, Fable_2LargeRows_2026-11-08.zip, 12 MB). Kept here: rows/ (selected sources + header overlays +
per-row ROW.json / PROBES.md / TEST_STATUS.md), the top-level notes, RESEARCH/ (allocator evidence, frame maps, local tools) and the
unselected `slide_factor_compound` alternate for 0x00CBFB7D. Not kept (scratch only): HISTORY/ (87 MB of per-attempt snapshots),
VARIANTS/, BASE_HEADERS/ (old pinned header snapshots), PRIOR_HANDOFFS/ (copies of our own hand876/885/886 material).

Regraded with the lane's q.py (VC7.1 RTM 3077, base /O2 /Oy /W3, overlay = the row's hdr/): sizes reproduce exactly.
Same comparator for all four (capstone over the whole leaf v the retail oracle; difflib share of retail instructions matched;
Action compares code only, 0x0041E6D3..0x00426E0E, without its 1,032 jump-table bytes):

| Row | Source | Bytes v retail | Exact | Registers masked | Registers + frame offsets masked |
|---|---|---|---|---|---|
| 0x0041E6D3 Action | ours, session 5 v23 | 35697 v 35651 (frame 0x808) | 81.0% | 87.5% | 96.4% |
| 0x0041E6D3 Action | Codex current_checkpoint_02 | 35702 v 35651 (frame 0x800) | 76.3% | 81.8% | 96.5% |
| 0x00CBFB7D RunCutsceneMacro | ours, row 885 | 74965 v 74778 (frame 0x18E4) | 46.2% | 49.7% | 92.5% |
| 0x00CBFB7D RunCutsceneMacro | Codex current_checkpoint_02 | 74906 v 74778 (frame 0x18DC) | 83.4% | 85.5% | 92.5% |
| 0x00CBFB7D RunCutsceneMacro | Codex alt slide_factor_compound (unreviewed x87) | 74882 v 74778 | 84.1% | 86.3% | 92.5% |

Reading: for RunCutsceneMacro Codex's exact frame (IS_THING_ALIVE body visible so pThing shares its slot; flagWasSet assigned in
both SetFlag arms, removing bool padding) re-aligns every frame offset -- the new best for that row. For Action the two are level once
registers and frames are masked; ours has more exact-matching instructions, Codex's has the exact frame (counted-pointer ctor body
visible, FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR) plus receiver caching after FindChild and the PHOTO_CAPTION named string.
A merge of the two Action sources is the obvious next step. Both rows: entry register roles still differ; no behaviour tests.
Portability: rows/00cbfb7d/hdr/fable_string.h (and other overlay copies of old headers) still spell 16 bare __forceinline --
convert with FABLE_FORCEINLINE before any landing. No __asm / naked / _emit / volatile in either source.
