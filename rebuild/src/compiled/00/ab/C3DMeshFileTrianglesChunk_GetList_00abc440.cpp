// C3DMeshFileTrianglesChunk::GetList() const -- the triangle list (+0x2C, 6-byte C3DTriangle2 records) viewed as a C array: its first element when it holds any, else the container object itself
// Leaf: GetList
// Retail: Fable.exe @ 0x00ABC440, 35 bytes (+13 int3), `__fastcall`, plain `ret`, no callees.
// Label correction: the manifest's propagated "CWaterPatchTesselatorBase<CTVertexSeaBackground>::GetBridgeVertexBufferAsCArray"
// cannot be right -- the size divide is by 6 (0x2AAAAAAB multiply, NO extra shift), the sea vertex is 12 bytes. The row
// sits among the C3DMeshFileTrianglesChunk methods (0xABBE90..0xABEC90), and its two callers are
// CPhysicsMesh::GetMeshDataFromFileData (0x00A6CDC5) and N3DPrimitive::CDeviceIndependent::AddTriangleList(const
// C3DMeshFileTrianglesChunk&) (0x00A90952); Ego_r.pdb has ?GetList@C3DMeshFileTrianglesChunk@@QBEPBVC3DTriangle2@@XZ
// (and a byte-identical non-const twin, folded here) over the member TriangleList (CArray<C3DTriangle2>).
// Layout: engine/C3DMeshFileTrianglesChunk.h is the Ego_r donor (TriangleList at +0x34 with 16-byte containers); retail's
// 12-byte containers put it at +0x2C, so this is a local model (vptr + 0x28 bytes of chunk-base members, then the list).
// Shape: same as the landed CLandscapePatchTesselator twin 0x00B6C9B0: the container's address in esi, `size()` as the
// signed pointer difference (the divide survives the `!= 0` test), `first` loaded before the branch.
class C3DTriangle2 { public:
    unsigned short VertexIndex[3];
};

template <class T> class CArray { public:                 // retail STLport-shaped vector: first / last / end of storage
    T* m_first;
    T* m_last;
    T* m_end;
    long size() const { return m_last - m_first; }
};

class C3DMeshFileTrianglesChunk { public:
    void*                 __vftable;               // +0x00
    unsigned char         _chunkBase[0x28];        // +0x04 CChunkedFileChunk members
    CArray<C3DTriangle2>  TriangleList;            // +0x2C
    const C3DTriangle2* GetList() const;
};

const C3DTriangle2* C3DMeshFileTrianglesChunk::GetList() const
{
    const CArray<C3DTriangle2>* list = &TriangleList;
    return list->size() != 0 ? list->m_first : (const C3DTriangle2*)list;
}