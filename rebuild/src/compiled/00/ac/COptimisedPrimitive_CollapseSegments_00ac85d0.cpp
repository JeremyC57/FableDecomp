// COptimisedPrimitive::CollapseSegments(float) -- mark every segment no longer than the threshold and, when any is marked, retire the faces that belong to a marked segment
// Leaf: CollapseSegments
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC85D0, 163 bytes (+ int3 padding; `ret 4`).
// Callees: CArray<unsigned char>::CArray(count, const unsigned char& fill, const allocator&) 0x009D2A40 (landed as CByteArray in
//   CCachedFile::CCachedFile 0x00A5B590), free 0x00BFEA14 (the byte array's inline destructor).
// Layout: retail COptimisedPrimitive (STLport 12-byte containers): Faces +0x1c (72-byte COptimisedFace: Exists +0x3c, Segment
//   +0x44), SegmentCount +0x40, SegmentSizes +0x44 (float array start).
// Shape: the marks are a byte array local filled with 0 (the fill byte a frame byte); `size <= threshold` (test ah,41h ; jp); the removed count is
//   unsigned (`jbe`); the face walk takes size() once as a signed count and names the face by reference.
//   The fill constructor is a same-TU noinline stand-in (retail's CArray<unsigned char> instantiation is out of line at
//   0x009D2A40). Its body sets the three pointers itself and hands the extern only the count and fill value, so the
//   array's address does not escape. VC7.1 then keeps m_first in ebp for the whole body, free included, and homes the
//   empty allocator temporary over the live threshold slot (`lea eax,[esp+20h]`), as retail does.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
struct SAllocator { SAllocator() { } };              // empty allocator passed by const reference (no zero-init of the temporary)
struct CByteArray {                                  // retail CArray<unsigned char>: {first, last, end}
    unsigned char* m_first;
    unsigned char* m_last;
    unsigned char* m_end;
    CByteArray(unsigned long count, const unsigned char& fill, const SAllocator& al = SAllocator());
    ~CByteArray() { if (m_first) free(m_first); }
    unsigned char& operator[](unsigned long n) { return m_first[n]; }
};
unsigned char* FillAlloc_009d2a40(unsigned long count, unsigned char fill);
#pragma auto_inline(off)
__declspec(noinline) CByteArray::CByteArray(unsigned long count, const unsigned char& fill, const SAllocator& al)   // 0x009D2A40 stand-in
{
    m_first = FillAlloc_009d2a40(count, fill);
    m_last = m_first + count;
    m_end = m_last;
}
#pragma auto_inline(on)
struct C3DVector { float X, Y, Z; };
template <class T> struct CArray {                   // retail CArray (STLport vector): first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) const { return m_first[n]; }
};
namespace QuadricOptimiserInternals {
struct COptimisedFace {                              // 72 bytes
    unsigned long VertexIndices[3];                  // +0x00
    unsigned long UniqueVertexIndices[3];            // +0x0c
    long          Edges[3];                          // +0x18
    C3DVector     UTangent;                          // +0x24
    C3DVector     VTangent;                          // +0x30
    bool          Exists;                            // +0x3c
    unsigned long OutputIndex;                       // +0x40
    long          Segment;                           // +0x44
};
}
using namespace QuadricOptimiserInternals;
class COptimisedPrimitive {
public:
    long                   MaterialIndex;            // +0x00
    unsigned char          UniqueVertices[0xc];      // +0x04
    unsigned char          Vertices[0xc];            // +0x10
    CArray<COptimisedFace> Faces;                    // +0x1c
    unsigned char          Edges[0xc];               // +0x28
    unsigned char          VertexPairContractions[0xc];   // +0x34
    long                   SegmentCount;             // +0x40
    CArray<float>          SegmentSizes;             // +0x44
    void CollapseSegments(float threshold);
};

void COptimisedPrimitive::CollapseSegments(float threshold)
{
    unsigned long removed = 0;
    CByteArray collapse(SegmentCount, 0);
    for (long i = 0; i < SegmentCount; ++i) {
        if (SegmentSizes[i] <= threshold) {
            collapse[i] = 1;
            ++removed;
        }
    }
    if (removed > 0) {
        long faceCount = Faces.size();
        for (long j = 0; j < faceCount; ++j) {
            COptimisedFace& face = Faces[j];
            if (collapse[face.Segment])
                face.Exists = false;
        }
    }
}