// std::__uninitialized_fill_n(CProng* first, unsigned int n, const CProng& x, const __false_type&) -- STLport placement-copy of one 28-byte combat-wheel prong into n raw slots; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F1180, 113 bytes, `ret 8` (/Gr: first in ecx -> ebp as `cur`, n in edx spilled
//   to the pushed-ecx slot and counted down there; &x and the empty __false_type tag reference on the stack).
// Caller: 0x006F3F96 in the vector<CDefendingCombatantInfo::CProng> insert helper 0x006F3F00 (28-byte stride; catalog
//   "std::vector::_Insert_range_fill_28byte").
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90;
//   CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8 stored after the base constructor.
// Layout (Ego_r.pdb CDefendingCombatantInfo::CProng, 28 bytes; as the landed __uninitialized_copy 0x006F14A0): Blocked
//   bool +0x00, Prong C3DVector +0x01 (C3DVector is 1-aligned in this module: pack(1)), ProngBlockageTimer +0x10,
//   POccupier CIntelligentPointer<CThingCreatureBase> +0x14.
// Shape: STLport _uninitialized.h -- `cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`
//   (unsigned size_t: `test edx,edx ; jbe`); the placement-new null test, then CProng's implicit copy constructor: the
//   bool byte, the packed vector's three words (through a second +9-biased destination register), the timer, then the
//   intelligent pointer's copy (base ctor, vtable store, SetPItem(x.GetPItem())).
// Label: the catalog's `fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>` is a propagated label.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CBaseObject;
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:      // vtable 0x0123E7D8 for CThingCreatureBase
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

#pragma pack(push, 1)
class C3DVector { public:                                          // 1-aligned in retail (CProng::Prong sits at +1)
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

struct CProng {                                             // CDefendingCombatantInfo::CProng, 0x1c bytes
    bool                                    Blocked;              // +0x00
    C3DVector                               Prong;                // +0x01
    long                                    ProngBlockageTimer;   // +0x10
    CIntelligentPointer<CThingCreatureBase> POccupier;            // +0x14
};

namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

template CProng* __fastcall std::__uninitialized_fill_n<CProng*, unsigned int, CProng>(CProng*, unsigned int, const CProng&,
                                                                                      const std::__false_type&);