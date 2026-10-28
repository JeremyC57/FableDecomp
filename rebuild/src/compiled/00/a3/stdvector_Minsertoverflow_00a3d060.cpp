// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<C3DAnimationStateAlpha>) STLport 4.6 growth insert: allocate size + max(size, fill_len) elements, copy the head, x (once, or fill_len times) and the tail unless atend, destroy (virtual destructor) and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VC3DAnimationStateAlpha@@V?$allocator@VC3DAnimationStateAlpha@@@std@@@std@@IAEXPAVC3DAnimationStateAlpha@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3D060, 291 bytes, thiscall `ret 14h`.
// Identity (proven): STLport 4.6 vector<_Tp>::_M_insert_overflow(.., const __false_type&, ..), protected (IAE) as in
//   STLport 4.6 _vector.h; callers are this vector's _M_fill_insert 0x00A3D190 and push_back 0x00A3D380 (landed, which
//   already binds 0x00A3D060 under this behaviour). Catalog label "Vector_Reallocate_CDisplayModeDesc" is propagated.
// Element (proven): C3DAnimationStateAlpha -- the copy stores the three vtables 0x012354D0 (CIVCountedPointeeBase) /
//   0x01249F50 (C3DAnimationState) / 0x01265128 (C3DAnimationStateAlpha) and copies IVRefCount +4, AnimationIndex +8,
//   AnimationTime +0xC, AlphaWeight +0x10 (Ego_r.pdb layout, 20 bytes; 66666667h / sar 3 stride); the vector is
//   C3DAnimationBlendState::CSharedData's combination-blend vector (landed push_back 0x00A3D380, resize 0x00A3D500
//   bound `vector<C3DAnimationStateAlpha>::resize`). The base destructor is a user-written inline virtual: the ??_G in
//   slot 0 (0x0070FE30) stores only 0x012354D0, and retail destroys elements through `p->~T()` (push 0 ; call [eax]).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy<C3DAnimationStateAlpha*, C3DAnimationStateAlpha*>
//   0x00A3CB90 (head and tail), __uninitialized_fill_n<C3DAnimationStateAlpha*, unsigned int, C3DAnimationStateAlpha>
//   0x00A3CBE0 (both landed, same retail TU).
// Shape: STLport 4.6 vector.c verbatim; the implicit copy constructor inline for the single fill, _M_clear's _Destroy
//   loop inline (__forceinline __destroy_aux: virtual destructor call per element). The two STLport helpers get
//   same-TU noinline bodies (retail holds them in this TU): declared only, VC7.1 keeps a second frame word and puts the
//   tag on the fill_len slot; with the bodies visible new_start is homed in the dead fill_len slot and every tag shares
//   the atend slot, as retail. At landing the retail COMDATs of those helpers win.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CIVCountedPointeeBase {                               // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase() { }                    // slot 0 (inline: ??_G stores only this vtable)
    unsigned long IVRefCount;                               // +0x04
};

class C3DAnimationState : public CIVCountedPointeeBase {    // vtable 0x01249F50
public:
    virtual long GetType() const;
    virtual void Validate();
    long  AnimationIndex;                                   // +0x08
    float AnimationTime;                                    // +0x0C
};

class C3DAnimationStateAlpha : public C3DAnimationState {   // vtable 0x01265128, sizeof 0x14
public:
    virtual long GetType() const;                           // 0x0070FE20
    virtual void Validate();                                // 0x00A3BE20
    float AlphaWeight;                                      // +0x10
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                   // 0x00A3CB90
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                   // 0x00A3CBE0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)
template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
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
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
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
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
} // namespace std

template void std::vector<C3DAnimationStateAlpha>::_M_insert_overflow(C3DAnimationStateAlpha*, const C3DAnimationStateAlpha&,
                                                                      const std::__false_type&, unsigned int, bool);