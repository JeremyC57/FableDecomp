// CTriangleListArray::Sizeof() const -- memory footprint of an array of counted triangle-list pointers: the array itself, one pointer per slot, and each list's own Sizeof (4 for an empty slot)
// Leaf: Sizeof
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A972A0, length 102 bytes.
// Identity: the manifest's "CDeviceIndependent::Sizeof" does not fit the body (`this` is a 12-byte array {first, last, end}
// whose elements are pointers, not the PDB's 100-byte N3DPrimitive CDeviceIndependent with Vertices at +4); the elements
// are N3DPrimitive::CTriangleList-like records (CIVCountedPointer<CTriangleList> per Ego_r.pdb's TriangleLists member):
// vptr + count, then two retail CArrays (12 bytes: first/last/end) of 6-byte triangles (+8) and 16-bit indices (+0x14),
// sizeof 0x3c. All inlined, no callees.
// Shape: the per-list size is `sizeof(*this) + capacity1 * 6 + capacity2 * 2` (capacities are end - first: the allocated
// memory), which VC folds to `((cap1 + 10) * 3 + cap2) * 2`; the outer array counts its used slots (`last - first`) at 4
// bytes each plus its own 12; the loop walks the slots with the end cached (edi) and an aligned head.
struct STriangle {                              // 6 bytes: three 16-bit vertex indices
    unsigned short m_index[3];
};

template <class T> struct CArray {              // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    long Capacity() const { return m_pEnd - m_pFirst; }
    long Size() const { return m_pLast - m_pFirst; }
};

struct CTriangleList {                          // N3DPrimitive::CTriangleList, 0x3c bytes
    void*                   __vftable;          // +0x00
    long                    m_refCount;         // +0x04
    CArray<STriangle>       m_triangles;        // +0x08
    CArray<unsigned short>  m_indices;          // +0x14
    unsigned char           _pad20[0x3c - 0x20];
    unsigned long Sizeof() const
    {
        return sizeof(*this) + m_triangles.Capacity() * sizeof(STriangle) + m_indices.Capacity() * sizeof(unsigned short);
    }
};

struct CTriangleListPointer {                   // CIVCountedPointer<N3DPrimitive::CTriangleList>
    CTriangleList* m_pObject;
    unsigned long Sizeof() const { return m_pObject ? m_pObject->Sizeof() : sizeof(*this); }
};

struct CTriangleListArray : CArray<CTriangleListPointer> {
    unsigned long Sizeof() const;
};

unsigned long CTriangleListArray::Sizeof() const
{
    unsigned long size = sizeof(*this) + Size() * sizeof(CTriangleListPointer);
    for (const CTriangleListPointer* it = m_pFirst; it != m_pLast; ++it)
        size += it->Sizeof();
    return size;
}