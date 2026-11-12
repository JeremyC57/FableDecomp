# Chunk 3022 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A9EB90 / 0x00A9F540 / 0x00AA0090) To settle the text of a shared inline helper, compile each candidate OUT OF LINE against
  retail's uncalled copy before fitting the rows. BlendWith matched 0x00AA3C50 only WITHOUT chunk 2044's named `from` reference
  (FableWin 48i), and with that text all three rows matched. FableWin size picked the wrong one of two size-close texts here.
- When a shared helper text breaks one row's record-keyed order (x87 dot order), look in that row for natural names first.
  `C3DKeyframe& keyframe = keyframes[bone];` in each bone loop fixed 0x00A9F540 and widened its window from 2..3 to 1..8 dead floats.
  Dead `long x = 0;` rulers did not move this linearizer order; dead floats (`float dd = (float)n * k;`) did.
- An in-place `q.SetSlerp(q, other, t)` with a direct-store SetSlerp loses retail's frame copy. Slerping into a named temporary and
  assigning it back (`CQuaternion slerp; slerp.SetSlerp(rotation, to, t); rotation = slerp;`) reproduces it (0x00A9F009..0x00A9F05D).
- Unused FABLE_FORCEINLINE helper bodies (and the opt-ins that declare them) are byte-neutral in a row TU. A full common helper
  block can sit in every row of a class without moving any order.
- md.py (relocation-masked diff) masked `[eax*4 + table]` wrongly when the addend equals the scale. Mask the displacement, not every
  matching number (bin/md.py here is fixed).

Review at landing:
- The three rows carry one identical helper block. If a shared header is made for GFFastArcCos / SetSlerp / CalculateSlerpWeights /
  SetSlerpFromIdentity / BlendWith, use this block. The landed out-of-line copies (0x00A22530, 0x00A22590, 0x00AA3390, 0x00AA3560,
  0x00AA3C50, 0x00B05620) all grade RELOCATION_MATCH with it (probe/users/).
- Chunk 2044's README describes BlendWith as "a named reference + SetSlerp". The final text has no reference.
