// N3DPrimitive::CDeviceIndependent::Sizeof() const -- memory footprint of a primitive's device-independent data: the vertex array (allocated capacity), the triangle-list pointer array with each list's own size, the bounding sphere and both tangent arrays
// Leaf: ?Sizeof@CDeviceIndependent@N3DPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90450, length 189 bytes (+ int3 padding), `ret`. No callees.
// Called by the landed C3DPrimitive2::Sizeof (0x00A90860) through its Data scoped pointer.
// Layout: retail N3DPrimitive::CDeviceIndependent = Ego_r.pdb with 12-byte CArrays {first, last, end}: vptr +0, Vertices
// (CArray<C3DVertex2>, 32-byte vertices) +4, TriangleLists (CArray<CIVCountedPointer<CTriangleList>>) +0x10, Groups
// +0x1c (not counted), BoundingSphere (16 bytes) +0x28, UTangents / VTangents (CArray<C3DVector>) +0x38 / +0x44.
// The triangle-list array part is the landed CTriangleListArray::Sizeof (0x00A972A0) inlined: CTriangleList is 0x3c
// bytes with a 6-byte-triangle CArray at +8 and a 16-bit index CArray at +0x14.
// Shape: POD arrays count their allocated capacity (`end - first`) plus their own 12 bytes; the pointer array counts
// its used slots at 4 bytes plus each list (`sizeof + capacities`, folded to `((c1 + 10) * 3 + c2) * 2`); one
// the vertex part is a named local computed first (kept in edi across the list loop, the list total spilled) and
// the return sums the rest (VC folds the constants 12 + 12 + 16 into the final `lea ... + 28h`).
struct C3DVertex2 { unsigned char _data[0x20]; };
struct C3DVector { float X, Y, Z; };
struct C3DBoundingSphere { float X, Y, Z, Radius; };
struct STriangle { unsigned short m_index[3]; };

template <class T> struct CArray {                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    long Capacity() const { return m_pEnd - m_pFirst; }
    long Size() const { return m_pLast - m_pFirst; }
    unsigned long Sizeof() const { return sizeof(*this) + Capacity() * sizeof(T); }
};

namespace N3DPrimitive {
struct CTriangleList {                             // 0x3c bytes
    void*                   __vftable;             // +0x00
    long                    m_refCount;            // +0x04
    CArray<STriangle>       m_triangles;           // +0x08
    CArray<unsigned short>  m_indices;             // +0x14
    unsigned char           _pad20[0x3c - 0x20];
    unsigned long Sizeof() const
    {
        return sizeof(*this) + m_triangles.Capacity() * sizeof(STriangle) + m_indices.Capacity() * sizeof(unsigned short);
    }
};
}

struct CTriangleListPointer {                      // CIVCountedPointer<N3DPrimitive::CTriangleList>
    N3DPrimitive::CTriangleList* m_pObject;
    unsigned long Sizeof() const { return m_pObject ? m_pObject->Sizeof() : sizeof(*this); }
};
struct CTriangleListArray : CArray<CTriangleListPointer> {
    __forceinline unsigned long Sizeof() const
    {
        unsigned long size = sizeof(*this) + Size() * sizeof(CTriangleListPointer);
        for (const CTriangleListPointer* it = m_pFirst; it != m_pLast; ++it)
            size += it->Sizeof();
        return size;
    }
};

namespace N3DPrimitive {
struct CDeviceIndependent {
    void*               __vftable;                 // +0x00
    CArray<C3DVertex2>  Vertices;                  // +0x04
    CTriangleListArray  TriangleLists;             // +0x10
    CArray<void*>       Groups;                    // +0x1c CArray<CCountedPointer<C3DGroup2>>
    C3DBoundingSphere   BoundingSphere;            // +0x28
    CArray<C3DVector>   UTangents;                 // +0x38
    CArray<C3DVector>   VTangents;                 // +0x44
    unsigned long Sizeof() const;
};
}

unsigned long N3DPrimitive::CDeviceIndependent::Sizeof() const
{
    unsigned long size = Vertices.Sizeof();
    unsigned long lists = TriangleLists.Sizeof();
    return lists + sizeof(BoundingSphere) + UTangents.Sizeof() + VTangents.Sizeof() + size;
}