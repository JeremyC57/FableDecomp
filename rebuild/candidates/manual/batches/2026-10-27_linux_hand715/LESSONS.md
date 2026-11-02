# chunk 715 lessons

## GOTCHAS one-liners
- awk `$2==a` with addresses like `00e37680` compares NUMERICALLY (0e37680 == 0): force string compare `($2 "")==(a "")` when querying reloc_bindings.tsv.
- reloc_bindings.tsv columns are symbol, bound target, caller address; read column 2 for "what spelling binds this address".
- An implicit dtor that only ??_G uses gets inlined into ??_G and no ??1 is emitted (grade falls back to another function, e.g. DIFFER 53v84); a destroy-by-name harness `p->T::~T()` emits it (chunks 656/657/712/715).
- A dtor made virtual only through its base (CTextureRenderer : CBaseVideoRenderer) mangles UAE under VC7.1 even when the PDBs say QAE; PDB spelling can describe a build where the class had no base.
- sizeof asserts for classes with __int64 members (REFERENCE_TIME in strmbase) round to 8: CTextureRenderer is 0x180, confirmed by the caller's `push 180h`.
- A class whose dtor tail calls ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300 (not ~CBaseClass 0x0099A2E0) derives through CBaseClassNonCopyable; check the tail reloc before writing the comment.
- CModeStandBase is global (RTTI .?AVCModeStandBase@@, ??1CModeStandBase@@UAE@XZ) even though its derived modes live in NCreatureMode.

- Fable's DirectShow strmbase renames CBaseObject to CMyBaseObject (RTTI); a local strmbase CBaseObject trips vland's SHARED_HEADER_AVAILABLE warning against engine/CBaseObject.h.

## Review at landing
- Coordinator decision on 0x00A3B970 Leaf `??1CTextureRenderer@@UAE@XZ` vs the PDBs' QAE (NOTES.md).
- Respell the eight landed ??_G stand-in callees and the archery ctor/vtable (NAME_FIXES.tsv).
- Chunk 710's CAnimFilterBase slot order and its labels for 0x0073E970 / 0x0073E9A0 (NOTES.md header debt).
- hdr/ overlays FABLE_RETAIL_CGAMESCRIPTTHING and FABLE_RETAIL_CTEXTURE are opt-in; default text unchanged.
