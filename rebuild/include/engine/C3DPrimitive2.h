#ifndef FABLE_ENGINE_C3DPRIMITIVE2_H
#define FABLE_ENGINE_C3DPRIMITIVE2_H
#include "engine/C3DTextureStages.h"
#include "engine/N3DPrimitive_CStaticBlock.h"
#include <stddef.h>
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_CLOTHPRIMITIVES
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
class C3DClothPrimitive;
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#include "boost/scoped_ptr.h"
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
class CVertexBufferWin32;
#endif
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
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_CLOTHPRIMITIVES
    // Retail opt-in (wf8, row 0x00B81F30): Ego_r.pdb types ClothPrimitives (+0x20) as CArray<CCountedPointer<C3DClothPrimitive> >
    // (the retail 12-byte STLport vector). Not combinable with FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER yet. No layout change.
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;
    unsigned char Data[4],VertexBuffer[4],IndexBuffer[4];
#else
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
    // Opt-in (chunk Q16, row 0x00A90260 CreateDegenerateTriangleIndexBuffer): Ego_r.pdb IndexBuffer is scoped_ptr<CIndexBuffer> (+0x34
    // retail; the retail reset swaps the new buffer in and deletes the old one through its virtual destructor, 0x00A90423..0x00A90437).
    // Needs FABLE_RETAIL_SCOPED_PTR (+ _RESET) and FABLE_RETAIL_CINDEXBUFFER.
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
    // Opt-in (wf8, row 0x00B903D0): Ego_r.pdb VertexBuffer is scoped_ptr<CVertexBufferWin32> (+0x30 retail). No layout change.
    unsigned char ClothPrimitives[12],Data[4];
    boost::scoped_ptr<CVertexBufferWin32> VertexBuffer;
    boost::scoped_ptr<CIndexBuffer> IndexBuffer;
#elif defined(FABLE_RETAIL_C3DPRIMITIVE2_DATA)
    // Opt-in (wf19, row 0x00A8F9E0 UploadToIndexBuffer): Ego_r.pdb Data is boost::scoped_ptr<N3DPrimitive::CDeviceIndependent>
    // (+0x2C retail; the includer defines CDeviceIndependent). No layout change.
    unsigned char ClothPrimitives[12];
    boost::scoped_ptr<N3DPrimitive::CDeviceIndependent> Data;
    unsigned char VertexBuffer[4];
    boost::scoped_ptr<CIndexBuffer> IndexBuffer;
#else
    unsigned char ClothPrimitives[12],Data[4],VertexBuffer[4];
    boost::scoped_ptr<CIndexBuffer> IndexBuffer;
#endif
#else
    unsigned char ClothPrimitives[12],Data[4],VertexBuffer[4],IndexBuffer[4];
#endif
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
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_UPLOADTOINDEXBUFFER
    // Opt-in (wf19, row 0x00A8F9E0): Ego_r/FableWin ?UploadToIndexBuffer@C3DPrimitive2@@AAE_NK@Z (private) and the inline helpers
    // it expands: ?InitIndexBuffer@C3DPrimitive2@@AAE_NAAVCIndexBuffer@@KK@Z (Ego_r 0x00BD0F20, no retail out-of-line copy),
    // ?SetIndexBufferStatus@C3DPrimitive2@@AAEXW4EBufferStatus@1@@Z, GetRepeatingMeshBatchSize, ?GetBaseBlock@C3DPrimitive2@@QAEPAVCBaseBlock@N3DPrimitive@@K@Z.
    // Needs FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS and the includer's EIndexBufferFormat (IndexFormat16) / EIndexBufferPool (IndexPoolManaged). No layout change.
    enum EBufferStatus { };                                              // PDB type name only; enumerator names unknown
    long GetRepeatingMeshBatchSize() { return RepeatingMeshReps > 1 ? RepeatingMeshReps : 1; }
    N3DPrimitive::CBaseBlock* GetBaseBlock(unsigned long index) {
        if (AnimatedBlockCount) return &AnimatedBlocks[index];
        return &StaticBlocks[index];
    }
private:
    void SetIndexBufferStatus(EBufferStatus status) { IndexBufferStatus |= status; }
    bool InitIndexBuffer(CIndexBuffer& buffer, unsigned long length, unsigned long flags) {
        unsigned long usage = 8;
        EIndexBufferPool pool = IndexPoolManaged;
        if (flags & 1) { usage = 0x18; pool = (EIndexBufferPool)2; }
        buffer.Initialise(length, usage, IndexFormat16, pool, PT_TRIANGLE_LIST);
        return true;
    }
    bool UploadToIndexBuffer(unsigned long flags);                       // 0x00A8F9E0
public:
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_UPLOADBUFFERS
    // wf22 (row 0x00A24520 C3DMeshLODInfo::BuildMesh): Ego_r.pdb ?UploadAnimatedBuffers@C3DPrimitive2@@QAE_NK@Z (0x00A92870),
    // ?UploadRepeatedBuffers@C3DPrimitive2@@QAE_NKK@Z (0x00A932C0), ?UploadStaticBuffers@C3DPrimitive2@@QAE_NK@Z (0x00A93320), landed.
    bool UploadAnimatedBuffers(unsigned long);                           // 0x00A92870
    bool UploadRepeatedBuffers(unsigned long, unsigned long);            // 0x00A932C0
    bool UploadStaticBuffers(unsigned long);                             // 0x00A93320
#endif
#ifdef FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
    void SetRecordedFrame(long frame){RecordedFrame=frame;}
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_RECORDEDSTATE
    // Retail opt-in (wf8, row 0x00BA4A40): FableWin.pdb ?GetRecordedFrame@C3DPrimitive2@@QBEJXZ,
    // ?PeekRecordedTextureStages@C3DPrimitive2@@QBE..., ?SetRecordedTextureStages@C3DPrimitive2@@QAEXABVC3DTextureStages@@@Z
    // (inline in retail: +0x7C / +0x6C read and written in place). No layout change.
    long GetRecordedFrame() const { return RecordedFrame; }
    const C3DTextureStages& PeekRecordedTextureStages() const { return RecordedTextureStages; }
    void SetRecordedTextureStages(const C3DTextureStages& stages) { RecordedTextureStages=stages; }
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DPrimitive2)==0xa0);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,StaticBlockCount)==0x40);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,StaticBlocks)==0x44);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,RecordedFrame)==0x7c);
#endif
