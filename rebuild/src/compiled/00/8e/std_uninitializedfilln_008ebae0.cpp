// std::__uninitialized_fill_n(CHoverState* first, unsigned int n, const CHoverState& x, const __false_type&) -- copy-construct n copies of a hover state into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x008EBAE0, 76 bytes, `ret 8`: __fastcall (first ecx, n edx; x and the empty tag by reference on
// the stack), returning the end pointer in eax. Only call site 0x008EC12D in the vector<CHoverState> assign/fill path
// at 0x008EC070 (catalogued std_vector_assign<CExplosionRing>).
// Identity: the catalog's `_Fill<pair<EHeroMorphType, CTextureMorphEntry>*>` is propagated -- the element starts with a
// CCharString (copy constructor 0x0099EC30) and is NCreatureMode::CModeHoverBase::CHoverState (Ego_r.pdb, 24 bytes):
// AnimName CCharString +0, Area CRadialArea +4 (ThetaLow, ThetaHigh, RadHigh), MovementType +0x10, MovementSpeed +0x14;
// vector<CHoverState> is the HoverStates member of the landed CModeHoverBase constructor 0x008EAC40.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: CHoverState's implicit copy constructor is memberwise because CCharString declares one: the string copy call,
// the trivially copyable CRadialArea as a block through `lea` (eax / ecx), then the two words. The placement new keeps
// the `test esi,esi` null check; n counts down in ebx.
#include <new>
class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
};
enum EMovementType { MOVEMENT_TYPE_NONE };
struct CRadialArea {                            // 12 bytes
    float ThetaLow;                             // +0x00
    float ThetaHigh;                            // +0x04
    float RadHigh;                              // +0x08
};
namespace NCreatureMode {
struct CHoverState {                            // CModeHoverBase::CHoverState, 24 bytes
    CCharString   AnimName;                     // +0x00
    CRadialArea   Area;                         // +0x04
    EMovementType MovementType;                 // +0x10
    float         MovementSpeed;                // +0x14
};
}
using NCreatureMode::CHoverState;

struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template CHoverState* __fastcall __uninitialized_fill_n(CHoverState*, unsigned int, const CHoverState&, const __false_type&);
}