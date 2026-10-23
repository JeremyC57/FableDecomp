# Chunk 297 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Per-column entry reference** (0xF3A2E0 Compress): index `entries[i]` at each use and VC re-reads the CArray begin after every `unsigned char` store to dest (char aliasing). Retail's shape is `const CScriptEntry& entry = entries[i];` bound once per column. Read values as `source + j * stride`; a pointer local initialised before the loop is loaded ahead of the count test.
- **Strided writes** (0xF39ED0 Decompress): write `*(T*)(column + j * stride)`. A named out pointer moves the bit accumulator into ebx and spills the flags byte. VC hoists the per-value flags-bit tests into byte locals on its own. The `__int64` range check of blockSize*8 returns without closing the timer. The raw path is a plain memcpy.
- **Wide-string temporaries in ctors** (0x9D9A30 CRenderManager2D ctor):
  - `CWideString platform(L"pc\\")` is a named local inside the if-block (a temporary is built after the bool arg push).
  - A named `CCharString bankName` lands in the dead argument slot.
  - The counted pointer's inline `operator->` is used for the resource calls.
  - An implicit literal → `const CCharString&` is used; an explicit `CCharString("...")` moves the Data load from edi to ecx.
  - VC7.1 has no native wchar_t: declare `const unsigned short*` and pass `L"..."` (C2238 otherwise).
- **Vertex store schedule** (0x9DDA50 Draw2DBoxColours): write U, V, Diffuse, Z, RHW of every vertex first and the X/Y of all four last. That source order reproduces retail's interleaved store schedule and the RHW word copied into three registers. Draw2DTexture 0x9DC870 wants per vertex Diffuse, X, Y first, then U/V/Z/RHW after the offset.
- **x87 dot-product operand order is canonical in VC7.1** (0x853D30): swapping `this`/arg or reversing the formula does not change which operand is `fld` vs `fmul`. Do not spend grades on it.
- **Frame-slot assignment of named float locals ignores declaration order** (0x9DDC60): the top/right slot swap survived every reorder.
- **Hand-rounded `fistp` table slerp** (0xAA3C50 CRightHandedSet::Interpolate): a dword `fistp` after a float store/reload is a lane gap, the same family as chunks 145/150/236. Park it at once.
- **CTimer through a `const long&` accessor** (0xF1B220, advisor): `GetTime()` reads the handle through a reference temp. That keeps a conditional-temp flags word in its frame home instead of ebx (921v907 → match).
- **Out-of-line operator= with the rest inlined** (0xDEF760): make `CScriptThing::operator=` and `CCountedPointer::operator=` `__forceinline`. For the one out-of-line call site, bind the element to a reference and assign under `#pragma inline_depth(0)`.
- **Same-TU read-only `_M_insert_overflow` stand-in** (0x9DB810, 0x9DBAE0, 0x9DBD10, 0xCB8440): with only a declaration, the vertex local escapes and later RHW stores schedule three slots late. A same-TU body that only reads its arguments fixes it.
- **Chained assignment** `a = b = f()` stores b first; UVs copied from a const-reference-bound temporary give `lea esi` (0x9DD210).
- **GFAXToVX/GFAYToVY are `__fastcall`** with the float on the stack (0x9DCA80, 0x9DD210).
- **`while (!(!x))`** reproduces retail's `test al,al ; setne` on bool-returning virtuals (0xEF2BA0). `!(a > b)` on floats gives `test ah,41h`.
- **Identical arms are cross-jumped** (0xDE21D0, 0xDE3830, 0xEC1EF0): write both arms in full and VC7.1 merges them. Do not factor them by hand.
- **State-map keys as fresh `const unsigned long&` temporaries** (0x555440): one frame word for the first eight. A key used five times is hoisted into ebp.
- **A block-scoped find iterator** (0x881A40) fixes the slot layout even where regalloc still differs.

Review at landing:
- Same-TU read-only `__declspec(noinline)` `_M_insert_overflow` stand-in bodies in 009db810, 009dbae0, 009dbd10 and 00cb8440 (each emits an extra TU-local function beside the graded one): confirm the landing lane accepts a TU-local definition of an STLport member.
- Local base/struct models where an engine header exists (CRenderManager2D quick-draw records, CScriptEntry, CStackEntry): retype with `retype_landed.py` if the strict header check flags them.
- 00de21d0 and 00dedf30 land under corrected class names (CRandomAttacker, CSP_DeskGuard). The catalog module differs from the src line 1.
- Rows that took more than 4 grades before parking: 00f3a890, 00853d30, 009ddc60, 00555a90, 00a87700. They are good candidates for the Opus near-miss sweep.
- 00aa3c50 is a lane gap: a baked-oracle candidate, not a reconstruction.
