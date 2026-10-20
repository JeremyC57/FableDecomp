// COptimisedPrimitive::DetectFoldOver(CVertexPairContraction*) -- true as soon as any sub-contraction of the vertex-pair contraction would fold a face over when its vertices move to the resulting position
// Tried: CArray operator[], `m_pData + i`, a signed `long` Size() -- all give the same colouring.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ACD900, length 83 bytes.
// Callees: COptimisedPrimitive::DetectFoldOver(CSubContraction*, C3DVector*) @ 0x00ACD0E0 (the per-pair test; manifest-typed).
// Layout: Ego_r.pdb QuadricOptimiserInternals::CVertexPairContraction (Exists +0, Unsplit +1,
// CArray<CSubContraction> SubContractions +4, Weight2, Cost, C3DVector ResultingPosition +0x1c,
// ListIterator) with the retail 12-byte CArray: ResultingPosition at +0x18 (`lea ebx,[esi+18h]`).
// CSubContraction is two vertex indices (8 bytes, `sar 3`, `[eax+edi*8]`). The engine header
// COptimisedPrimitive.h is not needed: `this` is only forwarded to the per-pair test.
// Shape: /O2 loop (aligned head), the invariant position address hoisted, the element address and
// the unsigned element count re-derived from the array every iteration; the shared returns put
// `xor al,al` (loop exhausted) before `mov al,1` (found).
// Shape: the vector's operator[] is STLport's `*(begin() + n)` through an inline begin() accessor (`_SweepBegin1()`): the element
//   address is formed from the accessor's returned pointer plus the scaled index as retail; `first[n]` folds the index into the
//   load and recolours the registers (the GOTCHAS wave-25 rule, confirmed on the RTM compiler).
namespace QuadricOptimiserInternals {
struct C3DVector {
    float X, Y, Z;
};

struct CSubContraction {
    unsigned long Index1;
    unsigned long Index2;
};

template<class T> struct CArray {           // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
    T* _SweepBegin1() const { return m_pData; }
    T& operator[](unsigned long index) { return *(_SweepBegin1() + index); }
};

struct CVertexPairContraction {
    bool                     Exists;            // +0x00
    bool                     Unsplit;           // +0x01
    CArray<CSubContraction>  SubContractions;   // +0x04
    float                    Weight2;           // +0x10
    float                    Cost;              // +0x14
    C3DVector                ResultingPosition; // +0x18
    void*                    ListIterator;      // +0x24
};
}

using namespace QuadricOptimiserInternals;

struct COptimisedPrimitive {
    bool DetectFoldOver(CSubContraction* subContraction, C3DVector* position);   // 0x00ACD0E0
    bool DetectFoldOver(CVertexPairContraction* contraction);
};

bool COptimisedPrimitive::DetectFoldOver(CVertexPairContraction* contraction)
{
    for (unsigned long i = 0; i < contraction->SubContractions.Size(); ++i) {
        if (DetectFoldOver(&contraction->SubContractions[i], &contraction->ResultingPosition))
            return true;
    }
    return false;
}