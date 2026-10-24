// std::remove_copy(CTargetPoint* first, CTargetPoint* last, CTargetPoint* result, const CTargetPoint& val) -- STLport remove_copy: copy every target point that is not equal to val down to result, return the new end
// Leaf: remove_copy
// Retail: Fable.exe @ 0x00806D80, 144 bytes, `ret 8`: __fastcall (first ecx, last edx -- spilled to the `push ecx`
// frame word -- result and the val reference on the stack). The catalog's `_Find<CTargetPoint*>` is the propagated
// name: the body copies, it does not search. Call sites 0x008052B1, 0x00805307, 0x0080796E; the last is STLport
// remove(first, last, val) at 0x00807940 (`first = find(..)` @ 0x008066A0, then remove_copy(first + 1, last, first, val)).
// Types (Ego_r.pdb): NPlayerTargeting::CTargetPoint (28 bytes) = PThing CIntelligentPointer<CThing> +0, Index +8,
//   TargetingPosition C3DVector +0xC, FrameRejected +0x18 (the landed 0x0080AA60 __linear_insert model).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: `*first == val` is CTargetPoint's operator== -- the two resolved things (left first), then Index (val
//   re-read from its stack slot each time); `*result = *first` is CTargetPoint's assignment: SetPItem(GetPItem()), Index,
//   the C3DVector through `lea eax,[esi+0Ch]` (implicit C3DVector assignment) and FrameRejected. VC7.1 strength-reduces
//   result->TargetingPosition into a second induction register (`lea edi,[ebx+0Ch]`, Index stored at [edi-4]).
struct CThing;
struct CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    __forceinline bool operator==(const CIntelligentPointer& other) const { return GetPItem() == other.GetPItem(); }
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
    __forceinline bool operator==(const CTargetPoint& other) const
    {
        return PThing == other.PThing && Index == other.Index;
    }
};
}
using NPlayerTargeting::CTargetPoint;

namespace std {
template <class InputIter, class OutputIter, class T>
OutputIter __fastcall remove_copy(InputIter first, InputIter last, OutputIter result, const T& val)
{
    for ( ; first != last; ++first)
        if (!(*first == val)) {
            *result = *first;
            ++result;
        }
    return result;
}
template CTargetPoint* __fastcall remove_copy(CTargetPoint*, CTargetPoint*, CTargetPoint*, const CTargetPoint&);
}