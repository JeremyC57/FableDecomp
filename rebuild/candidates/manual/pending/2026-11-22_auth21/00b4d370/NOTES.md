# 0x00B4D370 CEngineShadowRenderer::UpdateShadowSpace: AUTH21 bounded item (advisor ruling 2026-11-22, item 1)
Inputs (read only): AUTH20 `$S/w22/auth20/00b4d370/x/ctl_p5.log` (p5 = best.cpp, DIFFER(7176v7176)) and `x/p9.log` (p9 = best_evidence.cpp,
DIFFER(7162v7176)), both graded by AUTH20 at `/d2inlT314` with overlay `ov/`. No new grade was needed (no candidate qualified). Tool: `blkdiff.py`
(aligns two listings by instruction text, prints per-hunk byte deltas).

## Question: where does p5 put the last PostMultiply result, and what is its frame?
- p5 frame: `sub esp, 0x3c0` (retail 0x3d0), the same as p9.
- p5's last PostMultiply (the second tail loop): output `lea edx, [esp+0x90]` at 0x00B4EE98, copy-out `lea esi, [esp+0x8c]` at 0x00B4EF4C,
  so the result is homed at 0x8C..0xCB. p9 does the same (0x00B4EE8A / 0x00B4EF3E). Retail: `lea edx, [esp+0x104]` / `lea esi, [esp+0x100]`
  (homed at 0x100..0x13F over local_world_to_base_shadow_space).
- **p5 does NOT have retail's placement.** By the ruling, there is no p5-v-p9 candidate at that site, so nothing was pre-registered or graded.
  The placement stays open.

## How p5 reaches 7176 (and what p9's 14-B gap really is)
- The frame/slot difference is length-neutral. Every displacement it moves is disp32 in both builds (`lea ecx,[esp+0x150]` v `0x160`,
  `[esp+0x8c]` v `[esp+0x100]`, ...). p5 has the wrong frame and is still exactly 7176. So the frame is NOT the 14-B gap.
- Per-region byte deltas against retail (blkdiff, built minus retail):

  | region | p5 | p9 |
  |---|---|---|
  | head .. 0x00B4D900 (setinv) | 0 | 0 |
  | .. 0x00B4E900 (persp/else/diag) | -2 | -2 |
  | grid26 .. 0x00B4EC00 | +2 | -2 |
  | tail | 0 | -10 |
  | total | 0 | -14 |

- The -2 shared by p5 and p9 (before 0x00B4E5C0) is the persp PostMultiply(perspective_transform) column-association site, 0x00B4E503..0x00B4E54F.
  Retail keeps a shared partial sum on the x87 stack (`fld st(1); fadd st(1)` at 0x00B4E509, then pops it with one more `fstp st(0)` at
  0x00B4E54F). Ours adds per column (`fadd st(3)` x3) and pops once. That is an x87 operand-order/CSE site, not a displacement-size change.
  Every frame-moved displacement checked in this function is disp32 on both sides.
- grid26 loop head (retail 0x00B4E97E). Retail pads 2 B (`mov edi,edi`); both builds arrive 2 B early and pad 4 (`lea esp,[esp]`). All three
  land at 0x00B4E980.
- Tail, first PostMultiply loop head (retail 0x00B4EDC6: `jmp; lea esp,[esp]; nop`, 10 B of pad to 0x00B4EDD0).
  - p5 arrives on time and emits the same 10-B pad.
  - p9 arrives 4 B early (0x00B4EDC2), because its grid26 x87 operand-order picks are 4 B shorter (p5 nets 0 there). Its pad would be 14 B, and VC7.1 emits
    none. That is -10. (Inferred from this one site: VC7.1 caps the alignment pad somewhere between 10 and 14 B. Not measured separately.)
- So p9's 14 B = 4 B net (persp -2 and grid26 order sites -4, x87; offset by the grid26 loop pad +2) + a 10-B loop-alignment pad that those 4 B suppress. p5's equal length is
  the same pad, reached because its own (different) order picks net to 0 by that point. Neither is a frame effect.
- Consequence for the record (unchanged): p9 stays the row's DIFFER(7162v7176), and p5 is a bracket. The remaining residue is (i) the x87
  operand-order sites (C2T1's comparator task, not touched here) and (ii) the slot placement of the last PostMultiply result. Slot placement
  is still open, but it is length-neutral. Inferred, not measured: closing the order sites alone should restore the tail pad and the length. Placement would then remain
  as pure displacement/frame-immediate residue.
