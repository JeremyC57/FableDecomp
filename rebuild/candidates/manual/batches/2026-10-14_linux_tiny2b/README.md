# Batch `2026-10-14_linux_tiny2b` — family tiny2 (≤16-byte bodies, `wtiny2`, second pass): 12 bodies (12 landed)

The twelve bodies that differed in `2026-10-14_linux_tiny2`, regenerated after three model fixes in
the `wtiny2` template (see that batch's README): `IsSet` byte predicates returning `unsigned char`
(`CASuspendableProcess::CheckTermination` and three siblings), one-bit `unsigned char` bitfield
getters (`CTCPhysicsStandard::CanCollideWithLandscape`, the `vfunc_23/51/59` flag getters) and the
`__stdcall` delete-expression wrappers (`vfunc_2`, `FreeBlock_*`; `operator delete` @ 0x00BFEA14 is
the FSE label `Game_free`).

All graded `MATCH` / `RELOCATION_MATCH` + `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
