# NF192 header overlay

Every overlay header is a copy of the repo header (base copy in ../orig/, byte-equal to the repo at the start of this session) with
`#ifdef` opt-ins only, member-specific macro names. With none of the NF192 macros defined, each header is token-identical to the repo
header; the only other change is pin_asserts.py rewriting `FABLE_STATIC_ASSERT(...)` to `FABLE_STATIC_ASSERT_AT(<repo line>, ...)`
so the assert typedef names keep their repo line numbers. No landed row defines any NF192 macro (grep of rebuild/), so the opt-ins
change none of the landed includers listed below. NF190 / NF191 overlays touch none of these headers.

| header | opt-in | declares | row |
|---|---|---|---|
| engine/CChunkedFileSuperChunk.h | FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_ADDSUBCHUNK | `void AddSubChunk(const CCountedPointer<CChunkedFileChunk>&);` (0x00AA84F0, decl only) | 0x00AC0C00 |
| engine/C3DMeshFileMaterialChunk.h | FABLE_RETAIL_C3DMESHFILEMATERIALCHUNK_CREATEMAP | `CCountedPointer<C3DMeshFileMapChunk> CreateMap(EMaterialMapType);` (0x00AC0C00) + its CCountedPointer include / forward decls | 0x00AC0C00 |
| engine/NAnimationEntryBodge_CAnimEntryLite.h | FABLE_RETAIL_CANIMENTRYLITE_INLINE_ACCESSORS | `GetDelayTime()` / `GetAnimateThingFlags()` with inline bodies (`#else` keeps the repo declarations; FableWin.pdb names, no Ego_r copy) | 0x00861A80 |
| engine/C3DMeshFileMapChunk.h | FABLE_RETAIL_C3DMESHFILEMAPCHUNK_TYPE_CTOR | inline `C3DMeshFileMapChunk(EMaterialMapType type) : CChunkedFileSuperChunk("MMAP"), MapType(type) { }` (Ego_r 0x00C3C940 / FableWin 0x0347D2D0; inline in retail 0x00AC0C00) | 0x00AC0C00 |

pin_asserts.py: CChunkedFileSuperChunk.h pinned 0 (already `_AT`), C3DMeshFileMaterialChunk.h pinned 12, C3DMeshFileMapChunk.h pinned 1,
NAnimationEntryBodge_CAnimEntryLite.h pinned 1.

Landed includers (grep -rlE '<header>' rebuild/src/compiled): CChunkedFileSuperChunk.h 23, C3DMeshFileMaterialChunk.h 6,
C3DMeshFileMapChunk.h 2, NAnimationEntryBodge_CAnimEntryLite.h 4; none defines an NF192 macro, so none changes.
