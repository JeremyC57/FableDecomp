# Chunk 320 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Entity-script text lines: put each line in its own inner block and leave it with goto exits; a cycling line chosen by a block-scope prefix ternary string matches (0x00EB23E0, 0x00EB10C0, the landed twin 0x00EB15C0).
- A by-value matrix read (`CMatrix3x4 m; m.X.Y = ReadFloat(); ... return m;`) gives retail's twelve inline reads into a local followed by the word copy-out (0x00995340).
- CCountedPointer `operator=` must read `other.Info` and then `other.Data` into locals before the self-compare. Keep it plain `inline`: VC7.1's inline budget then inlines it at the early sites and calls it out of line (0x754F10) later, as retail does. `__forceinline` inlines every site (0x00AF8170, 0x008621A0).
- A function-scope local declared in a dead argument's slot keeps that slot reserved and moves later temporaries (0x00B22670).
- A pointer-walking loop (`T* p = start; do { ... } while (++i, ++p, i < count)`) gives retail's ebx walk.
- `!p ? "" : p->Data` gives retail's `jne` layout for an inline c_str; `!x ? 0.0f : f()` does the same for floats.
- The implicit dtor of a scripted-thing handle (no user dtor, so no vptr store) expands at most once. A `while (!IsActiveThreadTerminating())` loop duplicates the exit, and the single expansion lands on the loop-entry copy. To get retail's one inlined exit at the bottom, write `for (;;) { if (IsActiveThreadTerminating()) return; ...; Game->NewScriptFrame(); continue; }` with the other returns under `inline_depth(0)`. A user `__forceinline` dtor adds the class's own vptr store, which retail lacks (0x00F00C90).
- A movie or handle object declared inside an `inline_depth(0)` region gets an out-of-line ctor. Keep declarations outside the pragma regions (0x00F00C90).
- The engine's fast Normalise has a guarded variant: `if (X*X + Y*Y + Z*Z > eps*eps) { float l = X*X; l += Y*Y; l += Z*Z; ... }`. The X-first accumulation reproduces retail's recomputed sum after the compare (0x00ACD0E0). The unguarded GetRHSet model (Z-first accumulation) matches FindTextureTangent (0x00AC7300).
- `fabs(float)` must be the `<math.h>` C++ overload, `(float)fabs((double)x)`, to get `fabs ; fcomp dword [eps]`. A bare double fabs loads eps and uses `fcompp` (0x00A18FC0).
- An early return written `if (a + eps >= b) return zero;` gives retail's `test ah,1 ; jne body` with the zero return falling through. `if (!(a + eps < b))` gives `test ah,5 ; jnp` (0x00AC7300).
- Two identical `return ARRIVED` blocks, one per condition, tail-merge into retail's layout: the first test falls into the block and the second jumps back to it. `||` and a goto label lay the block out at the end (0x00A18FC0).
- Zeroing a `C3DVector&` out-parameter field by field (`move.X = 0.0f; ...`) stores the zero register. Assigning `C3DVector(0,0,0)` goes through a temporary (0x00A18FC0).
- An inline `CrossProduct(a, b)` evaluates b first (right to left): retail's (p1-p0)-first load order is `CrossProduct(p2 - p0, p1 - p0)` (0x00AC7300, 0x00ACD0E0).
- Bare dword `fistp` rounding with no `__ftol2` (six rows: 0x00A781A0, 0x00B16410, 0x00B103F0, 0x00B00DB0, 0x00B012E0, 0x00AFB7C0) is the inline-asm helper. These are asm_bake family: park them on sight.

Review at landing:
- Rows that needed more than 4 grades: 0x00F00C90 (18), 0x00ACD0E0 (10), 0x00AC7300 (8), 0x006C2170 (8), 0x00B22670 (8), 0x00B1A870 (7), 0x008621A0 (6), 0x00A18FC0 (5). Two matched rows also took 6 grades each: 0x00AF8170 and 0x00EB4F80.
- 15 rows are triage-only stubs (decoded, no source drafted): 0x008DD0F0, 0x00AB4BB0, 0x009BD460, 0x00A7A8D0, 0x00A79B30, 0x00A1A260, 0x00A1B5A0, 0x00A1D6E0, 0x00A475C0, 0x00700AE0, 0x00701010, 0x00ACE600, 0x00ACC490, 0x00D9C680, 0x00B079B0. They are large x87, aligned-frame or many-callee bodies.
- Local base models used next to existing engine headers:
  - CModeStandBase: a local counted-pointer model, because the engine header flattens counted pointers.
  - CNavigatorFlyer: the header only names DestinationPos.
  - COptimisedPrimitive: retail 12-byte vectors.
- 0x00A14FD0 is landed as `IsInSameMap(const C3DVector&, const C3DVector&)`, but its callers pass 2D temporaries. The flyer drafts declare `(const C2DVector&, const C2DVector&)`. Check the parameter type when either side lands.
- 0x00ACA380 carries a low-trust forge autoname (`QuadricOptimiser_DetectFoldOver_BuildTree`). The DetectFoldOver draft names it `CFaceSet::MergedWith(const CFaceSet&) const`.
