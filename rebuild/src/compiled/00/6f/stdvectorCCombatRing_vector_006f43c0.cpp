// std::vector<CCombatRing>::vector(const vector& x) -- STLport copy constructor: allocate x.size() rings, copy-construct them
// Leaf: ??0?$vector@VCCombatRing@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F43C0, 104 bytes, `ret 4`.
// Label correction: catalog "Vector_InitFromRange_Prongs" -> STLport vector<CCombatRing>::vector(const vector& x).
// Callees: malloc 0x00BFEA0E, __uninitialized_copy<const CCombatRing*> 0x006F4140 (separate row; same-TU noinline body
//   so the empty tag temporary is homed in the dead `x` argument slot, GOTCHAS 24l).
// Shape: `_Vector_base(n, a)` zero-initialises the three pointers in its init list, then allocates in the body (the
//   zero stores land before the size computation only in this form); the copy then sets _M_finish.
extern "C" void* __cdecl malloc(unsigned int size);      // MSVCR71 import thunk 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
struct __false_type { __false_type() {} };

template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template<class T, class Alloc> class _Vector_base {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
    _Vector_base(unsigned int n, const Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = Alloc(a).allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
};

template<class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    vector(const vector<T, Alloc>& x);
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    Alloc get_allocator() const { return Alloc(); }
};
}

class CDefendingCombatantInfo {
public:
    class CProng;
    class CCombatRing {                                   // Ego_r.pdb (retail 0x1c: 12-byte STLport vector)
    public:
        float RotationAngle;                              // +0x00
        float DeltaRotation;                              // +0x04
        long Index;                                       // +0x08
        float RingThickness;                              // +0x0c
        std::vector<CProng> Prongs;                       // +0x10
    };
};

namespace std {
template<> vector<CDefendingCombatantInfo::CProng>::vector(const vector<CDefendingCombatantInfo::CProng>& x);   // 0x006F20A0
}

namespace std {
#pragma auto_inline(off)
// 0x006F4140 (separate row; same-TU body so the empty tag can share the dead `x` slot)
template<class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
#pragma auto_inline(on)

template<class T, class Alloc>
vector<T, Alloc>::vector(const vector<T, Alloc>& x)
    : _Vector_base<T, Alloc>(x.size(), x.get_allocator())
{
    this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
}

template vector<CDefendingCombatantInfo::CCombatRing>::vector(const vector<CDefendingCombatantInfo::CCombatRing>& x);
}