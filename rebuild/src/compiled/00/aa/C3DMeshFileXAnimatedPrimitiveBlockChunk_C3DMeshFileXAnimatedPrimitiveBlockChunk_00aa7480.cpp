// C3DMeshFileXAnimatedPrimitiveBlockChunk::~C3DMeshFileXAnimatedPrimitiveBlockChunk() -- free the bone-weight and vertex-list arrays, destroy the vertex group block, then tail-jump to the CChunkedFileSuperChunk destructor
// Leaf: ??1C3DMeshFileXAnimatedPrimitiveBlockChunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7480, length 51 bytes.
// Callees: free @ 0x00BFEA14 twice (the inline CArray destructors), CVertexGroupBlock::~CVertexGroupBlock @ 0x00A96DA0
// (labelled "CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures", propagated), CChunkedFileSuperChunk's destructor
// @ 0x00AA8490 (stores vtable 0x0129E4F0; labelled "~CCompressedMemoryDataInputStream", propagated; tail jump).
// Evidence: the only caller is the scalar deleting destructor @ 0x00AA7460, slot 0 of the vtable 0x0129E404 of
// C3DMeshFileXAnimatedPrimitiveBlockChunk (rtti-vtables.tsv). The catalog label "~CPatchTesselationEdgeStrip" is propagated.
// Layout: engine/CChunkedFileSuperChunk.h is the PDB base layout with 16-byte CArrays and the vptr as a data word, so it
// cannot be the polymorphic base here (as in the landed 0x00AA8490 destructor). Ego_r.pdb with the retail 12-byte CArray (each CArray 4 bytes narrower): the CChunkedFileSuperChunk base is
// 0x2c bytes (two CArrays), GroupBlock +0x2c, BonesPerVertex +0x44, VertexList +0x48, BoneWeights +0x54. No engine header.
// Shape: members destroyed in reverse order, each array `test ; je ; push ; call free ; add esp,4`; no vtable store of its
// own (the class is `__declspec(novtable)`, as for an abstract-looking leaf whose own store would sit before the members);
// the base destructor is the tail jump.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

template <class T>
class CArray { public:                                    // retail CArray<T>: first / last / end (0x0c bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    ~CArray() { if (m_pFirst) free(m_pFirst); }
};

class CChunkedFileSuperChunk { public:                    // vtable 0x0129E4F0, retail 0x2c bytes
    virtual void v00();
    unsigned char _body[0x28];                     // +0x04 chunk header, SubChunks, RegisteredSubChunkTypes, CurrentChunkIndex
    ~CChunkedFileSuperChunk();                     // 0x00AA8490
};

class CVertexGroupBlock { public:                         // retail 0x18 bytes
    unsigned char _body[0x18];
    ~CVertexGroupBlock();                          // 0x00A96DA0
};

class C3DIndexedBoneWeight;

struct __declspec(novtable) C3DMeshFileXAnimatedPrimitiveBlockChunk : CChunkedFileSuperChunk {   // vtable 0x0129E404
    CVertexGroupBlock              GroupBlock;     // +0x2c
    unsigned long                  BonesPerVertex; // +0x44
    CArray<unsigned long>          VertexList;     // +0x48
    CArray<C3DIndexedBoneWeight>   BoneWeights;    // +0x54
    virtual void v00();
    ~C3DMeshFileXAnimatedPrimitiveBlockChunk();
};

C3DMeshFileXAnimatedPrimitiveBlockChunk::~C3DMeshFileXAnimatedPrimitiveBlockChunk()
{
}