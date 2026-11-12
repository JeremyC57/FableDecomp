#ifndef FABLE_ENGINE_C3DPRIMITIVE2_H
#define FABLE_ENGINE_C3DPRIMITIVE2_H
#include "engine/C3DTextureStages.h"
#include "engine/N3DPrimitive_CStaticBlock.h"
#include <stddef.h>
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH
#ifndef FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR
#endif
#include "boost/scoped_ptr.h"
class CVertexBufferWin32;
class CIndexBuffer;
#endif
// Full original member range: donor172 becomes native160 after three16-byte CArrays shrink to12.
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
#include "engine/CMovableResource.h"
class C3DPrimitive2 : public CMovableResource {
#else
class C3DPrimitive2 {
#endif
public:
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
    virtual ~C3DPrimitive2();
    virtual void RelocateData(long,void*,void*,long);
#else
    unsigned char UnmodelledBase[4];
#endif
    long MaterialIndex,RepeatingMeshReps;unsigned char BoundingSphere[16];float AverageTextureStretch;
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH
    // Retail opt-in (chunk 2059, row 0x00BC25F0 CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive): Ego_r.pdb
    // scoped_ptr<CVertexBufferWin32> VertexBuffer / scoped_ptr<CIndexBuffer> IndexBuffer typed (one pointer each, the same +0x30 / +0x34),
    // and the FableWin.pdb inline accessors the row uses (no out-of-line copy in Ego_r.pdb or Fable.exe; FableWin debug sizes:
    // GetMaterialIndex 17b/9i, GetRepeatingMeshBatchSize 17b/9i, GetVertexCount 17b/9i, GetTriangleCount 17b/9i, GetVertexBuffer 22b/10i,
    // GetIndexBuffer 345b/94i = assert code; retail reads +0x04 / +0x08 / +0x58 / +0x5C / +0x30 / +0x34 in place, 0x00BC2CCC..0x00BC2F50).
    unsigned char ClothPrimitives[12],Data[4];
    boost::scoped_ptr<CVertexBufferWin32> VertexBuffer;
    boost::scoped_ptr<CIndexBuffer> IndexBuffer;
#else
    unsigned char ClothPrimitives[12],Data[4],VertexBuffer[4],IndexBuffer[4];
#endif
    unsigned long VertexBufferStatus,IndexBufferStatus,StaticBlockCount;
    N3DPrimitive::CStaticBlock* StaticBlocks;
    unsigned long AnimatedBlockCount;unsigned char AnimatedBlocks[4];
    unsigned long AnimatedBlocksStatus,RepeatedBlocksStatus,VertexCount,TriangleCount,IndexCount;
    bool UploadedFlag;unsigned char VertexStreamFlags;unsigned long InitFlags;
    C3DTextureStages RecordedTextureStages;void* RecordedConstantData;long RecordedFrame;
    unsigned char PartitionedMeshSourceIndexBuffer[12],PartitionTree[8],MirroredTextureVertexIndexList[12];
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH
    long GetMaterialIndex() const { return MaterialIndex; }
    long GetRepeatingMeshBatchSize() { return RepeatingMeshReps; }
    unsigned long GetVertexCount() const { return VertexCount; }
    unsigned long GetTriangleCount() const { return TriangleCount; }
    CVertexBufferWin32* GetVertexBuffer() { return VertexBuffer.get(); }
    CIndexBuffer* GetIndexBuffer() { return IndexBuffer.get(); }
#endif
    N3DPrimitive::CStaticBlock* GetStaticBlock(unsigned long index){return &StaticBlocks[index];}
    unsigned long GetStaticBlockCount()const{return StaticBlockCount;}
#ifdef FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
    void SetRecordedFrame(long frame){RecordedFrame=frame;}
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DPrimitive2)==0xa0);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,StaticBlockCount)==0x40);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,StaticBlocks)==0x44);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,RecordedFrame)==0x7c);
#endif
