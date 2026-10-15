// Grade: DIFFER(123v125) -- the null arm of `new` stores the zero register (`mov [ebx+ebp*4+20h],edi`) where retail materialises `xor eax,eax ; mov [..],eax`; everything else matches
// C3DMeshPartitionElement::BuildSubElement(long index) -- destroys the existing sub-element at SubElements[index] (if any) and replaces it with a fresh, empty element
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC2160, length 125 bytes.
// Callees: 0x00AC37B0 C3DMeshPartitionElement scalar deleting destructor (for the grandchildren), 0x00BFE9BC
// operator delete, 0x00BFEA1A operator new(0x38).
// Layout (PDB, 56 bytes, no vtable; as the landed 0x00AC37B0 / 0x00AC2100): BoundingBox +0, StartTriangle +0x18,
// TriangleCount +0x1c, SubElements[2] +0x20, BoundingSphere +0x28.
// Shape: the old element's destructor is inlined (`p->~T(); operator delete(p);` with a `__forceinline`
// destructor -- a plain `delete p` calls the out-of-line ??_G); inside it the two grandchildren are plain
// `delete`s (null test + `push 1 ; call ??_G`). The new element's inline constructor zeroes the sphere first
// (its own constructor, dwords from the zero register), then the sub-element pointers and the triangle range
// (body assignments); the result is stored in both arms of the `new` null test.
struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector Min, Max; };
struct C3DBoundingSphere {                          // centre + radius, zeroed as dwords (landed 0x00A98100 model)
    long m_x, m_y, m_z, m_radiusBits;
    C3DBoundingSphere() : m_x(0), m_y(0), m_z(0), m_radiusBits(0) { }
};

struct C3DMeshPartitionElement {
    C3DBoundingBox           BoundingBox;      // +0x00
    long                     StartTriangle;    // +0x18
    long                     TriangleCount;    // +0x1C
    C3DMeshPartitionElement* SubElements[2];   // +0x20
    C3DBoundingSphere        BoundingSphere;   // +0x28

    C3DMeshPartitionElement()
    {
        SubElements[0] = 0;
        SubElements[1] = 0;
        StartTriangle = 0;
        TriangleCount = 0;
    }
    __forceinline ~C3DMeshPartitionElement()
    {
        delete SubElements[0];
        delete SubElements[1];
        StartTriangle = 0;
        TriangleCount = 0;
    }
    void BuildSubElement(long index);
};

void C3DMeshPartitionElement::BuildSubElement(long index)
{
    C3DMeshPartitionElement* old = SubElements[index];
    if (old) {
        old->~C3DMeshPartitionElement();
        operator delete(old);
    }
    SubElements[index] = new C3DMeshPartitionElement;
}
