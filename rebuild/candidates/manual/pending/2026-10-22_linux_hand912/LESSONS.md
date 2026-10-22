# Chunk 912 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00F33480 / 0x00F334A0) A persist hook that TAIL-JUMPS with `this` = its own dead argument slot (`mov [esp+0Ch],eax ; lea ecx,[esp+0Ch] ; jmp`) is `CPersistTraits<T> traits; traits.TransferIn(context, parser, &(*TransVector)[index]);` over an EMPTY traits specialisation -- and VC7.1 emits the tail jump only when the traits method's body is defined EARLIER in the same TU (`#pragma auto_inline(off)` stand-in). Declared-only, it keeps `push x3 ; call ; ret 0Ch` (35 v 25). Same family as the "callee facts from earlier in the TU" lesson.
- (0x00F33D40 family) A bitfield store through an indexed element (`mov cl,[eax+edx+24h] ; add eax,edx ; ... mov [eax+24h],cl`) needs the element in a named reference (`T& def = v[i]; def.Bit = b;`); the direct `v[i].Bit = b` folds the offset into a lea (37 v 38).
- (0x006421C0) Byte stores of an inline default ctor come out in REVERSE source order: CRGBColour opaque black with retail's +2/+1/+0 then late +3 is `A = 255; R = 0; G = 0; B = 0;`.
- (0x00D80AE0) A dead local built as ctor-into-temp / copy-ctor / temp-dtor is `T x = Helper();` where the inline Helper returns a NAMED local (no NRVO); `T x = T(..)` and `T x = ".."` are elided by VC7.1.
- (0x00F33B90) CPersistContext::TransferVectorOfSubComponents<T> with an out-of-line one-argument `resize(n)` is the landed 0x00F274D0 model with `value.resize(count)`; the 0x34/0x38/0x3C element families all share it.
- CPersistContext_TransferableVectorOfSubComponents<T> vtable = Size, Resize, AreDefaultValsApplied(long), SetDefaultValsApplied(long, bool), Transfer(CPersistContext&, long); DefaultValsApplied is bit 2 of the CDefClassBase +0x24 group (Setup:1, Template:1, DefaultValsApplied:1). engine/CDefClassBase.h flattens it to `unsigned char DefaultValsApplied[1]` (wrong bit position for codegen).

Review at landing:
- 0x00F33480 / 0x00F334A0 carry same-TU stand-in bodies for CPersistTraits<CDefString>::TransferIn/TransferOut (0x007E3820 / 0x007E3930); the lander must bind those calls to the retail addresses (RELOCATION_MATCH).
- 0x00449B60 declares the method on `CPlayerManager_Methods : CPlayerManager` (engine header is data-only).
- Local CDefClassBase-derived element models next to engine/CDefClassBase.h (bitfield group flattened in the header).
- The landed 0x00F274D0 header calls its wrapper CPersistContext_TransferableVector<T> and its name Transfer<CCreatureEntry>; by the PDB it is CPersistContext::TransferVectorOfSubComponents<T> with CPersistContext_TransferableVectorOfSubComponents<T> (and 0x009BC120 = TransferVectorOfSubComponentsLoadText).
- Chunk catalog rows: none held two functions; all rows graded at their stated length.
