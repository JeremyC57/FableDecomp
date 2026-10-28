// std::vector::_M_insert_overflow(CHoverState* position, const CHoverState& x, const __false_type&, unsigned int fill_len, bool atend) -- (vector<CHoverState>, CHoverState being NCreatureMode::CModeHoverBase::CHoverState) STLport regrowth: allocate size + max(size, fill_len) records, copy the prefix, construct the fill, copy the suffix unless appending, destroy and free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCHoverState@CModeHoverBase@NCreatureMode@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC070, 304 bytes incl. int3 padding (290 of code), thiscall, `ret 14h`.
// Identity: the catalog's "std_vector_assign<CExplosionRing>" is propagated and wrong (retail CExplosionRing is 16 bytes
//   with no string). The single caller is the landed push_back 0x008EC1A0 and both __uninitialized_copy call sites of the
//   landed 0x008EBA90 sit in this body: the 0x18-byte element {CCharString, 12-byte CRadialArea, enum, float} is Ego_r.pdb
//   NCreatureMode::CModeHoverBase::CHoverState (as identified for the landed __uninitialized_fill_n 0x008EBAE0).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport malloc allocator), __uninitialized_copy(first, last, result,
//   const __false_type&) 0x008EBA90 (landed), __uninitialized_fill_n(first, n, x, const __false_type&) 0x008EBAE0
//   (landed) -- both /Gr `__fastcall` -- CCharString::CCharString(const CCharString&) 0x0099EC30 and
//   CCharString::~CCharString() 0x0099EAE0.
// Shape: STLport 4.6 `_M_insert_overflow` verbatim (the same body as vector<C3DSkeletalBoneMorph> 0x0088B4F0): the
//   `(max)(__old_size, __fill_len)` reference select, the new block parked in fill_len's dead slot, every empty
//   __false_type temporary homed on atend's slot (same-TU noinline stand-ins, tags unnamed), the single-element case
//   placement-copies inline (the POD CRadialArea through `lea` pointers), _M_clear's _Destroy is the ~CCharString loop.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class CRadialArea {                                         // 12 bytes, POD
public:
    float ThetaLow;                                         // +0x00
    float ThetaHigh;                                        // +0x04
    float RadHigh;                                          // +0x08
};

enum EMovementType { MOVEMENT_TYPE_NONE };

namespace NCreatureMode {
class CModeHoverBase {
public:
    class CHoverState {                                     // Ego_r.pdb, 24 bytes
    public:
        CCharString   AnimName;                             // +0x00
        CRadialArea   Area;                                 // +0x04
        EMovementType MovementType;                         // +0x10
        float         MovementSpeed;                        // +0x14
    };
};
}

namespace std {

struct __false_type { __false_type() { } };

// Same-TU stand-ins for the out-of-line /Gr callees (real bodies, never inlined, tag parameters unnamed).
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)               // 0x008EBA90
{
    for (; __first != __last; ++__first, ++__result)
        new (__result) NCreatureMode::CModeHoverBase::CHoverState(*__first);
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)             // 0x008EBAE0
{
    for (; __n > 0; --__n, ++__first)
        new (__first) _Tp(__x);
    return __first;
}
#pragma auto_inline(on)

template <class _T1, class _T2>
inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIter>
inline void __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;

    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);
    public:
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<NCreatureMode::CModeHoverBase::CHoverState>;

} // namespace std