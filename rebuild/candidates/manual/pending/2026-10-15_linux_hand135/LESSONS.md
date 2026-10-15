# Lessons from hand chunk 135 (candidates for GOTCHAS)

- A deleting destructor that passes `xor edx,edx` to a `__fastcall` class operator delete: the class declares the
  two-argument `(void*, long)` forms (placement-matching its `operator new(size_t, long)`) plus INLINE usual forms
  `operator delete(void* p) { operator delete(p, 0); }` / `delete[]` likewise. A usual `(void*, size_t)` form passes the
  element size (`mov edx,48h`); a lone `(void*, long)` form is placement-only, so VC falls back to the global delete.
- Stores BEFORE a derived vtable store in an inlined constructor (members zeroed, an argument stored) are the init list
  of a `__declspec(novtable)` intermediate base; written as the derived class's own init list or body they land AFTER the
  vtable store (00be1ba0, 00b9e8c0).
- VC7.1 keeps a primitive constructor of ~20+ stores out of line even when it is defined in the class: `new T` factories
  whose retail body has the constructor inlined need `__forceinline` on every constructor in the chain (00be1a00,
  00b9e8c0); the same for `??_G` (forceinline the virtual destructor and the counted-pointer destructor, 00bf5d30).
- A 4-byte colour copied with all four byte loads before any store (one byte parked in a dead argument slot) is
  `CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }` with `Set(r, g, b, a)` storing R, G, B, A (arguments
  evaluated right to left = loads A, B, G, R). In a `/Os` TU both need `__forceinline`.
- A C3DVector member copied straight into `[eax+N]` through edx/ecx = a memberwise user copy constructor; the implicit POD
  copy forms `lea edx,[eax+N]` and borrows esi (00b44e30, 004c3090). A C2DVector stored X by `mov` and Y by `fld/fstp`
  is a `C2DVector(float x, float y)` constructor fed from the source's members (00543910).
- An identity matrix whose nine zero stores run DESCENDING and precede the three 1.0f diagonal stores is a fill written
  from the top down then the diagonal (m[8], m[4], m[0]); an ascending fill or a 12-float constructor reorders it.
- A 4-float vector set whose w is computed before x (parked on the x87 stack) is an inline `Set(x, y, z, w)` call; a
  `C4DVector(x, y, z, w)` temporary assigned whole goes through a 16-byte frame copy and VC CSEs the divisor.
- A counted member released with `dec [ecx+N] ; jne ; mov eax,[ecx] ; push 1 ; call [eax]` and no second null test is
  `T* p = m_p; if (p) { if (--p->m_count == 0) delete p; }` (24f's local-copy rule; `delete` on the known-non-null local
  drops its own test).
- Byte-identical bodies in the chunk (00bb1900 / 00bc7b20) are separate rows: author both with the same source.
- Check the vtable store, the callers and the PDB size before trusting a propagated label: six of 40 rows here were other
  classes (NShape::CShapeManager, CConsole, CEngineMeshEffectOutlineSmooth, CKeyCamera, a libpk queue record,
  CTCHeroExperience). A caller scan (E8/E9 rel32 to the address) plus the overflow string of the caller named the libpk one.
- Lane gaps confirmed again: a value temporary sharing the dead argument slot with an empty tag (00b59c70, 00bb1640), an
  empty tag over a LIVE parameter slot (00b3a110), shrink-wrapped callee-saved pushes (00b41db0), the RTM dead `lea` of a
  vector slot (00bc5a20).
- Grading tip: diff the instruction text of the two listings (`diff`), not only a context-free unified diff -- a moved
  vtable store looks like a substitution otherwise (00bb63a0 graded DIFFER with only "relocation" lines visible).
