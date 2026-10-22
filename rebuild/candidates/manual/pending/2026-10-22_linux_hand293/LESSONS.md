# Chunk 293 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## x87 operand order and inlining
- **x87 commutative operand order is canonical in VC7.1.** For `a*b` with both operands in memory, and for the order of terms in a sum, our VC7.1 gives byte-identical output whatever the source order. It is also unchanged by a union/array matrix representation, `/G6` and `/Op`.
  - Retail's inline CMatrix3x4 inverse (SetInverse / SetInverseNoTranslate) loads the other operand first in every cofactor product.
  - This parked 00aede60, 00aee240, 00aee560 and 00aeeba0 at same or near-same size. 00b08250 and 00b08530 show it too.
  - Treat it as a lane gap until someone finds the expression shape retail used.
- **Small matrix/vector helpers need `__forceinline`.** Without it, VC7.1 leaves `Set(CRightHandedSet, pos)`, `SetInverseNoTranslate` and the interpolators out of line (157 bytes vs 796).
- **A 2D vector copied by x87 needs a user copy ctor.** A C2DVector/C3DVector copied through the FPU (`fld; fld; fxch; fstp; fstp`) needs `C2DVector(const C2DVector&)`. The implicit copy goes through int registers and adds a frame temporary (0x84 vs 0x80 frame).
- **Accessor calls decode the flag word differently from direct bitfield tests.**
  - `IsSleeping()` / `IsEmitter2D()` accessors returning bool give `xor eax,eax; mov ax,[+40h]; mov ecx,eax; shr ecx,0Ah; test cl,1`.
  - Testing the bitfield directly gives `test ah,4`.
  - Pick the form by the prologue.
- **Early return vs shared return.**
  - `if (!Initialised) { ... } return Initialised;` gives retail's shared `mov al,[esi+41h]; and al,1` return.
  - `if (Initialised) return Initialised;` builds a temp.
- **A constant VC can see gets folded.**
  - `fld [eps]; fchs` means the epsilon is an `extern const float`; a visible `static const` folds -eps into a new constant.
  - Similarly, `CPlane(normal, normal * v0)` (D computed by the caller) gives retail's delayed plane stores; computing D in the ctor's init list does not.

## Spotting inline asm
- **Inline-asm signature (lane gap).** Treat a row as inline asm when you see any of these:
  - C locals or members stored to frame slots and immediately reloaded into registers right before SSE code;
  - `push esi / pop esi` inside straight-line code;
  - 16-byte `and esp` frames with an ebx frame pointer;
  - `fistp` floors with the 0.5/1.0 constants stored to memory first.
  - Seen in 00ae29d0, 00ae1320, 00adf960 and 00f361d0. Write the behavioural C++, grade once, park.

## Loops, stand-ins and temporaries
- **Loop helpers.** Loops inside `__forceinline` member helpers lose the loop-head alignment padding (`lea esp,[esp]`), matching retail 00aed2f0.
- **Same-TU stand-ins can over-inform register allocation.** A same-TU `__declspec(noinline)` callee stand-in gives VC7.1 both memory-effect and register-clobber knowledge. Retail sometimes had only the first (00aed2f0 keeps the flag in bl, ours in dl).
- **Empty filter structs need a user ctor.** `CThingFilter_All` needs `CThingFilter_All() {}`, or VC emits a zeroing byte store. Passing a temporary rather than a named filter matches retail's frame.

## Carried from the earlier half of the chunk
- The goto trick for an inline vector free next to an out-of-line hero dtor.
- `cmp al,bl` comes from an if/return accessor.
- forceinline `operator==` with the strcmp intrinsic.
- Per-coordinate GetPosition() temporaries.
- forceinline ReadUByte/ReadULong with a 64-bit range check.
- A chained `a = b = c = 0` stores in reverse order; separate statements keep source order.
- Bitfield group order.
- STLport `_STLP_alloc_proxy`.
- Inline ctor parameter order.
- By-value accessor temporaries.
- `__forceinline` Clear.
- Macro-style min recomputation.
- In-place list node construction.

Review at landing:
- [ ] Decide whether the CMatrix3x4 inverse operand-order gap gets its own GOTCHAS line plus a ROADMAP item; it blocks 4 emitter rows here.
- [ ] Re-label the manifest entries listed under "Label corrections" in NOTES.md (0xAF0880, 0xA6CBB0, 0xA14480, 0xAEDCA0, 0xAE04E0, 0xAE2510 are new this chunk).
- [ ] 00459e46 lands as CPlayerGuiDef::~CPlayerGuiDef, not under CPatchTesselationEdgeStrip.
- [ ] The four inline-asm rows (00ae29d0, 00ae1320, 00adf960, 00f361d0) are lane gaps, not authoring misses.
