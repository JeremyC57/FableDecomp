# NF452 p1 notes (Pan family, stacked on d1)

## Decisions
1. Stacked on d1: 0x00C55270 and its includers carry d1's text as their base (head/payload.json and orig/ hold d1's text for 00c55270,
   00c550b0, 00c55110 and d1's tests for the unit rows; HEAD text for every other row). Gates for those rows compare d1-new (H) with p1-new
   (N); the strict recheck ran on a stacked simulation (HEAD + d1 + p1).
2. engine/CSalRenderPan.h in NF447's form: SDK CodeView names / access / order (CSalRenderPan : public CSalRender; ParamsPan; private
   m_fGainPan), retail offsets and 6-entry bounds (`push 78h`; constructor clears 2 x 6 dwords + 6 gains). FilterPan is the struct of
   engine/CSalRenderDisable.h, shared through FABLE_GAMECODA_FILTERPAN_DEFINED (both SDK objects declare the same 4-byte struct).
   SIMD headers as d1: inline empty ctors that 0x00C55270 calls out of line (retail and the SDK Release object call 0x00C55090 / 0x00C550F0).
3. Fold 0x00C6DE60 Params3DInit (owner) + 0x00C6DEA0 UpdateFilters (thin), as NF447's 0x00C6CEF0 / 0x00C6CF40: one TU reproduces retail's
   inlining (expanded at the first two sites of each branch, called at the rear two) with no helper of our own (HEAD 0x00C6DEA0 carried a
   private inline copy SetGainRamp to get that shape).
4. The SIMD Process rows keep HEAD's kernels and tap mixers; the MMX row's tap mixers move into `namespace Row00c6f990` (as the SSE2 Disable
   sibling's `Row00c6eef0`): with both MMX Process rows on FilterPan, d1's 0x00C6FE10 and this row would define the same three global
   helpers (forcelink DUP+ ?MixGainMMX / ?MixGainRampUpMMX / ?MixGainRampDownMMX before the fix; in HEAD the two rows already clashed on the
   SGainRamp spellings). The namespace changes no graded byte (RELOCATION_MATCH, test, mutant rerun).

## Per-row ties (evidence/tie_table.tsv; SDK = GameCODA 2.10 CODAApi.lib Release, relocation lines excluded)
| row | name | retail insns / ret | SDK insns / ret |
|---|---|---|---|
| 00c6d9d0 | ??0CSalRenderPan | 36 | 30: the identical shape at 4 entries (6 extra stores = 2 entries x 3 arrays); SDK SIMD COMDATs call it = retail 55090 / 550f0 call 6d9d0 |
| 00c6da30 | ~CSalRenderPan | 2 | 2 |
| 00c6de60 | Params3DInit | 17 / 8 | 17 / 8 |
| 00c6dea0 | UpdateFilters | 111 / 4 | 111 / 4 |
| 00c6e020 | SetGain | 12 / 0Ch | 12 / 0Ch |
| 00c6e410 / 00c6f990 | Pan SSE2 / MMX Process | 198 / 8 | 183 / 8; same Enter / fade x2(+2) / swap / fade x2(+2) / Mix x2(+2) / Leave structure |
| 00c55090 / 00c550f0 | ??0CSalRenderPanSSE2 / MMX | 18 B | SDK COMDATs of SalSw3DVoice.obj, called by CreateRenderer in the same order |
Also tied, not rows: SetPosition 0x00C6E050 119 / 1Ch = SDK 119 / 1Ch (slot 4); MixFadeIn / MixFadeOut 0x00C6DA40 / 0x00C6DAD0 53 / 54 = SDK
(NAME_FIXES.tsv); retail SalRenderPan.obj function order = SDK Release object order.

## Follow-ups
- Slot-0 stand-ins 0x00C6E1D0 (??_GCSalRenderPan), 0x00C554F0 / 0x00C55510 (PanSSE2 ??_G / ??1), 0x00C55580 / 0x00C555A0 (PanMMX): EQ at
  those homes from the p1 objects (pk_all --addrs).
- 0x00C6E050 SetPosition and 0x00C6DBA0 Process have no rows (vt432 unknown, forcelink UND ?SetPosition / ?Process@CSalRenderPan).
