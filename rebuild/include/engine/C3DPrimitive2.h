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
#ifdef FABLE_RETAIL_CDEVICEINDEPENDENT_ADDTRIANGLELIST_PRIVATE
class C3DPrimitive2;   // NF314 opt-in: forward declaration for the friend below
#endif
namespace N3DPrimitive {
class CTriangleList;
class CDeviceIndependent : public CBaseClassNonCopyable {
#ifdef FABLE_RETAIL_GROUP_REMAP_PASS29
private:
 void RemapBoneIndices(std::map<unsigned long,unsigned long>& index_map);
#endif
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
#ifdef FABLE_RETAIL_CDEVICEINDEPENDENT_ADDTRIANGLELIST_PRIVATE
    // NF314 opt-in (row 0x00A91640): the PDB access. Ego_r.pdb and FableWin.pdb both spell ?AddTriangleList@CDeviceIndependent@N3DPrimitive@@AAEX
    // ABVC3DMeshFileTrianglesChunk@@@Z (private); its only retail caller is C3DPrimitive2::InitialiseFromMeshFileData (0x00A919D6 / 0x00A9202A),
    // so C3DPrimitive2 is a friend of the class.
    friend class ::C3DPrimitive2;
private:
    void AddTriangleList(const C3DMeshFileTrianglesChunk& chunk);        // 0x00A90900 (private in Ego_r and FableWin)
public:
#else
    void AddTriangleList(const C3DMeshFileTrianglesChunk& chunk);        // 0x00A90900 (private in Ego_r)
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_CONVERTSTATICBUMP
    // Overlay opt-in (hand4d, row 0x00A8F1B0 ConvertToVBFormatStaticBump): FableWin.pdb ?GetVertexCount@CDeviceIndependent@N3DPrimitive@@QBEKXZ
    // (0x032F8CB0 debug; inline in retail: the row reads the Vertices words in place).
    unsigned long GetVertexCount() const { return Vertices.size(); }
#endif
    CArray<C3DVertex2> Vertices;                                         // +0x04
    CArray<CIVCountedPointer<CTriangleList> > TriangleLists;             // +0x10
    CArray<CCountedPointer<C3DGroup2> > Groups;                          // +0x1C
    C3DBoundingSphere BoundingSphere;                                    // +0x28
    CArray<C3DVector> UTangents;                                         // +0x38
    CArray<C3DVector> VTangents;                                         // +0x44
};
}
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#include "engine/C3DBoundingSphere.h"
#endif
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
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
    void InitialiseFromMeshFileData(const C3DMeshFileXAnimatedPrimitiveBlockChunk& chunk, bool b);   // 0x00A90AE0
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADBINARY_PASS27
    // FableWin 032FEDD0 calls only the CBaseBlock and Data constructors.
    CAnimatedBlock() { }
    void LoadBinary(CMemoryDataInputStream& is);                           // 00A8EB10
#endif
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
FABLE_STATIC_ASSERT_AT(68, sizeof(N3DPrimitive::CAnimatedBlock)==0x30);
#endif
// Full original member range: donor172 becomes native160 after three16-byte CArrays shrink to12.
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
#include "engine/CMovableResource.h"
class C3DPrimitive2 : public CMovableResource {
#else
class C3DPrimitive2 {
#endif
public:
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP
    // Opt-in (bwf2, row 0x00BA2350): FableWin.pdb ?IsDP3Bump@C3DPrimitive2@@QBE_NXZ (13 debug instructions), inline: InitFlags bit 1.
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP_MASK
    bool IsDP3Bump()const{return (InitFlags&2)!=0;}
#else
    bool IsDP3Bump()const{return (InitFlags>>1)&1;}
#endif
#endif
#ifdef FABLE_RETAIL_C3DMESH2_LOADBINARY
 // Retail opt-in (Codex pass22): both-PDB exact public LoadBinary signature and native caller.
 bool LoadBinary(class CMemoryDataInputStream&,unsigned long,unsigned char**,class CMovableResourceMemoryPool*);
#endif
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
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADBINARY_PASS27
    boost::scoped_ptr<CIndexBuffer> IndexBuffer;                           // +0x34
#else
    unsigned char IndexBuffer[4];                                        // +0x34
#endif
#else
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_RENDER
    // Opt-in (chunk 2060 row 0x00BA5260; re-based by bwf1 / agentC2 row 0x00BB3E80): BoundingSphere typed as Ego_r.pdb declares it
    // (C3DBoundingSphere +0xC, same storage) and the FableWin.pdb inline accessors ?GetMaterialIndex@C3DPrimitive2@@QBEJXZ,
    // ?GetBoundingSphere@C3DPrimitive2@@QBE?AVC3DBoundingSphere@@XZ (by value), ?GetAverageTextureStretch@C3DPrimitive2@@QBEMXZ.
    long GetMaterialIndex()const{return MaterialIndex;}
    C3DBoundingSphere GetBoundingSphere()const{return BoundingSphere;}
    float GetAverageTextureStretch()const{return AverageTextureStretch;}
    long MaterialIndex,RepeatingMeshReps;C3DBoundingSphere BoundingSphere;float AverageTextureStretch;
#else
    long MaterialIndex,RepeatingMeshReps;unsigned char BoundingSphere[16];float AverageTextureStretch;
#endif
#if defined(FABLE_RETAIL_C3DPRIMITIVE2_GETVERTEXBUFFER) && defined(FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER)
    // Opt-in (agentC13, row 0x00BA5260 RenderMeshPrimitive; chunk 2060's text): FableWin.pdb inline accessor
    // ?GetVertexBuffer@C3DPrimitive2@@QAEPAVCVertexBufferWin32@@XZ = VertexBuffer.get() (needs _VERTEXBUFFER for the typed member).
    CVertexBufferWin32* GetVertexBuffer(){return VertexBuffer.get();}
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_CLOTHPRIMITIVES
    // Retail opt-in (wf8, row 0x00B81F30): Ego_r.pdb types ClothPrimitives (+0x20) as CArray<CCountedPointer<C3DClothPrimitive> >
    // (the retail 12-byte STLport vector). Not combinable with FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER yet. No layout change.
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;
    unsigned char Data[4],VertexBuffer[4],IndexBuffer[4];
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_GETCLOTHPRIMITIVE
    // Nested opt-in (agentNF41, row 0x00BA77B0): FableWin.pdb ?GetClothPrimitive@C3DPrimitive2@@QBEPAVC3DClothPrimitive@@K@Z (debug only:
    // inline in release, the element's counted-pointer Data read in place). No layout change.
    C3DClothPrimitive* GetClothPrimitive(unsigned long index) const { return ClothPrimitives[index].Get(); }
#endif
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

#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Original Ego_r/FableWin TPI and module916 call sites; private evidence in00ba115c.
#ifndef FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH
 long GetMaterialIndex()const{return MaterialIndex;}
#endif
 bool HasCloth()const{return !ClothPrimitives.empty();}
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH
    // Retail opt-in (chunk 2059, row 0x00BC25F0 CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive): the FableWin.pdb inline
    // accessors the row uses (no out-of-line copy in Ego_r.pdb or Fable.exe; FableWin debug sizes: GetMaterialIndex 17b/9i,
    // GetRepeatingMeshBatchSize 17b/9i, GetVertexCount 17b/9i, GetTriangleCount 17b/9i, GetVertexBuffer 22b/10i, GetIndexBuffer 345b/94i =
    // assert code; retail reads +0x04 / +0x08 / +0x58 / +0x5C / +0x30 / +0x34 in place, 0x00BC2CCC..0x00BC2F50). Needs the typed buffers
    // of FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER + FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER.
    long GetMaterialIndex() const { return MaterialIndex; }

    long GetRepeatingMeshBatchSize() { return RepeatingMeshReps; }
#ifndef FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
    unsigned long GetVertexCount() const { return VertexCount; }
#endif
    unsigned long GetTriangleCount() const { return TriangleCount; }
    CVertexBufferWin32* GetVertexBuffer() { return VertexBuffer.get(); }
    CIndexBuffer* GetIndexBuffer() { return IndexBuffer.get(); }
#endif
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
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_CONVERTSTATICBUMP
    // Overlay opt-in (hand4d): Ego_r.pdb / FableWin.pdb ?ConvertToVBFormatStaticBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1Bump@@@Z
    // (private; FableWin 0x032F9EF0), out of line in retail at 0x00A8F1B0. Needs FABLE_RETAIL_C3DPRIMITIVE2_LOADER (the typed Data).
private:
    void ConvertToVBFormatStaticBump(class CTVertexNormTex1Bump* dest);  // 0x00A8F1B0
public:
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
    bool InitialiseFromMeshFileData(const C3DMeshFilePrimitiveChunk& chunk, bool b, unsigned long reps, unsigned long initFlags, CScopedMemoryHandle& memoryHandle);
    void FreeData();                                                     // 0x00A901E0
    static unsigned long FABLE_FASTCALL CalculateVertexSize(unsigned long flags, bool a, bool b);   // 0x00A910E0
private:
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADBINARY_PASS27
public:
    // FableWin 032F8530 is the unsigned block count test.
    bool IsAnimated() const { return AnimatedBlockCount > 0; }
private:
    void LoadVertexBuffer(CMemoryDataInputStream&, unsigned long, unsigned long,
                          unsigned long, class CVertexCompressionParams&); // 00A91560
#endif
    bool SetupVertexBuffer(EVertexType type, unsigned long size, unsigned long count, unsigned long flags);   // 0x00A8EDA0
    void AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count);   // 0x00A90510
    void SetVertexBufferStatus(unsigned long status) { VertexBufferStatus |= status; }
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_STREAMFLAGS_MEMBER
    // Retail opt-in (row 0x00A91640): the expansion at 0x00A92578..0x00A925A1 tests the InitFlags MEMBER, re-read after the byte
    // store (`mov cl,[edi+68h] ; mov al,2 ; test al,cl ; ... ; or [edi+65h],al ; test byte ptr [edi+68h],10h`), not the by-value
    // copy the caller passed. Same signature (Ego_r ?SetupVertexStreamFlags@C3DPrimitive2@@AAEXK_N@Z). fwsize: 32i/89b v FableWin
    // 35i/98b (the default body below is 30i/83b; neither equal: FableWin's text still unknown).
    void SetupVertexStreamFlags(unsigned long flags, bool b)
    {
        VertexStreamFlags = 0;
        if (b)
            VertexStreamFlags = 1;
        if (InitFlags & 2)
            VertexStreamFlags |= 2;
        if (InitFlags & 0x10)
            VertexStreamFlags |= 4;
    }
#else
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
#endif
public:
#endif
};
FABLE_STATIC_ASSERT_AT(193, sizeof(C3DPrimitive2)==0xa0);
FABLE_STATIC_ASSERT_AT(194, offsetof(C3DPrimitive2,StaticBlockCount)==0x40);
FABLE_STATIC_ASSERT_AT(195, offsetof(C3DPrimitive2,StaticBlocks)==0x44);
FABLE_STATIC_ASSERT_AT(196, offsetof(C3DPrimitive2,RecordedFrame)==0x7c);
#endif
