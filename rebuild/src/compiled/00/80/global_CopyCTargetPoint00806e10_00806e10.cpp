// _global::Copy_CTargetPoint_00806e10(const CTargetPoint* first, const CTargetPoint* last, CTargetPoint* result, const random_access_iterator_tag&, int*) -- STLport __copy<NPlayerTargeting::CTargetPoint*>: counted forward assignment of 28-byte target points, returns the advanced result
// Leaf: Copy_CTargetPoint_00806e10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806E10, length 135 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Types (Ego_r.pdb): NPlayerTargeting::CTargetPoint (28 bytes) = PThing CIntelligentPointer<CThing> +0, Index +8,
//   TargetingPosition C3DVector +0xC, FrameRejected +0x18 (the model of the landed __linear_insert @ 0x0080AA60).
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed count
// `last - first` (imul 0x92492493 ; add ; sar 4 = /28) computed once and kept in the dead `result` argument slot
// (`mov [esp+14h],eax ; ... dec eax`), then CTargetPoint's assignment inlined per element: SetPItem(src.GetPItem()),
// Index, the C3DVector through its IMPLICIT assignment (both sides re-addressed in ecx/edx: `mov ecx,esi ; mov edx,edi`)
// and FrameRejected, the four pointers (src, src+0xC, dst, dst+0xC) stepped by 0x1c. The advanced result is returned.
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on the stack.
// Label: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` names the element correctly (the body is STLport __copy).
struct CThing;
struct CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NPlayerTargeting {
struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;          // +0x00
    long                        Index;           // +0x08
    C3DVector                   TargetingPosition; // +0x0C
    long                        FrameRejected;   // +0x18
    __forceinline CTargetPoint& operator=(const CTargetPoint& other)
    {
        PThing = other.PThing;
        Index = other.Index;
        TargetingPosition = other.TargetingPosition;
        FrameRejected = other.FrameRejected;
        return *this;
    }
};
}
using NPlayerTargeting::CTargetPoint;

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CTargetPoint* __fastcall Copy_CTargetPoint_00806e10(const CTargetPoint* first, const CTargetPoint* last, CTargetPoint* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}