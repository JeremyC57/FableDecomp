# Chunk 3018 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x007D6870) Frame words missing because string temporaries did not conflict with later floats: retail's movement block sat INSIDE the
  switch (the input case falls through into `case A: case B:` holding the movement code). A free frame-packer trace (pk.sh) of the
  candidate reproduced retail's exact slot list before grading.
- (0x007D6870) Which of two inline def-release copies gets eax v edx: `if (aligned) break; if (dot > 0.0f) return; ...` inside the case
  (the break-path shares the final release) -- an if/else with an empty arm gave the swapped colouring.
- (0x007D6870) fwsize flagged CRightHandedSet::GetRight RIGHT_COMPONENTS (43i) v FableWin 15i: the default `Forward ^ Up` is the text.
- (0x007D6870) A 3-term x87 dot whose order needs ~+55 c2 record ids is not moved by any small spelling (named / reference / ordered
  vectors, helper named-result texts): measure with the linearizer key hook (tools/c2h/key.py + sums.py, from agent3010) and the
  dead-long ruler before spending grades; explicit CCharString temporaries add ~8 ids each, dead longs ~2.3.

- (0x007D6870, session 2) An "amount of IL" x87 term-order residue (c2 linearizer record-id hash) closed with a step-by-step spelling:
  named intermediate values for each step (dots, movement products, pointer offset, card fields, alignment inputs). Measure with a
  dead-local ruler first; dead locals count the same anywhere in the function and dead references count too, but a USED forwarded name
  is worth only part of a unit, so expect to need 2-3x as many natural names as the ruler says. Several record-keyed sites (a cross
  product's load order, a loop's square order, a dot's term order) must land in the window together: classify every graded candidate
  by which of those sites still differ (gq.sh) and add / drop single natural names to move between them.
- (0x007D6870) FableWin debug sizes for C3DVector operator- / operator^ / Dot (+6 / +6 / +2 instructions) are VC10 return artefacts,
  not text differences; a named-result operator- changes VC7.1 code (extra copy in a switch-arm assignment).

Review at landing:
- 0x007D6870 RELOCATION_MATCH (src/ + test/, PASS g++ + VC7.1, vland PASS). The named intermediate values are FITTED (amount of IL),
  and the match sits in a one-unit window: re-grade after ANY change to C3DVector / CRightHandedSet / C2DVector / CDefPointer bodies.
