// std::__find_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CFilter_Bind3 pred, const random_access_iterator_tag&) -- STLport random-access find_if: the first target point inside the field of view, further than the range from the position and not the locked target, or last
// Grade: DIFFER(659v749) -- inline budget: retail expands the FurtherFromPosThan range test (0x00805920) at six of the seven predicate sites (only the 4th unrolled loop test calls it), the lane at four (tests 3 and 4 and switch case 3 call it); that also swaps ebx/ebp (F2 vs last). Everything else (float code, F1/F3 order, trip count, switch) matches; __forceinline on the C3DVector helpers or on CFilter_Bind3::operator() changes nothing.
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00807C30, 750 bytes, /Gr `__fastcall` (first ecx, last edx -- spilled to the
//   `push ecx` word; the 12-byte CFilter_Bind3 by value and the tag reference on the stack, `ret 10h`).
// Identity: Ego_r.pdb `std::_Find_if<CTargetPoint*, CFilter_Bind3<CThingFilter_IsInFOV, `anonymous namespace'::
//   CTargetPointFilter_FurtherFromPosThan, std::unary_negate<`anonymous namespace'::CTargetPointFilter_IsLockedTarget> > >`
//   (donor 0x00801860) is the Dinkumware counterpart; retail is STLport's `__find_if` (callers 0x008046FB, 0x0080891F,
//   0x00809476). The catalog's "std_remove_if_ByPredicate" is wrong (nothing is removed: this is the find_if half).
// Layout: Ego_r.pdb CFilter_Bind3 {F1, F2, F3 pointers}; CTargetPointFilter_FurtherFromPosThan {C3DVector* Pos, float
//   Range}; CTargetPointFilter_IsLockedTarget {CThing* PLockedTarget}; NPlayerTargeting::CTargetPoint (0x1c).
// Callees: CThingFilter_IsInFOV::operator()(const CTargetPoint&) const 0x006A7380, CBaseIntelligentPointer::GetPItem
//   0x00A01B50.
// Shape: STLport 4.x `__find_if` for random-access iterators (trip count `(last - first) >> 2`, four tests per pass,
//   then `switch (last - first)` over 3/2/1, default returns last); the bind is `(*F1)(t) && (*F2)(t) && (*F3)(t)`
//   with the range test inline (`(t.TargetingPosition - *Pos).GetLengthSquared() > Range * Range`, the difference
//   kept on the x87 stack, Z,Y,X sum order) and F3 STLport's `!_M_pred(x)` (`point.PThing == PLockedTarget`: the locked target, an
//   argument of the inline operator==, is read into a register before GetPItem).
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    void* PData;                                           // +0x04
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

// The intelligent pointer's comparison with a raw pointer (the landed 0x006E66E0 form): the raw pointer is an argument,
// so it is read before GetPItem runs.
template <class T> inline bool operator==(const CIntelligentPointer<T>& pointer, T* object)
{
    return pointer.GetPItem() == (CBaseObject*)object;
}

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

namespace NPlayerTargeting {
class CTargetPoint {                                       // 0x1c bytes
public:
    CIntelligentPointer<CThing> PThing;                    // +0x00
    long Index;                                            // +0x08
    C3DVector TargetingPosition;                           // +0x0c
    long FrameRejected;                                    // +0x18
};
}
using NPlayerTargeting::CTargetPoint;

class CThingFilter_IsInFOV {                               // Ego_r.pdb, 0x24 bytes
public:
    unsigned char _pad00[0x24];
    bool operator()(const CTargetPoint& point) const;      // 0x006A7380
};

namespace {
class CTargetPointFilter_FurtherFromPosThan {              // Ego_r.pdb, 8 bytes
public:
    C3DVector* Pos;                                        // +0x00
    float Range;                                           // +0x04
    bool operator()(const CTargetPoint& point) const
    {
        return (point.TargetingPosition - *Pos).GetLengthSquared() > Range * Range;
    }
};
class CTargetPointFilter_IsLockedTarget {                  // Ego_r.pdb, 4 bytes
public:
    typedef CTargetPoint argument_type;
    CThing* PLockedTarget;                                 // +0x00
    bool operator()(const CTargetPoint& point) const
    {
        return point.PThing == PLockedTarget;
    }
};
}

namespace std {
template <class _Predicate> class unary_negate {
protected:
    _Predicate _M_pred;
public:
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};
}

template <class TF1, class TF2, class TF3> class CFilter_Bind3 {
public:
    TF1* F1;                                               // +0x0
    TF2* F2;                                               // +0x4
    TF3* F3;                                               // +0x8
    template <class T> bool operator()(const T& t) const { return (*F1)(t) && (*F2)(t) && (*F3)(t); }
};
typedef CFilter_Bind3<CThingFilter_IsInFOV, CTargetPointFilter_FurtherFromPosThan,
                      std::unary_negate<CTargetPointFilter_IsLockedTarget> > Pred;

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}

template CTargetPoint* __fastcall __find_if(CTargetPoint*, CTargetPoint*, Pred, const random_access_iterator_tag&);
}
