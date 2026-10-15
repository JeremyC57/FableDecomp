# Chunk 75 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- CShaderRenderManager single-vector uploads match only through an inline helper `SetVertexShaderConstant(reg, x, y, z, w)`
  (register first, components by value) -- fixed 7 rows. `engine/CVertexShaderConstantLayout.h` matches retail; its
  12-byte ranges are {vptr, Offset +4, Count +8}.
- Model STLport vectors with their REAL structure: `_Vector_base(const allocator&)` with the end-of-storage pointer in an
  `_STLP_alloc_proxy` over an empty allocator, built from an `allocator_type()` temporary. This fixed the "24i lane gap"
  (`lea ecx` / `mov ecx,edi` after the zero stores) in three constructors and the late `mov esi,ecx` in four `vector(n)`
  constructors (mislabelled `New<C3DAnimationSequence>`). The 24i entries in GOTCHAS are therefore mostly MODEL errors.
- F15 family (two argument stores before the vtable store): an inlined `__declspec(novtable)` intermediate base that owns
  the pair fixes the vtable-store placement (00dbf190; with the STLport array the argument load order matched too).
  The 76 F15 rows are worth a new family pass.
- One zero register for an 8-byte fill (`vector(n)` copying `T()`) needs a POD element with no constructor (value-
  initialised as a whole); a constructor zeroing two members gives ecx and edx.
- An inline record constructor written as `memset(this, 0, sizeof *this)` gives `lea ecx,[esi+N]` addressing with stores
  through ecx; member-wise initialisers fold the offset into esi.
- `GetData()` must test unsigned `Size() > 0` (`!= 0` folds into `test edx,0FFFFFFF8h`); "build a default local, copy it
  into ecx's object, destroy the local" = a static `__fastcall` function returning the class by value (`CEntry e; return e;`,
  no NRVO); an out-of-line function with one store after an if/else tail-duplicates the store into both arms, inlined keeps one.

Review at landing:
- The landed twin UpdateAmbient (0x00989760) DIFFERs under plain q.py with its own source; re-grade it with the lander
  (pragma sweep) and, if it really differs, re-author it with the `SetVertexShaderConstant` helper.
- Retry the parked "24i" rows of the other wave-25 batches with the STLport `_Vector_base` / novtable-intermediate-base
  model before treating them as lane gaps (e.g. hand76 004570d3 / 00457da9 / 00458c19 / 004e00bc, hand71 0x00DEF6A0).
- Parked here: 00c40450 / 00c404a0 (`or [ecx+48h],imm` split into load/or/store by the lane), 0046313e (last byte store
  below the pops), 006808c0 (count reused across a join), 00a5c570 (`xor ecx,ecx` placement), 00a533f0 (an unreachable `jb`
  kept by the lane in a 64-bit size guard).
