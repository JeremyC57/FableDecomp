# Landing nf406 b2: 0x004D31C0 is CTCVillage::StatGetClassTypeName

0x004D31C0 moves from the free stand-in to `?StatGetClassTypeName@CTCVillage@@SI?AVCCharString@@XZ`. Sources:
- FableWin tc_register.obj;
- the retail literal 0x0123A5CC, which reads "CTCVillage";
- RegisterTCs 0x004EE23F, which already binds this spelling.

The HEAD text wrote the literal as a cast with no relocation. It is now a real string literal.

The made-up `?ctor@CCharString` is replaced by the PDB constructor `??0CCharString@@QAE@PBDJ@Z`. Its table line `?ctor@CCharString@@QAEPAV1@PBDH@Z -> 0099ebf0` is deleted, and a source census found no other user.

Form: the static is defined in the class body, and a scaffolding helper takes its address (the nf86 / nf88 form, ruling nf387).
- An out-of-line definition adds a /FORCE duplicate (0 -> 1), because RegisterTCs emits the same static as a byte-identical pick-any copy.
- Retail expands it inline in RegisterTCs and in 0x0066122E, which supports an inline origin.
- The out-of-line alternate is kept in pending/2026-11-25_nf406_handback.

Results:
- NF406 gates: RELOCATION_MATCH PASS; linked test; pk 0 NEQ; /FORCE 0 -> 0.
- Coordinator: 1/1 landed, 0 LITERAL_DIFF, self_symbols respelt, T3 nf402 0x004D31C0 closed, baselines OK.
