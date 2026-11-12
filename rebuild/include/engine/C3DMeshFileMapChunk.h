#ifndef FABLE_ENGINE_C3DMESHFILEMAPCHUNK_H
#define FABLE_ENGINE_C3DMESHFILEMAPCHUNK_H
// Retail opt-in (chunk 851, row 0x00A9BBE0 C3DMeshFileLoader::C3DMeshFileLoader): the polymorphic retail class, RTTI
// .?AVC3DMeshFileMapChunk@@ : CChunkedFileSuperChunk (ghidra_out/rtti_bases.tsv); members Ego_r.pdb (struct_layouts_egor.tsv), retail sizeof 0x3C
// (12-byte STLport vectors). Default constructor out of line (0x00ABFE50); no user destructor (the implicit one runs the member destructors and the base's out-of-line one, as retail tears the temporaries down).
// Codex pass 12 (verified_pending/00a9bbe0) class model, rebased onto our CChunkedFileChunk / CChunkedFileSuperChunk retail branches. Needs FABLE_RETAIL_CCHUNKEDFILECHUNK / FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK.
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "fable_string.h"
enum EMaterialMapType;
class C3DMeshFileMapChunk : public CChunkedFileSuperChunk {
public:
    C3DMeshFileMapChunk();
    virtual const CCharString GetChunkDescription() const;
    virtual const CCharString GetChunkInfo() const;
    virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
#ifdef FABLE_RETAIL_C3DMESHFILEMAPCHUNK_METHODS
    // Opt-in (row 0x00AC53F0): FableWin.pdb inline getters.
    EMaterialMapType GetType() const { return MapType; }               // ?GetType@C3DMeshFileMapChunk@@QBE?AW4EMaterialMapType@@XZ
    const CCharString GetFilename() const { return Filename; }         // ?GetFilename@C3DMeshFileMapChunk@@QBE?BVCCharString@@XZ
#endif
private:
    EMaterialMapType MapType;
    CCharString MapDescription;
    CCharString Filename;
    unsigned long CRC;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileMapChunk) == 0x3C);
#endif // FABLE_ENGINE_C3DMESHFILEMAPCHUNK_H
