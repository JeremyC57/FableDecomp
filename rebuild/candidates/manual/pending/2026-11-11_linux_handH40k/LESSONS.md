# Chunk H40k lessons (GOTCHAS-ready one-liners)

- 0x00D13580: a movie dtor that retail expands while ours calls it at s74 against T74 (tools/inl.sh) is a one-point
  inline-budget residue: a per-row `// Flags: /d2inlT75` expands it (0x00D13D94) and moves nothing else (119 -> 106 lines).
- 0x00D13580: a call left at an implicit dtor site also lowers that object's frame-packer weight (expanded dtor = 3 IL
  references, call = 1); a movie that retail expands but we call loses its shared slot to a heavier STB (pk2.sh: movie 6 v STB 7).
- 0x00DB97A0: retail keeps T-return pads apart when the block laid before the exit is NOT one of them (there a bare
  `call IsActiveThreadTerminating` from a folded post-loop `if (T) return;`, 0x00DBB2E1); if the last pad lies before the exit,
  c2 cross-jumps every other pad into its tail. Read the block before the exit label before blaming the pads.
- 0x00D13580: rewriting `while (!c) { NSF; if (T) return; }` as `for (;;) { if (c) break; ... }` leaves c2's block
  frequencies (inline trace f values) and bytes unchanged; and `while (A && B)` vs `for(;;){if(!A||!B)break;...}` too.
- /d2inlT73 / T75 sweeps are byte-neutral on 9 of the 10 long NScript rows tried: check tools/inl.sh for a site within a
  point or two of T before spending grades on threshold flags.
