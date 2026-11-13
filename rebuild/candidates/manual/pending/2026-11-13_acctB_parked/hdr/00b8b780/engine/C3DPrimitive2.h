#ifndef FABLE_ENGINE_C3DPRIMITIVE2_H
#define FABLE_ENGINE_C3DPRIMITIVE2_H
#include "engine/C3DTextureStages.h"
#include "engine/N3DPrimitive_CStaticBlock.h"
#include <stddef.h>
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#include "boost/scoped_ptr.h"
#include "engine/CIndexBuffer.h"
#endif
#ifdef FABLE_RETAIL_CANIMATEDBLOCK_DATA
// Retail opt-in (wf4, row 0x00A90040 N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump; needs FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS):
// types CAnimatedBlock::Data (+0x2C) as Ego_r.pdb's CIVCountedPointer<CAnimatedBlock::CDeviceIndependent>, adds that nested pointee
// (Ego_r 40 bytes with 16-byte CArrays, retail 0x20: CIVCountedPointeeBase +0, CArray<unsigned long> VertexList +0x08,
// CArray<C3DIndexedBoneWeight> BoneWeights +0x14) and declares the two private converters
// ?ConvertToVBFormatNonBump@CAnimatedBlock@N3DPrimitive@@AAEXPAVCTVertexNormBlend4Tex1@@AAVCDeviceIndependent@2@@Z (0x00A8FF50) and
// ?ConvertToVBFormatBump@CAnimatedBlock@N3DPrimitive@@AAEXPAVCTVertexNormBlend4Tex1Bump@@AAVCDeviceIndependent@2@@Z (0x00A90040).
// No layout change (Data stays 4 bytes at +0x2C).
#include "engine/CIVCountedPointer.h"
#include "engine/CArray.h"
#include "engine/C3DIndexedBoneWeight.h"
class CTVertexNormBlend4Tex1;
class CTVertexNormBlend4Tex1Bump;
namespace N3DPrimitive { class CDeviceIndependent; }
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
// Retail opt-in (chunk F2, row 0x00BCFB00): Ego_r.pdb N3DPrimitive::CAnimatedBlock (0x30 bytes, : CBaseBlock per the shared
// PrimitiveCount / StartIndex / IsStrip prefix the row reads at +0 / +4 / +8): BoneConstantsFixupOffset +0x0C, VertexCount +0x10,
// BonesPerVertex +0x14, PalettedFlag +0x16, GroupCount +0x17, Groups[18] +0x18, Data (CIVCountedPointer<CDeviceIndependent>) +0x2C;
// FableWin.pdb inline accessors ?GetGroupCount@CAnimatedBlock@N3DPrimitive@@QBEKXZ, ?GetGroup@..@@QBEKK@Z, ?GetVertexCount@..@@QBEKXZ.
// C3DPrimitive2 then types AnimatedBlocks (+0x4C) and gets ?GetAnimatedBlock@C3DPrimitive2@@QAEPAVCAnimatedBlock@N3DPrimitive@@K@Z,
// ?GetAnimatedBlockCount@..@@QBEKXZ and ?GetVertexCount@..@@QBEKXZ (all inline in retail). No layout change.
namespace N3DPrimitive {
class CAnimatedBlock : public CBaseBlock {
public:
    unsigned long GetVertexCount() const { return VertexCount; }
    unsigned long GetGroupCount() const { return GroupCount; }
    unsigned long GetGroup(unsigned long index) const { return Groups[index]; }
    unsigned long BoneConstantsFixupOffset;                              // +0x0C
    unsigned long VertexCount;                                           // +0x10
    unsigned short BonesPerVertex;                                       // +0x14
    bool PalettedFlag;                                                   // +0x16
    unsigned char GroupCount;                                            // +0x17
    unsigned char Groups[18];                                            // +0x18
#ifdef FABLE_RETAIL_CANIMATEDBLOCK_DATA
    class CDeviceIndependent : public CIVCountedPointeeBase {
    public:
        CArray<unsigned long> VertexList;                                // +0x08
        CArray<C3DIndexedBoneWeight> BoneWeights;                        // +0x14
    };
    CIVCountedPointer<CDeviceIndependent> Data;                          // +0x2C
private:
    void ConvertToVBFormatNonBump(CTVertexNormBlend4Tex1* out, N3DPrimitive::CDeviceIndependent& mesh);     // 0x00A8FF50
    void ConvertToVBFormatBump(CTVertexNormBlend4Tex1Bump* out, N3DPrimitive::CDeviceIndependent& mesh);    // 0x00A90040
public:
#else
    unsigned char Data[4];                                               // +0x2C CIVCountedPointer<CAnimatedBlock::CDeviceIndependent>
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(N3DPrimitive::CAnimatedBlock)==0x30);
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
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
    // Opt-in (chunk Q16, row 0x00A90260 CreateDegenerateTriangleIndexBuffer): Ego_r.pdb IndexBuffer is scoped_ptr<CIndexBuffer> (+0x34
    // retail; the retail reset swaps the new buffer in and deletes the old one through its virtual destructor, 0x00A90423..0x00A90437).
    // Needs FABLE_RETAIL_SCOPED_PTR (+ _RESET) and FABLE_RETAIL_CINDEXBUFFER.
    unsigned char ClothPrimitives[12],Data[4],VertexBuffer[4];
    boost::scoped_ptr<CIndexBuffer> IndexBuffer;
#elif defined(FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER)
    // Opt-in (row 0x00B8B780): Ego_r.pdb VertexBuffer is scoped_ptr<CVertexBufferWin32> (+0x30 retail). Needs FABLE_RETAIL_SCOPED_PTR.
    unsigned char ClothPrimitives[12],Data[4];
    boost::scoped_ptr<CVertexBufferWin32> VertexBuffer;
    unsigned char IndexBuffer[4];
#else
    unsigned char ClothPrimitives[12],Data[4],VertexBuffer[4],IndexBuffer[4];
#endif
    unsigned long VertexBufferStatus,IndexBufferStatus,StaticBlockCount;
    N3DPrimitive::CStaticBlock* StaticBlocks;
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
    unsigned long AnimatedBlockCount;N3DPrimitive::CAnimatedBlock* AnimatedBlocks;
    N3DPrimitive::CAnimatedBlock* GetAnimatedBlock(unsigned long index){return &AnimatedBlocks[index];}
    unsigned long GetAnimatedBlockCount()const{return AnimatedBlockCount;}
    unsigned long GetVertexCount()const{return VertexCount;}
#else
    unsigned long AnimatedBlockCount;unsigned char AnimatedBlocks[4];
#endif
    unsigned long AnimatedBlocksStatus,RepeatedBlocksStatus,VertexCount,TriangleCount,IndexCount;
    bool UploadedFlag;unsigned char VertexStreamFlags;unsigned long InitFlags;
    C3DTextureStages RecordedTextureStages;void* RecordedConstantData;long RecordedFrame;
    unsigned char PartitionedMeshSourceIndexBuffer[12],PartitionTree[8],MirroredTextureVertexIndexList[12];
    N3DPrimitive::CStaticBlock* GetStaticBlock(unsigned long index){return &StaticBlocks[index];}
    unsigned long GetStaticBlockCount()const{return StaticBlockCount;}
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
private:
    bool CreateDegenerateTriangleIndexBuffer();                          // 0x00A90260 ?CreateDegenerateTriangleIndexBuffer@C3DPrimitive2@@AAE_NXZ
public:
#endif
#ifdef FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
    void SetRecordedFrame(long frame){RecordedFrame=frame;}
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DPrimitive2)==0xa0);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,StaticBlockCount)==0x40);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,StaticBlocks)==0x44);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,RecordedFrame)==0x7c);
#endif
