# 0x00B4D370 HANDOFF (AUTH21)
- State: the bounded item (advisor ruling, item 1) is done. No grade was used: no candidate qualified.
- Best record: unchanged. p9 `$S/w22/auth20/00b4d370/best_evidence.cpp` DIFFER(7162v7176) at /d2inlT314 with overlay `auth20/00b4d370/ov/`;
  p5 (`best.cpp`, 7176v7176) is a bracket only.
- Finding (NOTES.md):
  - p5 does not have retail's placement. Its frame is 0x3c0 and its last PostMultiply result sits at 0x8C, the same as p9; retail uses 0x100.
  - The frame and slot difference is length-neutral (disp32 on both sides).
  - p9's 14 B = 4 B of x87 operand picks (persp -2, grid26 -4, grid26 loop pad +2). Those 4 B suppress retail's 10-B loop-alignment pad at
    0x00B4EDC6.
  - The pad cap (between 10 and 14 B) is inferred from this one site.
- Next: the remaining residue is the x87 order sites (C2T1 readout) plus the slot placement. No pre-registered candidate is pending.
