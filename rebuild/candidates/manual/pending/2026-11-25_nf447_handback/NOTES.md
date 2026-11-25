# NF447 notes (GameCODA render slot-row debt, R-NF445-1)

## Closure (measured 2026-11-25, before any edit)
Retail vftables (slot 0..4):
- 0x012BED7C PanDelay: c6d350 c6cad0 c6cc60 c6d120 c6d150; SSE2 0x012B8A64: c554c0 c6d560 c6d630 c6d120 c6d150; MMX 0x012B8AA0: c55550 c6f300 c6f3e0 c6d120 c6d150
- 0x012BED90 Pan: c6e1d0 c6e690 c6dba0 c6e020 c6e050 (SSE2 8a78 / MMX 8ab4 slot 2 = c6e410 / c6f990)
- 0x012BEDB8 Disable: c6ecb0 c6e690 c6e8f0 c6ec50 c6ec80 (SSE2 8a8c / MMX 8ac8 slot 2 = c6eef0 / c6fe10)
- 0x012BEDCC HrtfMMX: c70a30 c701e0 c70480 c70840 c708a0; 0x012BEDE0 HrtfSSE2: c71400 c70bc0 c70e50 c71210 c71270
Emitters of the ??_7: ed7c 6ca20/6cac0; 8a64/8aa0 55270(+8 funclets)/55070/550d0; ed90 6d9d0/6da30; edb8 6e760/6e780;
edcc 70120/701d0/70a30; ede0 70b00/70bb0/71400. 55070 / 550d0 have no retail caller (out-of-line copies; 55270 inlines them).
0x00C55270 is the table witness of every ctor it calls and of both SIMD ??_7 lines.

Plan (advisor):
- b1 PanDelay unit (21 rows): slots cad0 cc60 d120 d150 d560 d630 f300 f3e0; emitters 6ca20 6cac0; SIMD ctors 55070 550d0; 55270 + 8 funclets.
  NF441 fix 0x00C6CA20 carried. New headers CSalRenderPanDelay.h / CSalRenderPanDelaySSE2.h / CSalRenderPanDelayMMX.h.
- u2 Disable (ec50 ec80 6e760 6e780), u3 Pan (e020 6d9d0 6da30): small units if 55270 needs no respell for them.
- HOLD HrtfMMX / HrtfSSE2 (14 rows: 701e0 70480 70840 708a0 70120 701d0 70a30 / 70bc0 70e50 71210 71270 70b00 70bb0 71400): need 55270 again -> 35-row unit.
- HOLD NF441 fixes 550b0 / 55090 / 55110 / 550f0: each needs its SIMD header, its Process row (6eef0 / 6e410 / 6fe10 / 6f990) and 55270.

## Hand-back state (2026-11-25, end of NF447)
- b1 (batches/b1, 23 rows, one unit 00c55270): the 21-row PanDelay closure + the fold of 0x00C6CEF0 Params3DInit / 0x00C6CF40 UpdateFilters
  (option S slice, owner 0x00C6CEF0) so no row keeps a CVtable_012bed7c view. All gates in batches/b1/README.md; decisions and the per-row
  tie table in batches/b1/NOTES.md. The 21-row state is kept in saved/b1_21rows.
- Held, as post-b1 units in landing order (each re-ports 0x00C55270 + its 8 funclets, so each waits for the previous landing and its HEAD text):
  1. Disable (~18 rows): 0x00C6E760 / 0x00C6E780 (emitters), 0x00C6EC50 / 0x00C6EC80 (slots), 0x00C6EBE0 (rebuild helper), 0x00C6EEF0 /
     0x00C6FE10 (SIMD Process), 0x00C550B0 / 0x00C55110 (SIMD ctors, NF441 fixes) + 0x00C55270 + 8 funclets; headers CSalRenderDisable/SSE2/MMX.
  2. Pan (~18 rows): 0x00C6D9D0 / 0x00C6DA30, 0x00C6E020, 0x00C6DEA0, 0x00C6E410 / 0x00C6F990, 0x00C55090 / 0x00C550F0 (NF441 fixes) + 0x00C55270
     + 8 funclets; headers CSalRenderPan/SSE2/MMX.
  3. Hrtf MMX / SSE2 (~23 rows): 0x00C701E0 0x00C70480 0x00C70840 0x00C708A0 0x00C70120 0x00C701D0 0x00C70A30 / 0x00C70BC0 0x00C70E50 0x00C71210
     0x00C71270 0x00C70B00 0x00C70BB0 0x00C71400 + 0x00C55270 + 8 funclets. HrtfSSE2 is a style name (BEST_GUESS `style`); Hrtf
     ClearFilterHistory 0x00C701E0 is 92 bytes v the SDK's 19 instructions: no size tie, needs another tie or BEST_GUESS.
  4. Slot-0 deleting destructors 0x00C6D350 / 0x00C554C0 / 0x00C55550 and destructors 0x00C554E0 / 0x00C55570: with b1's headers the compiler
     emits ??_G / ??1 of the three classes EQ at exactly these homes (pk_all --addrs), so a small respell batch can follow b1.
  5. Tap mixers 0x00C6CAF0 / 0x00C6CB80 / 0x00C6CC10 (landed MixDelayTapRampUp / RampDown / MixDelayTap): the SDK private statics MixFadeIn /
     MixFadeOut / Mix of CSalRenderPanDelay (Release relocation order in Process matches); port with a size tie in a later batch.
