#ifndef FABLE_ENGINE_C3DMESHFILEROOTCHUNK_H
#define FABLE_ENGINE_C3DMESHFILEROOTCHUNK_H
// Retail opt-in (chunk 851, row 0x00A9BBE0 C3DMeshFileLoader::C3DMeshFileLoader): the polymorphic retail class, RTTI
// .?AVC3DMeshFileRootChunk@@ : CChunkedFileSuperChunk (ghidra_out/rtti_bases.tsv); members Ego_r.pdb (struct_layouts_egor.tsv), retail sizeof 0x2C
// (12-byte STLport vectors). Default constructor inline ("3DRT"; C3DMeshFileLoader::C3DMeshFileLoader 0x00A9BBE0 news it); no user destructor (the implicit one runs the member destructors and the base's out-of-line one, as retail tears the temporaries down).
// Codex pass 12 (verified_pending/00a9bbe0) class model, rebased onto our CChunkedFileChunk / CChunkedFileSuperChunk retail branches. Needs FABLE_RETAIL_CCHUNKEDFILECHUNK / FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK.
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk {
public:
    C3DMeshFileRootChunk() : CChunkedFileSuperChunk("3DRT") { }
    virtual const CCharString GetChunkDescription() const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileRootChunk) == 0x2C);
#endif // FABLE_ENGINE_C3DMESHFILEROOTCHUNK_H
