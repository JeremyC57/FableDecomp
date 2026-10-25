# Chunk 478 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- x87 operand order:
  - Name every vector temp in source order (`p0 = *it; p1 = it[1]; p2 = it[2]; toFirst = first - p1; ...`). Building
    the temps inside the call lets VC's right-to-left argument evaluation reorder the subtractions. Fixed at
    0x00760760.
  - `-eps` with `extern const float g_Epsilon;` gives retail's hoisted `fld [g]; fchs` kept on st(1). A literal
    `-0.0001f` folds to a pool constant instead (0x00760190).
- Register pressure:
  - Reuse one iterator: `it = begin(); first = *it; for (++it; it != last; ++it)` matches retail. A separate begin
    local swaps two registers (0x00760760).
  - STLport `min(a, b)` takes references and forces the count into memory; a ternary keeps it in a register.
  - `if (n > 1) { ...; return true; } return false;` puts the false block at the function end, as retail does
    (0x0076E4D0).
- C3DVector:
  - `v *= k` keeps the components on the x87 stack; `v = v * k` goes through memory temps (0x00760190, 0x0079AA90).
  - `GetLength()` written as `sqrt(Dot(*this))` evaluates z, y, x like retail.
  - A `Normalise()` that returns the length gives retail's `fld1; fdiv; fst inv; fmul st(2)` reciprocal sequence
    (0x0079AA90, 0x007BBC9F).
- `#pragma optimize("s", on)` TUs (NThingShot: unaligned entries, fld1/fldz): small inline C3DVector helpers are not
  inlined unless marked `__forceinline` (0x007BBC9F).
- STLport deque:
  - `_S_buffer_size()` is `_MAX_BYTES` (128) / sizeof(T), so a node is 0x80 bytes (16 eight-byte crimes). A 512-byte
    guess shifts every constant (0x007B3760).
  - The iterator `++` is a member `_M_increment` (0x007B2E20).
- CDefPointer:
  - A temporary `CDefPointer<T>` releases through `Object->ReleaseTemporary(); Object = 0;` (0x007B2E20).
  - A member CDefPointer releases through the pointee's inline ReleaseReference (0x007676A0).
- A named `CCharString` local for a looked-up string fixes the temp slot order in a map-assign loop (0x00766C00).
- STLport map walks use the static `_Rb_global<bool>::_M_increment(node)`. /Ob2 inlines it with the walk in eax,
  copied back to esi (0x00766550, 0x00766C00).
- Same-TU `__declspec(noinline)` stand-ins under `auto_inline(off)` for callees that take a local's address:
  - They removed a dead list-header zero store and a reload at 0x007AA880, but not its register allocation.
  - They did not change copy scheduling at 0x0076E4D0. Try them once, then park.
- VC's own inline budget reproduces retail's mix of inline and out-of-line STLport helpers (0x0076B950: inline
  `__copy_backward_ptrs`, out-of-line `uninitialized_fill_n`). Do not force either side.
- Lionhead's inline-asm ceil is a retail_asm signature: `fld q; fadd 0.5; fistp i; fild i; fsub 1.0; fstp t`, a bit
  compare of q and t, then `dec`, with an ebp frame. Seen at 0x00774610 and 0x00779EF0. Park after one reference
  grade.
- Declare callees with their Ego_r.pdb access. A private member mangles `AAE`, not `QAE`, so a public declaration
  names another symbol at landing even when q.py masks it (0x00768210 -> SetupAugmentationSounds).

Review at landing:
- No row needed more than 4 grades. 00768210 and 007b3760 used exactly 4: one compile error each, plus a regrade after
  a model fix (the callee made private; the deque buffer size set to 128).
- Already-landed sources with wrong names or comments:
  - 0x00768310 OnSetInLimbo calls 0x00766C00 "UpdateParticleMorphs"; it is SetupAugmentationSounds.
  - 0x007688B0 says CIVCountedPointer; the element is CDefPointer<const CThingBaseDef>.
  - 0x007AF980 names vtable 0x0123E7E0; it is CIntelligentPointer<CThingAICreature>.
- Callee labels used by landed code: 0x0066ADD0 is CTCSoundPlayer::StopSample (not SetToKillOnLevelUnload), 0x00769650
  is `_Rb_tree::_M_erase` (not BuildTreeArray), 0x00686830 is ~CTCBase (not ~CChunkedFileChunk).
- Local base models next to an existing engine header: rebuild/include/engine/CTCVillage.h exists, but it carries the
  Ego_r layout (VillageMarkers +0xEC with 12-byte lists; retail +0x70). 007b2e20 and the parked 007b6b10 use local
  CTCVillage models. The other classes here (CTCGuard, CTCObjectAugmentations, CTCShop, ...) have no engine header.
- 007748e0 and 007808b0 identities are "likely" (from strings and the caller's tail jump). Confirm them against the
  Ego_r.pdb name sets before naming the rows.
- 00774610 and 00779ef0 are retail_asm candidates. They may land under the 2026-10-24 retail_asm exception with a
  minimal inline-asm ceil and a `// Retail-asm:` evidence line.
