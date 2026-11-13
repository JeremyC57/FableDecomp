#ifndef FABLE_ENGINE_C3DPRIMITIVE2_H
#define FABLE_ENGINE_C3DPRIMITIVE2_H
#include "engine/C3DTextureStages.h"
#include "engine/N3DPrimitive_CStaticBlock.h"
#include <stddef.h>
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
// Opt-in (bwf1, row 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData): types BoundingSphere, ClothPrimitives, Data, VertexBuffer
// (Ego_r.pdb), adds N3DPrimitive::CDeviceIndependent (retail 0x50; Ego_r members) and the loader's members.
#include "boost/scoped_ptr.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DVertex2.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CVertexBufferWin32.h"
class C3DClothPrimitive;class C3DGroup2;class C3DMeshFilePrimitiveChunk;class C3DMeshFileTrianglesChunk;class C3DMeshFileXAnimatedPrimitiveBlockChunk;
class CScopedMemoryHandle;
namespace N3DPrimitive {
class CTriangleList;
class CDeviceIndependent : public CBaseClassNonCopyable {
public:
    CDeviceIndependent();                                                // 0x00A98100
    virtual ~CDeviceIndependent();
    void ResizeVertexList(unsigned long count) { Vertices.resize(count); }                       // FableWin ?ResizeVertexList@CDeviceIndependent@N3DPrimitive@@QAEXK@Z
    void SetVertex(unsigned long index, const C3DVertex2& vertex) { Vertices[index] = vertex; }  // ?SetVertex@..@@QAEXKABVC3DVertex2@@@Z
    void AddGroup(CCountedPointer<C3DGroup2>& group) { Groups.push_back(group); }                // ?AddGroup@..@@QAEXAAV?$CCountedPointer@VC3DGroup2@@@@@Z
    void ClearTriangleLists() { TriangleLists.clear(); }                                          // ?ClearTriangleLists@..@@QAEXXZ
    unsigned long GetTriangleListCount() const { return TriangleLists.size(); }                  // ?GetTriangleListCount@..@@QBEKXZ
    unsigned long GetTotalTriangleCount() const;                         // 0x00A8ECE0
    unsigned long GetTotalIndexCount() const;                            // 0x00A8ED30
    void AddTriangleList(const C3DMeshFileTrianglesChunk& chunk);        // 0x00A90900 (private in Ego_r)
    CArray<C3DVertex2> Vertices;                                         // +0x04
    CArray<CIVCountedPointer<CTriangleList> > TriangleLists;             // +0x10
    CArray<CCountedPointer<C3DGroup2> > Groups;                          // +0x1C
    C3DBoundingSphere BoundingSphere;                                    // +0x28
    CArray<C3DVector> UTangents;                                         // +0x38
    CArray<C3DVector> VTangents;                                         // +0x44
};
}
#endif
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
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
    void InitialiseFromMeshFileData(const C3DMeshFileXAnimatedPrimitiveBlockChunk& chunk, bool b);   // 0x00A90AE0
#endif
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
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
    long MaterialIndex,RepeatingMeshReps;C3DBoundingSphere BoundingSphere;float AverageTextureStretch;
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;          // +0x20
    boost::scoped_ptr<N3DPrimitive::CDeviceIndependent> Data;            // +0x2C
    boost::scoped_ptr<CVertexBufferWin32> VertexBuffer;                  // +0x30
    unsigned char IndexBuffer[4];                                        // +0x34
#else
    long MaterialIndex,RepeatingMeshReps;unsigned char BoundingSphere[16];float AverageTextureStretch;
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
    // Opt-in (chunk Q16, row 0x00A90260 CreateDegenerateTriangleIndexBuffer): Ego_r.pdb IndexBuffer is scoped_ptr<CIndexBuffer> (+0x34
    // retail; the retail reset swaps the new buffer in and deletes the old one through its virtual destructor, 0x00A90423..0x00A90437).
    // Needs FABLE_RETAIL_SCOPED_PTR (+ _RESET) and FABLE_RETAIL_CINDEXBUFFER.
    unsigned char ClothPrimitives[12],Data[4],VertexBuffer[4];
    boost::scoped_ptr<CIndexBuffer> IndexBuffer;
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
#ifdef FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
    void SetRecordedFrame(long frame){RecordedFrame=frame;}
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
    bool InitialiseFromMeshFileData(const C3DMeshFilePrimitiveChunk& chunk, bool b, unsigned long reps, unsigned long initFlags, CScopedMemoryHandle& memoryHandle);
    void FreeData();                                                     // 0x00A901E0
    static unsigned long FABLE_FASTCALL CalculateVertexSize(unsigned long flags, bool a, bool b);   // 0x00A910E0
private:
    bool SetupVertexBuffer(EVertexType type, unsigned long size, unsigned long count, unsigned long flags);   // 0x00A8EDA0
    void AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count);   // 0x00A90510
    void SetVertexBufferStatus(unsigned long status) { VertexBufferStatus |= status; }
    void SetupVertexStreamFlags(unsigned long flags, bool b)             // ?SetupVertexStreamFlags@C3DPrimitive2@@AAEXK_N@Z
    {
        VertexStreamFlags = 0;
        if (b)
            VertexStreamFlags = 1;
        if (flags & 2)
            VertexStreamFlags |= 2;
        if (flags & 0x10)
            VertexStreamFlags |= 4;
    }
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DPrimitive2)==0xa0);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,StaticBlockCount)==0x40);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,StaticBlocks)==0x44);
FABLE_STATIC_ASSERT(offsetof(C3DPrimitive2,RecordedFrame)==0x7c);
#endif
