#ifndef FABLE_ENGINE_C3DMESHFILELOADER_H
#define FABLE_ENGINE_C3DMESHFILELOADER_H
// Retail opt-in (Codex pass 12): original PDB types and native chunk ctor/dtor offsets.
#include "engine/CChunkedFileLoader.h"
#include "engine/C3DMeshFileRootChunk.h"
class C3DMeshFileLoader:public CChunkedFileLoader {
public:C3DMeshFileLoader();virtual ~C3DMeshFileLoader();
 CCountedPointer<CChunkedFileChunk> RegisterRootSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,RootChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterSubMeshSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,SubMeshChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterPrimitiveSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,PrimitiveChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterTrianglesSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,TrianglesChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterGroupSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,GroupChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterMaterialListSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,MaterialListChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterMaterialSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,MaterialChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterMapSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,MapChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterHelpersSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,HelpersChunkType->GetAsSuperChunk());}
 void RegisterClothSubChunkType(const CChunkedFileChunk&);
private:
 CCountedPointer<CChunkedFileChunk>AddChunkType(const CChunkedFileChunk&,CChunkedFileSuperChunk*);
 CCountedPointer<C3DMeshFileRootChunk> RootChunkType;
 CCountedPointer<CChunkedFileChunk> SubMeshChunkType;
 CCountedPointer<CChunkedFileChunk> TransformationChunkType;
 CCountedPointer<CChunkedFileChunk> BoneChunkType;
 CCountedPointer<CChunkedFileChunk> PrimitiveChunkType;
 CCountedPointer<CChunkedFileChunk> TrianglesChunkType;
 CCountedPointer<CChunkedFileChunk> SmoothingGroupsChunkType;
 CCountedPointer<CChunkedFileChunk> VerticesChunkType;
 CCountedPointer<CChunkedFileChunk> UniqueVerticesChunkType;
 CCountedPointer<CChunkedFileChunk> GroupChunkType;
 CCountedPointer<CChunkedFileChunk> PrimitiveClothChunkType;
 CCountedPointer<CChunkedFileChunk> SubMeshClothChunkType;
 CCountedPointer<CChunkedFileChunk> MaterialListChunkType;
 CCountedPointer<CChunkedFileChunk> MaterialChunkType;
 CCountedPointer<CChunkedFileChunk> MaterialExtChunkType;
 CCountedPointer<CChunkedFileChunk> MapChunkType;
 CCountedPointer<CChunkedFileChunk> EngineMaterialChunkType;
 CCountedPointer<CChunkedFileChunk> HelpersChunkType;
 CCountedPointer<CChunkedFileChunk> HelperPointChunkType;
 CCountedPointer<CChunkedFileChunk> DummyObjectChunkType;
 CCountedPointer<CChunkedFileChunk> ConvexVolumeChunkType;
 CCountedPointer<CChunkedFileChunk> MirroredTextureChunkType;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileLoader)==192);

#endif
