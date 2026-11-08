#ifndef FABLE_ENGINE_C3DMESHFILE_H
#define FABLE_ENGINE_C3DMESHFILE_H
// C3DMeshFile (chunk 868, row 0x00A2F740 C3DMeshStats::InitialiseFromFileData): the mesh file over the chunked-file base. RTTI
// .?AVC3DMeshFile@@ : CChunkedFile (ghidra_out/rtti_bases.tsv); no members of its own (sizeof 0x14 = CChunkedFile: vptr, RootChunk +0x4,
// Version +0xC, FileComment +0x10 -- the landed 0x00A32620 layout). FableWin.pdb: ??0C3DMeshFile@@QAE@XZ,
// ?GetRoot@C3DMeshFile@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileRootChunk@@@@XZ (retail 0x00A32620, landed: the root chunk pointer
// static-cast into a const counted pointer, returned by value) and the non-const ?GetRoot@C3DMeshFile@@QAE?AV?$CCountedPointer@VC3DMeshFileRootChunk@@@@XZ.
#include "rebuild_abi.h"
#include "engine/CChunkedFile.h"
#include "engine/CCountedPointer.h"
class C3DMeshFileRootChunk;
class C3DMeshFile : public CChunkedFile {
public:
    C3DMeshFile();
    CCountedPointer<const C3DMeshFileRootChunk> GetRoot() const;        // 0x00A32620
    CCountedPointer<C3DMeshFileRootChunk> GetRoot();
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFile) == 0x14);
#endif // FABLE_ENGINE_C3DMESHFILE_H
