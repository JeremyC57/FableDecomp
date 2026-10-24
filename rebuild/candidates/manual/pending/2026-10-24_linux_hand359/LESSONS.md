# Chunk 359 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Labels
- Labels in STLport-era code mislead often:
  - A `_Move_backward` label (Dinkumware names from Ego_r.pdb) on a PC STLport row is one of three functions:
    `__copy_backward` (0x009B7870), `__insertion_sort` (0x0077ECB0) or `__destroy_aux` (0x00519EF0).
  - A `_Pop_heap_0` label can be `std::swap` (0x00A331B0, 0x00A70490).
  - Identify the function by its shape: `ret` size, stride magic, and which callee is out of line.
  - Take the element type from the TU's `_Adjust_heap` / `_Push_heap` neighbours, not from the row label.

## Layout
- PC STLport `std::list` is ONE header-node pointer (4 bytes). Ego_r.pdb's Dinkumware list is 12 bytes, so
  `_Pair_base<unsigned __int64, list<C2DLineF>>` is 24 bytes in the PDB but 16 bytes in retail (list at +8, +0xC
  padding) (0x00519EF0).

## Inlined bodies
- STLport list destructor inlined (0x00519EF0):
  - `clear()` frees each node unconditionally (`tmp = cur; cur = cur->_M_next; free(tmp)`) and re-reads the header
    after every call.
  - It then relinks `next`/`prev` to the header, re-reading the header for each store.
  - Last comes `if (node) free(node)`.
- `__copy_backward` stores its trip count in the dead `result` argument slot (`mov [esp+14h],eax` /
  `dec dword ptr [esp+14h]`). This is the plain STLport loop
  `for (n = last - first; n > 0; --n) *--result = *--last; return result;`; no pragma is involved (0x009B7870, 0x006FB830).

## Source form
- In a self-assignment guard, the operand order follows the source. `cmp edi(other),ebx(this)` is
  `if (&other != this)`; writing `if (this != &other)` flips the cmp operands (0x006FB830).
- The byte layout of a by-value `T(*(last - 1))` temporary in an inlined `__pop_heap` follows the record's
  copy constructor:
  - A float member that travels through `fld` / `fstp` (held on the x87 stack until the argument copy) means a memberwise
    user copy constructor on the float-holding record. An implicit copy constructor moves it as dwords (0x0076A590).
  - A `{ *this = other; }` copy constructor gives the frame slot plus a register copy (0x004C3BF0 family).
  - A memberwise copy constructor scalarises the temporary and drops the frame.
- `fxch st(1) ; fxch st(1)` immediately before `fcompp` means each `fabs` result was rounded into a float local:
  `float d = (float)fabs(R - x);` then `d_a < d_b`. Comparing the double `fabs` values directly drops the pair
  (0x0087DCA0).

## Forcing inlining
- If your build shows `call __pop_heap` where retail inlines it, write `__forceinline` (0x0076A590, 0x00471576; 0x00471150
  was written with it from the start). This applies even outside /Os TUs. Other TUs inline it with plain `inline` (0x009B6D80, 0x004D0D60).
- A /Os TU that still has a frame pointer (`push ebp ; mov ebp,esp ; push ecx ... leave`, `push 0Ch ; pop ecx`, `idiv`)
  needs `#pragma optimize("y", off)` plus `#pragma optimize("s", on)` (0x00471150).

## Polymorphic members
- A polymorphic pair member (CParticleUpdateParamDesc : CBaseClass, with an out-of-line virtual `~CBaseClass`) in
  `__pop_heap_aux` does four things:
  - It forces the temporary into a frame local.
  - Implicit assignment skips the vptr.
  - Both the temporary and the argument copy store the derived vtable.
  - The implicit derived destructor reduces to a direct `call ~CBaseClass` with no vtable re-store (0x0076AC70).

## Review at landing
- **0x0087DCA0, a retail bug that the source reproduces.** CTurningAnimCompare queries the LEFT operand twice
  (`mov ecx,ebp` for both `GetRotationInAnimation` calls). The comparison is always false and the turning-anim heap never
  sifts. The bytes are right, but confirm against the pop/make-heap siblings of 0x0087DEE3 before calling it a game bug in
  docs.
- **0x0076A590 (parked): best-evidence record.** CCacheGroup {StartDistance, EndDistance, PrimitiveTypeMask} is the only
  Ego_r.pdb 12-byte {float, float, non-pointer} record. The real element may differ, and the open frame/spill diff may
  depend on it.
- **0x004C3BF0 / 0x004C3C60 / 0x00ADB4D0 (parked) are byte-identical apart from their callee.** All three diverge on the
  same `push ebp` slot. Try one shared fix, or record them together as a lane gap.
- **Wrong `_Adjust_heap` labels.**
  - 0x004C2C70 is labelled with `CKeyPairCompareLess<ulong,CTextureMorph>` but tests `byte [+8] & 1`.
  - 0x00ADB000 is labelled `pair<EExpressionType,CEntry>` but compares the signed word at +4.
  - The element models for those rows were built from the bytes.
- **Local models next to existing headers.** None of these classes has a `rebuild/include/engine/<Class>.h` (checked:
  CQuestInfoElementTimer, CQ_OpeningGraveyardSecretPassageScript, CScriptBase, C3DMeshVolume, CDefClassInfo,
  CSmallVector, CAnimationEntry, CParticleUpdateParamDesc, CTurningAnimCompare). CCharString / C3DVector / CCountedPointer
  are local minimal models, as in the landed STL rows.
- **Reused models.** 0x00A331B0 / 0x00A70320 / 0x00A70490 use the C3DMeshVolume and CCountedPointer models from the
  landed 0x00A6F390. The landed 0x00A34420 / 0x00A71B40 call the same 12-byte record `SNamedPointer` with a
  CFaction / CNavConnectorNode pointer. All three call 0x00A32960, so one name should win.
