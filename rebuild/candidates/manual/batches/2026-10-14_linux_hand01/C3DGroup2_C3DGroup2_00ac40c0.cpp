// C3DGroup2::C3DGroup2() -- BoneIndex = -1 and an empty VertexBlends CArray (data, count, capacity zeroed)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC40C0, length 20 bytes. No callees.
// PDB layout: C3DGroup2 = {long BoneIndex +0, CArray<C3DVertexBlend2> VertexBlends +4}; the
// retail CArray is 12 bytes ({data, count, capacity}), so the object is 16 bytes.
struct C3DVertexBlend2;

struct CArrayVertexBlend2 {                // retail CArray<C3DVertexBlend2>
    C3DVertexBlend2* m_pData;
    unsigned long m_count;
    unsigned long m_capacity;
    CArrayVertexBlend2() : m_pData(0), m_count(0), m_capacity(0) { }
};

struct C3DGroup2 {
    long BoneIndex;                        // +0
    CArrayVertexBlend2 VertexBlends;       // +4
    C3DGroup2();
};

C3DGroup2::C3DGroup2()
    : BoneIndex(-1)
{
}
