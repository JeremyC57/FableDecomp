// NShape::CBasicShape::IsPosInShape(const C3DVector& pos) -- true when any subdivision triangle contains the position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007611C0, length 55 bytes (+ int3 padding).
// Callees: NShape::CSubdivisionTriangle::IsPosInShape (virtual slot 4, +0x10) on each element.
// Vtable 0x0126A31C (rtti-vtables.tsv: NShape::CBasicShape, slot 4 = this function).
// Shape: the member at +0x20 is a retail (VC6 Dinkumware, 12-byte) vector of 8-byte
// CCountedPointer<CSubdivisionTriangle> {object, count block}; the loop walks first..last and
// re-reads `last` after each virtual call (the callee may touch the container). The result is
// `unsigned char` (retail `xor al,al` / `mov al,1`, no `xor eax,eax`).
struct C3DVector { float X, Y, Z; };

namespace NShape {

struct CSubdivisionTriangle {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual unsigned char IsPosInShape(const C3DVector& pos);   // slot 4
};

template <class T>
struct CCountedPointer {                        // 8 bytes: pointee + shared count block
    T*    m_pObject;
    void* m_pCount;
};

template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
};

struct CBasicShape {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual unsigned char IsPosInShape(const C3DVector& pos);   // slot 4
    unsigned char _pad[0x1c];                                   // +0x04..+0x1f (CShape body, points)
    CRetailVector< CCountedPointer<CSubdivisionTriangle> > Subdivisions;   // +0x20
};

unsigned char CBasicShape::IsPosInShape(const C3DVector& pos)
{
    for (CCountedPointer<CSubdivisionTriangle>* p = Subdivisions.first; p != Subdivisions.last; ++p) {
        if (p->m_pObject->IsPosInShape(pos))
            return true;
    }
    return false;
}

}