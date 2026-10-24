// _STL::__uninitialized_copy(CHoverState*, CHoverState*, CHoverState*, const __false_type&) -- copy-construct the range [first, last) into raw storage at result and return the end of the copies
// Leaf: __uninitialized_copy
// Retail 0x008EBA90, 0x50 bytes, __fastcall (first in ecx, last in edx, result and the empty tag by reference on the
//   stack, `ret 8`), returns the end of the copies in eax. Manifest label "_Copy_backward<CExplosionRing*,...>" is
//   propagated and wrong: retail CExplosionRing is 16 bytes with no string, while this element is 0x18 bytes starting
//   with a CCharString. Both call sites (0x008EC0DC, 0x008EC147) are in vector<CHoverState>::_M_insert_overflow
//   0x008EC070 (the landed push_back 0x008EC1A0 calls it), so the element is NCreatureMode::CModeHoverBase::CHoverState
//   (Ego_r.pdb, as identified for the landed sibling __uninitialized_fill_n 0x008EBAE0).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` = `for (; first != last; ++first,
//   ++cur) _Construct(&*cur, *first)`: the placement new keeps VC's `test edi,edi` null test, CHoverState's implicit
//   copy is memberwise (CCharString declares a copy ctor): the string call, the 12-byte CRadialArea through `lea`
//   (eax/ecx) and the two words.
inline void* operator new(unsigned int, void* __p) { return __p; }

class CCharString {
public:
    void* PStringData;                          // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
};

class CRadialArea {                             // 12 bytes
public:
    float ThetaLow;                             // +0x00
    float ThetaHigh;                            // +0x04
    float RadHigh;                              // +0x08
};

enum EMovementType { MOVEMENT_TYPE_NONE };

namespace NCreatureMode {
class CModeHoverBase {
public:
    class CHoverState {                         // 24 bytes (Ego_r.pdb)
    public:
        CCharString   AnimName;                 // +0x00
        CRadialArea   Area;                     // +0x04
        EMovementType MovementType;             // +0x10
        float         MovementSpeed;            // +0x14
    };
};
}

namespace _STL {
struct __false_type { __false_type() {} };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template NCreatureMode::CModeHoverBase::CHoverState* __fastcall
__uninitialized_copy(NCreatureMode::CModeHoverBase::CHoverState*, NCreatureMode::CModeHoverBase::CHoverState*,
                     NCreatureMode::CModeHoverBase::CHoverState*, const __false_type&);
}