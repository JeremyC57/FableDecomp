#ifndef FABLE_ENGINE_C3DMESHFILEXANIMATEDPRIMITIVEBLOCKCHUNK_H
#define FABLE_ENGINE_C3DMESHFILEXANIMATEDPRIMITIVEBLOCKCHUNK_H
// New header (chunk 725b): C3DMeshFileXAnimatedPrimitiveBlockChunk : CChunkedFileSuperChunk (rtti_bases.tsv), vtable
// 0x0129E404 (scalar deleting dtor 0x00AA7460). Retail sizeof 0x60 (`new` 0x60 in C3DMeshFileXLoader::C3DMeshFileXLoader
// 0x00AA5C80): GroupBlock +0x2C, BonesPerVertex +0x44 (left uninitialised), VertexList +0x48, BoneWeights +0x54 (Ego_r names;
// 0x78 there with Dinkumware containers). The default constructor ??0..@@QAE@XZ is inline there ("XAPB", the out-of-line
// CVertexGroupBlock ctor, two empty vectors); the out-of-line copy 0x00AA7480 is the same body.
// Needs FABLE_RETAIL_CCHUNKEDFILECHUNK and FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK.
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/CVertexGroupBlock.h"
#include "engine/CArray.h"
#include "engine/C3DIndexedBoneWeight.h"
class C3DMeshFileXAnimatedPrimitiveBlockChunk : public CChunkedFileSuperChunk {
public:
    C3DMeshFileXAnimatedPrimitiveBlockChunk() : CChunkedFileSuperChunk("XAPB") { }
    virtual ~C3DMeshFileXAnimatedPrimitiveBlockChunk();
    virtual const CCharString GetChunkDescription() const;
    virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
    CVertexGroupBlock GroupBlock;                   // +0x2C
    unsigned long BonesPerVertex;                   // +0x44
    CArray<unsigned long> VertexList;               // +0x48
    CArray<C3DIndexedBoneWeight> BoneWeights;       // +0x54
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "XAPB"; }   // FableWin ?GetStaticChunkType@C3DMeshFileXAnimatedPrimitiveBlockChunk@@SIPBDXZ
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXAnimatedPrimitiveBlockChunk) == 0x60);
#endif
