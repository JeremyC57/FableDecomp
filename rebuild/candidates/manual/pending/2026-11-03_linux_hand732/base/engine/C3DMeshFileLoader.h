#ifndef FABLE_ENGINE_C3DMESHFILELOADER_H
#define FABLE_ENGINE_C3DMESHFILELOADER_H
// New header (chunk 725b): C3DMeshFileLoader : CChunkedFileLoader (rtti_bases.tsv), Ego_r sizeof 0xC0: 22 CCountedPointer chunk
// prototypes from +0x10 (no STL member, so the retail size is the PDB's). Constructor ??0C3DMeshFileLoader@@QAE@XZ 0x00A9BBE0.
// The Register*SubChunkType members are out of line in retail (0x00A9B840 Root, 0x00A9B870 SubMesh, 0x00A9B8A0 Primitive,
// 0x00A9B8D0 Triangles, 0x00A9BA10 Map; FableWin QAE spellings, by-value CCountedPointer result through the hidden pointer).
// Needs FABLE_RETAIL_CCHUNKEDFILECHUNK (engine/CChunkedFileChunk.h retail branch).
#include "rebuild_abi.h"
#include "engine/CChunkedFileLoader.h"
#include "engine/CCountedPointer.h"
class CChunkedFileChunk;
class CChunkedFileSuperChunk;
class C3DMeshFileRootChunk;
class C3DMeshFileLoader : public CChunkedFileLoader {
public:
    C3DMeshFileLoader();
    virtual ~C3DMeshFileLoader();
    CCountedPointer<CChunkedFileChunk> RegisterRootSubChunkType(const CChunkedFileChunk&);        // 0x00A9B840
    CCountedPointer<CChunkedFileChunk> RegisterSubMeshSubChunkType(const CChunkedFileChunk&);     // 0x00A9B870
    CCountedPointer<CChunkedFileChunk> RegisterPrimitiveSubChunkType(const CChunkedFileChunk&);   // 0x00A9B8A0
    CCountedPointer<CChunkedFileChunk> RegisterTrianglesSubChunkType(const CChunkedFileChunk&);   // 0x00A9B8D0
    CCountedPointer<CChunkedFileChunk> RegisterGroupSubChunkType(const CChunkedFileChunk&);
    CCountedPointer<CChunkedFileChunk> RegisterMaterialListSubChunkType(const CChunkedFileChunk&);
    CCountedPointer<CChunkedFileChunk> RegisterMaterialSubChunkType(const CChunkedFileChunk&);
    CCountedPointer<CChunkedFileChunk> RegisterMapSubChunkType(const CChunkedFileChunk&);         // 0x00A9BA10
    CCountedPointer<CChunkedFileChunk> RegisterHelpersSubChunkType(const CChunkedFileChunk&);
    void RegisterClothSubChunkType(const CChunkedFileChunk&);
private:
    CCountedPointer<CChunkedFileChunk> AddChunkType(const CChunkedFileChunk&, CChunkedFileSuperChunk*);
    CCountedPointer<C3DMeshFileRootChunk> RootChunkType;          // +0x10
    CCountedPointer<CChunkedFileChunk> SubMeshChunkType;          // +0x18
    CCountedPointer<CChunkedFileChunk> TransformationChunkType;   // +0x20
    CCountedPointer<CChunkedFileChunk> BoneChunkType;             // +0x28
    CCountedPointer<CChunkedFileChunk> PrimitiveChunkType;        // +0x30
    CCountedPointer<CChunkedFileChunk> TrianglesChunkType;        // +0x38
    CCountedPointer<CChunkedFileChunk> SmoothingGroupsChunkType;  // +0x40
    CCountedPointer<CChunkedFileChunk> VerticesChunkType;         // +0x48
    CCountedPointer<CChunkedFileChunk> UniqueVerticesChunkType;   // +0x50
    CCountedPointer<CChunkedFileChunk> GroupChunkType;            // +0x58
    CCountedPointer<CChunkedFileChunk> PrimitiveClothChunkType;   // +0x60
    CCountedPointer<CChunkedFileChunk> SubMeshClothChunkType;     // +0x68
    CCountedPointer<CChunkedFileChunk> MaterialListChunkType;     // +0x70
    CCountedPointer<CChunkedFileChunk> MaterialChunkType;         // +0x78
    CCountedPointer<CChunkedFileChunk> MaterialExtChunkType;      // +0x80
    CCountedPointer<CChunkedFileChunk> MapChunkType;              // +0x88
    CCountedPointer<CChunkedFileChunk> EngineMaterialChunkType;   // +0x90
    CCountedPointer<CChunkedFileChunk> HelpersChunkType;          // +0x98
    CCountedPointer<CChunkedFileChunk> HelperPointChunkType;      // +0xA0
    CCountedPointer<CChunkedFileChunk> DummyObjectChunkType;      // +0xA8
    CCountedPointer<CChunkedFileChunk> ConvexVolumeChunkType;     // +0xB0
    CCountedPointer<CChunkedFileChunk> MirroredTextureChunkType;  // +0xB8
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileLoader) == 0xC0);
#endif
