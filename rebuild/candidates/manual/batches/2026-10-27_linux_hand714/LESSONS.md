# Chunk 714 lessons
- An implicit (compiler-generated) virtual dtor stores no vtable. Define the class's PDB ctor in the TU and VC7.1 emits the vtable, ??_G and the out-of-line implicit dtor. It matched on probe 1 four times here.
- A ??_G emitted from the same TU, graded against the retail slot-0 caller, proves both the dtor name and the ??_G name at once.
- A polymorphic member with an inline virtual dtor (CSimpleMemoryPool) expands inline in the owner's implicit dtor: vtable store, body, base dtor. Model it with the inline dtor, not an out-of-line call.
- Including a flattened shared header under `#define X XLayout` / `#undef X`, then deriving the real class from it, keeps the shared layout and silences SHARED_HEADER_AVAILABLE.
- An overlay proof by perturbation can stay MATCH when the perturbed member falls into 8-byte alignment padding. Perturb a header that the row actually touches at an offset that shifts.
- A repo header can change mid-session when another chunk lands. Diff against the live repo copy before handing back, and rebase the overlay.
- The STLport 4.6 list sentinel is `_STLP_alloc_proxy<_Node*, _Node, allocator<_Node> >`. A missing `allocator` template gives "base class undefined".
- Catalog labels propagated across byte-identical dtor twins (ReplaceableMesh -> TintEffect / MultipleStaticMeshes) are wrong. Name each twin from its own vtable and ??_G caller.

## Review at landing
- hdr/engine/CTCBase.h: merge with chunk 712's hand-back (only the two PeekWorldMap lines are new from 714).
- hdr/engine/CSimpleMemoryPool.h: new opt-in branch; 52-includer recheck clean.
- NAME_FIXES.tsv: 22 rows. Landed callers 0x009BFB30, 0x009BF550, 0x007E27F0 and 0x00B260B0, plus the seven ??_G sources, carry the wrong spellings.
