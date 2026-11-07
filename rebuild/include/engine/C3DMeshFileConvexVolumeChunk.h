#ifndef FABLE_ENGINE_C3DMESHFILECONVEXVOLUMECHUNK_H
#define FABLE_ENGINE_C3DMESHFILECONVEXVOLUMECHUNK_H
// Retail opt-in (chunk 851, row 0x00A9BBE0 C3DMeshFileLoader::C3DMeshFileLoader): the polymorphic retail class, RTTI
// .?AVC3DMeshFileConvexVolumeChunk@@ : CChunkedFileSuperChunk (ghidra_out/rtti_bases.tsv); members Ego_r.pdb (struct_layouts_egor.tsv), retail sizeof 0x3C
// (12-byte STLport vectors). Default constructor out of line (0x00AAAA10); no user destructor (the implicit one runs the member destructors and the base's out-of-line one, as retail tears the temporaries down).
// Codex pass 12 (verified_pending/00a9bbe0) class model, rebased onto our CChunkedFileChunk / CChunkedFileSuperChunk retail branches. Needs FABLE_RETAIL_CCHUNKEDFILECHUNK / FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK.
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/CArray.h"
#include "engine/CPlane.h"
#include "fable_string.h"
class C3DMeshFileConvexVolumeChunk : public CChunkedFileSuperChunk {
public:
    C3DMeshFileConvexVolumeChunk();
    virtual const CCharString GetChunkDescription() const;
    virtual const CCharString GetChunkInfo() const;
    virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
private:
    CCharString Name;
    CArray<CPlane> Planes;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileConvexVolumeChunk) == 0x3C);
#endif // FABLE_ENGINE_C3DMESHFILECONVEXVOLUMECHUNK_H
