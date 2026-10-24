// std::vector<CIntelligentPointer<CParticleEmitter> >::_M_fill_insert(pointer position, unsigned int n, const value_type& x) -- STLport insert of n copies of x before position (in place when capacity allows, else the growth path)
// Leaf: _M_fill_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x006568F2, 232 bytes, thiscall `ret 0Ch`. Callers: the two-argument resize
// 0x00656F6D (jmp 0x00656EE8 / call 0x00656FA1) of the NPlayerGui CDraw* Effects vectors, element
// CIntelligentPointer<NParticleEngine::CParticleEmitter> (copy constructor 0x00654DAF, vtable 0x0125A26C).
// Callees: CIntelligentPointer copy constructor @ 0x00654DAF (x_copy), __uninitialized_copy @ 0x006556A8,
// __copy_backward(first, last, result, const random_access_iterator_tag&, int*) @ 0x00655855 (`ret 0Ch`: `push 0`),
// fill @ 0x006556D3, __uninitialized_fill_n @ 0x00654E90 (all `__fastcall`), ~CBaseIntelligentPointer @ 0x00A01C10
// (x_copy's implicit destructor), _M_insert_overflow @ 0x006566C3 (the growth path, atEnd defaulted false).
// Label correction: the catalog "std_vector_insert_IntelligentPointer_n" is descriptive; this is STLport 4.x
// `vector::_M_fill_insert`.
// Shape: STLport verbatim (`_Tp __x_copy = __x;` in the frame, elems_after / old_finish, the two branches,
// `__copy_backward_ptrs` inline over the out-of-line `__copy_backward`). The empty tag temporaries share the dead high
// byte of the position slot ([ebp+0Bh]): reproduced by same-TU `auto_inline(off)` noinline template bodies of the four
// tag-taking helpers with the tag parameter unnamed (GOTCHAS empty-tag rule).
// Opt: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }

class CBaseIntelligentPointer {
public:
    virtual void v000();
    CBaseObject* PItem;                               // +0x04
    ~CBaseIntelligentPointer();                       // 0x00A01C10
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v000();
    CIntelligentPointer(const CIntelligentPointer<T>& other);              // 0x00654DAF
    CIntelligentPointer<T>& operator=(const CIntelligentPointer<T>& other); // 0x0065533C
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Copy_Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Copy_Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, allocator<_Tp> > _M_end_of_storage; // +0x08
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x006566C3
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                __uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy, __false_type());
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::_M_fill_insert(
    CIntelligentPointer<NParticleEngine::CParticleEmitter>*, unsigned int, const CIntelligentPointer<NParticleEngine::CParticleEmitter>&);
}