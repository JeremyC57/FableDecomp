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
#ifdef FABLE_RETAIL_C3DMESHFILE_GETROOT_BODY
#ifndef FABLE_RETAIL_CCHUNKEDFILE_GETCHUNKEDFILEROOT
#error FABLE_RETAIL_C3DMESHFILE_GETROOT_BODY needs FABLE_RETAIL_CCHUNKEDFILE_GETCHUNKEDFILEROOT
#endif
#include "engine/C3DMeshFileRootChunk.h"
#endif
class C3DMeshFileRootChunk;
class C3DMeshFile : public CChunkedFile {
public:
    C3DMeshFile();
#ifdef FABLE_RETAIL_C3DMESHFILE_LOADER_FRIEND
    // Per-TU opt-in (NF422, lib_3d_mesh_file.obj unit 0x00A9AD80): C3DMeshFileLoader::LoadMeshFile 0x00A9B640 calls the PRIVATE root-chunk
    // constructor 0x00A9AE60 (Ego_r.pdb 0x00C0BF60 / FableWin.pdb 0x033D5540 spell it AAE), so the loader is a friend. Access only.
    friend class C3DMeshFileLoader;
#endif
#ifdef FABLE_RETAIL_C3DMESHFILE_GETROOT_BODY
    // Retail opt-in (wfE8, row 0x00A2F740): the body of the landed 0x00A32620 (FableWin.pdb ?GetRoot@C3DMeshFile@@QBE..XZ; no Ego_r.pdb
    // copy, so an inline header member whose retail COMDAT sits among the mesh-stats TU's COMDATs). Visible to the caller, which still
    // calls it out of line; c2 then knows the result slot is not captured.
    CCountedPointer<const C3DMeshFileRootChunk> GetRoot() const {
        CCountedPointer<const C3DMeshFileRootChunk> root;
        root.SetStaticCast(GetChunkedFileRoot());
        return root;
    }
#else
    CCountedPointer<const C3DMeshFileRootChunk> GetRoot() const;        // 0x00A32620
#endif
    CCountedPointer<C3DMeshFileRootChunk> GetRoot();
#ifdef FABLE_RETAIL_C3DMESHFILE_ROOT_CTOR
    // Per-TU opt-in (NF419, rows 0x00A9AE60 / 0x00A9D2A0, lib_3d_mesh_file.obj): the private root-chunk constructor, Ego_r.pdb 0x00C0BF60
    // and FableWin.pdb 0x033D5540 ??0C3DMeshFile@@AAE@ABV?$CCountedPointer@VC3DMeshFileRootChunk@@@@@Z (retail 0x00A9AE60). Declaration only.
private:
    C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root);
#endif
};
FABLE_STATIC_ASSERT_AT(41, sizeof(C3DMeshFile) == 0x14);
#endif // FABLE_ENGINE_C3DMESHFILE_H
