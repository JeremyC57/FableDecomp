// Behaviour test for N3DPrimitive::CDeviceIndependent::Sizeof (0x00A90450): sums allocated POD array capacities
// (+12 each), the used triangle-list slots (4 each, plus each list's 60 + capacities), and the 16-byte sphere.
#include <cstdio>
class C3DVertex2 { public: unsigned char _data[0x20]; };
class C3DVector { public: float X, Y, Z; };
class C3DBoundingSphere { public: float X, Y, Z, Radius; };
struct STriangle { unsigned short m_index[3]; };

template <class T> class CArray { public:                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    long Capacity() const { return m_pEnd - m_pFirst; }
    long Size() const { return m_pLast - m_pFirst; }
    unsigned long Sizeof() const { return sizeof(*this) + Capacity() * sizeof(T); }
};

namespace N3DPrimitive {
class CTriangleList { public:                             // 0x3c bytes
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
    unsigned long Sizeof() const
    {
        unsigned long size = sizeof(*this) + Size() * sizeof(CTriangleListPointer);
        for (const CTriangleListPointer* it = m_pFirst; it != m_pLast; ++it)
            size += it->Sizeof();
        return size;
    }
};

namespace N3DPrimitive {
class CDeviceIndependent { public:
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

int main()
{
    using namespace N3DPrimitive;
    C3DVertex2 verts[5]; C3DVector ut[4], vt[3]; STriangle tris[7]; unsigned short idx[9];
    CTriangleList list;
    list.m_triangles.m_pFirst = tris; list.m_triangles.m_pLast = tris + 2; list.m_triangles.m_pEnd = tris + 7;
    list.m_indices.m_pFirst = idx; list.m_indices.m_pLast = idx + 1; list.m_indices.m_pEnd = idx + 9;
    CTriangleListPointer slots[3]; slots[0].m_pObject = &list; slots[1].m_pObject = 0; slots[2].m_pObject = &list;
    CDeviceIndependent d;
    d.Vertices.m_pFirst = verts; d.Vertices.m_pLast = verts + 2; d.Vertices.m_pEnd = verts + 5;
    d.TriangleLists.m_pFirst = slots; d.TriangleLists.m_pLast = slots + 3; d.TriangleLists.m_pEnd = slots + 3;
    d.UTangents.m_pFirst = ut; d.UTangents.m_pLast = ut + 1; d.UTangents.m_pEnd = ut + 4;
    d.VTangents.m_pFirst = vt; d.VTangents.m_pLast = vt; d.VTangents.m_pEnd = vt + 3;
    unsigned long listSize = sizeof(CTriangleList) + 7 * 6 + 9 * 2;
    const unsigned long A = sizeof(CArray<C3DVector>);   // 12 on the 32-bit target
    unsigned long expected = (A + 5 * sizeof(C3DVertex2)) + (A + 3 * sizeof(CTriangleListPointer) + listSize + sizeof(CTriangleListPointer) + listSize)
        + 16 + (A + 4 * 12) + (A + 3 * 12);
    if (sizeof(CTriangleList) != 0x3c && sizeof(void*) == 4) { std::printf("00a90450_TEST FAIL\n"); return 1; }
    if (d.Sizeof() != expected) { std::printf("00a90450_TEST FAIL\n"); return 1; }
    std::printf("00a90450_TEST PASS\n");
    return 0;
}