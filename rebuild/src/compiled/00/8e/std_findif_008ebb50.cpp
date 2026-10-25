// std::__find_if(const CHoverState* first, const CHoverState* last, CIsValidHoverStateForMovementType pred, const random_access_iterator_tag&) -- STLport 4x-unrolled find_if over the hover states: the first state of the wanted movement type whose radial area contains the velocity (radius not checked), else last
// Leaf: ??$__find_if@PBVCHoverState@CModeHoverBase@NCreatureMode@@VCIsValidHoverStateForMovementType@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EBB50, 320 bytes incl. int3 padding (316 of code), /Gr `__fastcall`
//   (first in ecx, last in edx, the 12-byte predicate by value and the tag reference on the stack), `ret 10h`.
// Identity: the catalog's "Search_GeometryQuadByPlaneID" is descriptive. Callers: 0x008EA908 (the GetBestHoverState
//   search, which returns 1.0f when the result is the end, else the state's MovementSpeed at +0x14) and 0x008EBE3F. The
//   predicate is the FableWin/Ego PDB functor NCreatureMode::CModeHoverBase::CIsValidHoverStateForMovementType
//   (ctor (const C2DVector&, EMovementType), `bool operator()(const CHoverState&) const`): the caller builds it in the
//   argument slot as {C2DVector, EMovementType}; the element is the 0x18-byte CHoverState {CCharString AnimName +0,
//   CRadialArea Area +4, EMovementType MovementType +0x10, float MovementSpeed +0x14} (landed 0x008EBA90 / 0x008EC070).
// Callees: NCreatureMode::CModeHoverBase::CRadialArea::IsPosInArea(const C2DVector&, bool) const 0x008EA3F0 (landed; its
//   body is repeated above as a same-TU noinline stand-in so VC7.1 knows it preserves ecx).
// Shape: STLport 4.6 `__find_if(first, last, pred, const random_access_iterator_tag&)` verbatim: trip count
//   (last - first) >> 2 over the 4-way unrolled body, then the `switch (last - first)` fall-through cases 3/2/1; the
//   inlined operator() tests the movement type before the area call (`&&`), &state.Area strength-reduced into ecx.
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sin, cos, sqrt)

static const float TWO_PI = 6.2831855f;

class C2DVector {
public:
    float X, Y;
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    float Dot(const C2DVector& other) const { return X * other.X + Y * other.Y; }
};

enum EMovementType { MOVEMENT_TYPE_NONE };

class CCharString {
public:
    void* PStringData;                                      // +0x00
};

namespace NCreatureMode {
class CModeHoverBase {
public:
    class CRadialArea {                                     // 12 bytes
    public:
        float ThetaLow;                                     // +0x00
        float ThetaHigh;                                    // +0x04
        float RadHigh;                                      // +0x08
        bool IsPosInArea(const C2DVector& pos, bool checkRadius) const;     // 0x008EA3F0
    };
    class CHoverState {                                     // Ego_r.pdb, 24 bytes
    public:
        CCharString   AnimName;                             // +0x00
        CRadialArea   Area;                                 // +0x04
        EMovementType MovementType;                         // +0x10
        float         MovementSpeed;                        // +0x14
    };
    class CIsValidHoverStateForMovementType {               // PDB functor, 12 bytes
    public:
        C2DVector     m_velocity;                           // +0x00
        EMovementType m_movementType;                       // +0x08
        bool operator()(const CHoverState& state) const
        {
            return state.MovementType == m_movementType && state.Area.IsPosInArea(m_velocity, false);
        }
    };
};
}

// Same-TU stand-in: the landed body of 0x008EA3F0 (never inlined). VC7.1 sees that it reads `this` only through ecx and
// keeps &state.Area in ecx across every call, as retail does.
#pragma auto_inline(off)
__declspec(noinline) bool NCreatureMode::CModeHoverBase::CRadialArea::IsPosInArea(const C2DVector& pos, bool checkRadius) const
{
    float length = pos.GetLength();
    if (length < 0.0001f)
        return true;
    double thetaLow = ThetaLow * (double)TWO_PI;
    C2DVector low;
    low.X = (float)sin(thetaLow);
    low.Y = (float)cos(thetaLow);
    double thetaHigh = ThetaHigh * (double)TWO_PI;
    C2DVector high;
    high.X = (float)sin(thetaHigh);
    high.Y = (float)cos(thetaHigh);
    float scale = 1.0f / length;
    C2DVector direction;
    direction.X = pos.X * scale;
    direction.Y = pos.Y * scale;
    if (!checkRadius || length < RadHigh) {
        float arcCos = high.Dot(low);
        if (direction.Dot(low) >= arcCos && direction.Dot(high) >= arcCos)
            return true;
    }
    return false;
}
#pragma auto_inline(on)

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
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

template const NCreatureMode::CModeHoverBase::CHoverState* __fastcall
__find_if<const NCreatureMode::CModeHoverBase::CHoverState*, NCreatureMode::CModeHoverBase::CIsValidHoverStateForMovementType>(
    const NCreatureMode::CModeHoverBase::CHoverState*, const NCreatureMode::CModeHoverBase::CHoverState*,
    NCreatureMode::CModeHoverBase::CIsValidHoverStateForMovementType, const random_access_iterator_tag&);

} // namespace std