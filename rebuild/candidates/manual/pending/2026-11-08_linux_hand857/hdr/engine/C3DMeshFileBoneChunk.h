#ifndef FABLE_ENGINE_C3DMESHFILEBONECHUNK_H
#define FABLE_ENGINE_C3DMESHFILEBONECHUNK_H
// Retail opt-in (chunk 851, row 0x00A9BBE0 C3DMeshFileLoader::C3DMeshFileLoader): the polymorphic retail class, RTTI
// .?AVC3DMeshFileBoneChunk@@ : CChunkedFileSuperChunk (ghidra_out/rtti_bases.tsv); members Ego_r.pdb (struct_layouts_egor.tsv), retail sizeof 0x70
// (12-byte STLport vectors). Default constructor out of line (0x00AB9DA0); no user destructor (the implicit one runs the member destructors and the base's out-of-line one, as retail tears the temporaries down).
// Codex pass 12 (verified_pending/00a9bbe0) class model, rebased onto our CChunkedFileChunk / CChunkedFileSuperChunk retail branches. Needs FABLE_RETAIL_CCHUNKEDFILECHUNK / FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK.
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/CMatrix3x4.h"
#include "fable_string.h"
class C3DMeshFileBoneChunk : public CChunkedFileSuperChunk {
public:
    C3DMeshFileBoneChunk();
    virtual const CCharString GetChunkDescription() const;
    virtual const CCharString GetChunkInfo() const;
    virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
#ifdef FABLE_RETAIL_C3DMESHFILEBONECHUNK_GETBONE
    // Retail opt-in (chunk 857, row 0x00A8B990 C3DMesh2::InitialiseBones): Ego_r.pdb / FableWin.pdb ?GetBone@C3DMeshFileBoneChunk@@QBE
    // ?BVC3DBone2@@XZ (out of line, 0x00AB9DD0, landed: the hidden result is the fresh `new C3DBone2` block, `push eax ; call
    // 0x00AB9DD0` at 0x00A8B9E3) and FableWin ?GetName@C3DMeshFileBoneChunk@@QBE?BVCCharString@@XZ (inline in retail: the Name
    // member +0x6C copied straight into the caller's temporary, `add ecx,6Ch ; push ecx ; call CCharString(const CCharString&)`,
    // 0x00A8BB04). C3DBone2 has no shared header; the includer declares it.
    const class C3DBone2 GetBone() const;                              // 0x00AB9DD0
    const CCharString GetName() const { return Name; }
#endif
private:
    long BoneIndex;
    long ParentIndex;
    long FirstChildIndex;
    long NextSiblingIndex;
    CMatrix3x4 LocalizationMatrix;
    CCharString Name;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileBoneChunk) == 0x70);
#endif // FABLE_ENGINE_C3DMESHFILEBONECHUNK_H
