// NShape::CShape::UpdateDisplacePoints(const C3DVector& delta) -- adds delta to every selected point (the selection is a vector of point pointers at +0x14) and reports whether any point moved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00761200, length 61 bytes.
// Leaf: UpdateDisplacePoints
// Layout as the landed NShape::CShape destructor 0x00761BD0: vptr, owner +4, points vector +8, selected-point vector +0x14
// (retail VC6 vectors are 12 bytes; the PDB's sit 4 bytes high).
// Shape: `xor al,al` before the empty test, `mov al,1` re-done at the head of every iteration, the end pointer
// re-read from the member after the three x87 adds (the stores may alias it), `ret 4`.
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector& operator+=(const C3DVector& other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        return *this;
    }
};

template <class T> struct CRetailVector {       // VC6-era vector: first, last, end
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
};

class CTCShapeManager;

namespace NShape {
class CShape { public:                                 // vtable 0x0126A208
    virtual void v00();
    CTCShapeManager*          Owner;            // +0x04
    CRetailVector<C3DVector>  Points;           // +0x08
    CRetailVector<C3DVector*> SelectedPoints;   // +0x14
    bool UpdateDisplacePoints(const C3DVector& delta);
};
}

bool NShape::CShape::UpdateDisplacePoints(const C3DVector& delta)
{
    bool moved = false;
    for (C3DVector** it = SelectedPoints.begin(); it != SelectedPoints.end(); ++it) {
        moved = true;
        **it += delta;
    }
    return moved;
}