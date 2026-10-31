# retry/sec5 -- parked-row retry sweep, wave 2 section 5 (0x00989230-0x00B16410)

354 rows. 18 matched (src/ + test/, dry vland 18/18 MATCH or RELOCATION_MATCH + PASS, no RELOC_IDENTITY),
50 improved (differ/), 286 held. 39 of the held rows got no graded edit: 10 SSE inline-asm rows (intrinsic-emittable,
not retail_asm), 10 body-less fistp drafts, 11 body-less triage drafts, 8 skeleton drafts. The last 29 were left
because of budget, not because they were judged impossible.

## Per-lever wins (matched rows)
- Implicit destructor inlined into ??_G by also destroying a local object of the class elsewhere in the TU: 3 (00a6bfc0, 00a98760, 00a987b0)
- Heap pop as `if (!entry) { if (grow) { create; reload } else return 0; }` with an early entry local: 3 (00af8680, 00af86c0, 00af8b00)
- Extent fix (oracle_extra + BOUNDS_FIX: int3 after a noreturn call): 2 (00994b10, 0099c7e0)
- Stepwise sums in retail term order / named record reference / repeated cofactor: 2 (00a8a9d0, 00a0dcd0)
- Named local whose temporary lifetime ends at its block/statement: 2 (009ce050, 009c05c0)
- Explicit temporary argument instead of a default argument (homed in the dead arg slot): 1 (009bae70)
- Loop-invariant temp in a guarded scope + do/while with hoisted init: 1 (00b094b0)
- Shared CCountedPointer.h SetStaticCast + const callee: 1 (00ac09a0)
- Union in/out slots for a bit-pattern float round trip: 1 (00a0db60)
- STLport _Vector_base ctor order: 1 (00996610)
- Inline null-safe getter in an inverted if + shared CDefPointer headers: 1 (009ace90)

## Improving levers (count of improved rows)
- landed retail_asm FloatToLong / FloorToLong helpers (must be __forceinline): 9
- in-place __forceinline SetInverse into a named matrix instead of a returned GetInverse temporary: 4 (00a11d10, 00a11f20, 00a128e0, 00a12ab0)
- extent fix (catalog extent swallowed an uncatalogued function or int3 tail): 3 (00a69c20, 009c5890, 0099e690)
- stepwise dot / squared length in retail order: 7
- copies by value / named locals for spilled copies (position C4DVector, pending write, y before Normalise, cross from copies, first point of returns): 6
- guarded do/while down-counter stops VC7.1's 4x unroll (00addbe0); down-counting pixel loops (009f4590)
- removing a dead zero-init (00aa8830); root-chunk pointer local (00a6d0e0); declaration order (00ab7b00, 00989830, 0099e690)
- SetRow helper + separate end store, __forceinline Add/Sort + implicit literal conversion, scale loop over 9 floats,
  class-key fix, in-class SetDirtyDiscError (the landed body): 1 each

## Descriptive names used (not proven, not landed)
- `global::parked_<addr>` oracle_extra names are placeholders; the real names come from the src files.
- 009c3c20 (differ, names only): GetCodeSectionManager / CCodeSectionManager / g_DirtyDiscError respelled to the
  landed CTBaseSingleton<CFileInstaller>::Get, CFileInstaller::DirtyDiscError / ExitThread / CriticalSection.
