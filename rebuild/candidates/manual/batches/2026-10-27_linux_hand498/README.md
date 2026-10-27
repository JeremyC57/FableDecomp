# Chunk 498 (agent498, 2026-10-27): 39 of 40 single functions byte-exact

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00CA85A0..0x00CAF160: DirectShow BaseClasses
-- amfilter.cpp pins, CMediaSample, CBaseAllocator; ctlutil.cpp CMediaPosition, CPosPassThru, CDeferredCommand). One
Opus 5.5 agent at high effort; the landed strmbase models (0x00CA9C80, 0x00CA93F0, 0x00CAF090, 0x00CAE170) were reused
verbatim, so every row but one matched on its first grade.

- **Landed:** 39 rows with `vland.py --land` (verify.log: 19 MATCH, 20 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). The 39 SHARED_HEADER_AVAILABLE warnings are strmbase's own CBaseObject, a different class from the
  game header of that name (as on the earlier strmbase rows).
- **Skipped (1):** 0x00CAA470 holds CBaseAllocator's three DECLARE_IUNKNOWN bodies (QueryInterface, AddRef, Release at
  0x00CAA470 / 0x00CAA480 / 0x00CAA490); catalog re-split needed.
- **Identities:** all 39 proven by the BaseClasses source body plus the vtable slot
  (ghidra_out/labels_chunk498_2026-10-27.tsv). 0x00CAA770 is the compiler's adjustor thunk
  ``CDynamicOutputPin::Release`adjustor{148}'`` (leaf `?Release@CDynamicOutputPin@@WJE@AGKXZ`).

Label corrections (NOTES.md): 0x00CA96F0 / 0x00CA9700 / 0x00CA9710 (`Thunk_VirtualMethod_0..2`) are
CDynamicOutputPin::QueryInterface / AddRef / Release; landed 0x00CAEC80 `CAdviseHolder_00caec80::Unadvise` is
CDeferredCommand::Cancel and 0x00CAECB0 is CDeferredCommand::Confidence; reloc_bindings still names 0x00CAE170
`NativeDeferredCommandQueue::CheckTime` where the landed symbol is CCmdQueue::CheckTime. Review: 0x00CAA770 defines an
empty stand-in destructor only so the vtables (and the thunk) are emitted; 0x00CA9700 uses a model copy in which
CDynamicOutputPin declares its IUnknown methods instead of the inline macro.
