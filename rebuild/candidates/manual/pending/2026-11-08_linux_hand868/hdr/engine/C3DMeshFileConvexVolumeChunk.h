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
#ifdef FABLE_RETAIL_C3DMESHFILECONVEXVOLUMECHUNK_GETTERS
    // Retail opt-in (chunk 868, row 0x00A2F740 C3DMeshStats::InitialiseFromFileData): FableWin.pdb ?GetName@C3DMeshFileConvexVolumeChunk@@QBEABVCCharString@@XZ,
    // ?GetPlaneCount@..@@QBEKXZ, ?PeekPlanes@..@@QBEPBVCPlane@@XZ -- inline in retail (0x00A2FCF9 reads Name +0x2C in place; the landed
    // C3DMeshVolume constructor 0x00A2E9A0 reads the plane array through the CArray GetAsCArray idiom).
    const CCharString& GetName() const { return Name; }
    unsigned long GetPlaneCount() const { return Planes.size(); }
    const CPlane* PeekPlanes() const { return Planes.GetAsCArray(); }
#endif
private:
    CCharString Name;
    CArray<CPlane> Planes;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileConvexVolumeChunk) == 0x3C);
#endif // FABLE_ENGINE_C3DMESHFILECONVEXVOLUMECHUNK_H
