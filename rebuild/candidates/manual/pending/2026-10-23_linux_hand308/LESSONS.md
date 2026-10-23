# Chunk 308 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Rough-area SIB order: declare `long y = box.Y0; if (y < box.Y1) { long offset = lvl*sizeof(Info); long cellsOffset = lvl*sizeof(CellVec); for (; y < box.Y1; ++y) ... }`, then index through integer sums: Width at `offset + LevelInfo + 4`, the row at `Cells + cellsOffset`. This gives retail's `[hoisted + LevelInfo + 4]` (12 rows closed, 007FCEF0 and others). In a test, `+ 4` must become `+ sizeof(long)` on LP64.
- Type test with the mask copied into a local (`unsigned long mask = T::TypeMask; (flags & mask) == mask`): this keeps `xor dl,dl ; jne` for mask 0 and `cmp dl,imm8` for byte masks (0079BF30, 0079A050).
- An STLport list push_back needs `insert(end(), x)` over an iterator with a user copy ctor, plus malloc and placement new for the node; without the copy ctor VC shrink-wraps `push edi` (0078DB20).
- A same-TU `auto_inline(off)` stand-in for vector `_M_insert_overflow` puts the `__true_type` tag and the cell pointer in retail's argument slots (0074D710).
- CFilter_BindN holds its sub-filters BY REFERENCE (`mov ecx,[filter+4*k]`); a large inline filter, and sometimes the Bind operator, needs `__forceinline` (0085B2A0).
- `name == "" ? CCharString("NULL") : name` with an explicit `CCharString("prefix ") +` gives retail's 0x0099F570 call and the ebx temp-flag bits (0083BF90).
- C3DVector cross product: taking the arguments BY VALUE turns the third Normalise's `fst`-kept r into retail's `fstp`/reload; the operand order inside a commutative fmul is canonicalised (source swaps do nothing) (00837220).
- A virtual call through a member pointer, `T* physics = PhysicsTC; physics->F(...)`, loads ecx early like retail; the direct `PhysicsTC->F` form loads it late (00837220).
- `(GetLengthSquared written as l = Z*Z; l += Y*Y; l += X*X)` reproduces retail's Z,Y,X order; the expression `Z*Z + Y*Y + X*X` is emitted X-first (00837220).

Review at landing:
- These rows needed more than 4 grades: 00837220 (24, parked), 0083c2d0 (9, parked), and 0085B2A0, 009871C0 and 0063F7C0 in the earlier session.
- CThingSearchTools is a local model next to engine/CThingSearchTools.h: the PDB's 16-byte vector at +0x0c does not match retail's 12 bytes. The same goes for CThingSwitch (retail is 0x18 below the PDB, as in the landed 0x0083C230) and the CThing view in 009147e0 (retail is 0x10 below the PDB).
- 009147e0's ScaresBirds filter reads a byte at +0x10 (compared with 13) that is named only by its use (KindByte). It sits at PDB +0x20, the head of UsingComponents, so confirm it before landing.
- The catalog holds one propagated label for 21 different instances (see NOTES Label corrections). The leaf names are the template function names (FindThingsInRoughArea, GetPBestThingInRoughArea, GetPFirstThingInRoughArea). Each was graded with that leaf, and every TU holds exactly one instance.
