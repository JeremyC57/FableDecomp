# Batch `2026-10-14_linux_tiny4` — family tiny4 (≤16-byte bodies, `wtiny4`): 83 bodies (61 landed; the second pass after the template fixes is `2026-10-14_linux_tiny4b`)

Fourth generic synthesiser for the ≤16-byte tail (`tools/decomp_pipeline/families/fam.py` template
`wtiny4`, 40 recognisers written from the two-body shapes left after `wtiny3`): getters through one
or two pointers, 16-bit and bitfield getters, out-parameter setters, counters, FPU products with a
member or global scale, small constructors (`CtorInit`, `CtorBaseThenZero`, `CtorVtableValue`),
empty destructors tail-calling the base, `__cdecl` wrappers, `__fastcall` callback invocation, the
`_atexit` registration of a trivially constructed global, and an unoptimised `return this`.

61 bodies over 19 modules landed in the first pass (30 shapes, two bodies each, one `GetDifference`).
The first-pass failures were generator bugs (a repeated `_pad` member name, a constructor label used
as a method name) and three model lessons recorded in `docs/pipeline/GOTCHAS.md`: VC loads the
right-hand FPU operand first, so `scale * target->value` reproduces `fld [target] ; fmul [scale]`;
a one-slot call whose result is returned is cleaned with `add esp, 4` while a dropped result is
cleaned with `pop ecx`; and three bodies that call a sibling with the same arguments without a tail
jump (`add ecx, N ; call ; ret`, `mov ecx, [ecx+N] ; push arg ; call ; ret 4`) do not reproduce --
the DDK 13.10.4035 compiler always turns them into `jmp` (parked in `docs/journal/FINDINGS_LOG.md`).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
