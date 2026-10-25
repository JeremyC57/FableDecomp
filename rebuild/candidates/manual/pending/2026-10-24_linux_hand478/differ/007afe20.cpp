// std::vector::_M_fill_insert(CIntelligentPointer<CThingAICreature>* pos, size_type n, const CIntelligentPointer<CThingAICreature>& x) -- STLport fill-insert of n copies of a creature intelligent pointer at pos: shift in place when the capacity suffices, else grow through _M_insert_overflow
// Grade: DIFFER(302v302) -- ebx/ebp swap only: retail keeps `pos` in ebp and elems_after (then n*8) in ebx; ours the reverse. Tried: STLport iterator-tag chain vs a plain tag + __forceinline __copy_backward_ptrs, QFE-4035, a same-TU noinline _M_insert_overflow stand-in.
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AFE20, length 304 bytes (thiscall, `ret 0Ch`).
// Identity: the catalog label "Vector_InsertRange_CBaseIntelligentPointer" is descriptive. Callers: vector::resize
//   0x007B00C0 and the insert 0x007B0000 of CTCBoss's TU; the 8-byte element is CIntelligentPointer<CThingAICreature>
//   (vtable 0x0123E7E0, RTTI `.?AV?$CIntelligentPointer@VCThingAICreature@@@@`; Ego_r.pdb CTCBoss::HelperCreatures, the
//   vector the landed CTCBoss::SetHelperCreature 0x007AF310 grows through _M_insert_overflow 0x007AFD10).
// Callees: CBaseIntelligentPointer() 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (x_copy), __uninitialized_copy
//   0x007AF8F0 (twice), __copy_backward 0x007AFA50 (random_access_iterator_tag, null distance), __uninitialized_fill_n
//   0x007AF940, fill 0x007AF980, ~CBaseIntelligentPointer 0x00A01C10 (x_copy), _M_insert_overflow 0x007AFD10.
// Shape: STLport 4.x vector.c `_M_fill_insert` verbatim (as 0x00770990 / 0x0076B950): x_copy is a frame local (the
//   pointer's inline copy constructor: base ctor, vtable store, SetPItem(x.GetPItem())) destroyed by the direct base
//   destructor call; `__copy_backward_ptrs` and `uninitialized_fill_n` are inline and forward to out-of-line
//   `__copy_backward` / `__uninitialized_fill_n`. The tag-taking helpers are same-TU `__declspec(noinline)` bodies that
//   never read their tags, so every tag is homed on the dead `n` slot.
inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
class CThingAICreature;

class CBaseIntelligentPointer {
public:
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7E0 for CThingAICreature
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x007AF8F0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x007AF940
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x007AFA50
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x007AF980
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x007AFD10
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
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

template void vector<CIntelligentPointer<CThingAICreature> >::_M_fill_insert(CIntelligentPointer<CThingAICreature>*,
                                                                             unsigned int,
                                                                             const CIntelligentPointer<CThingAICreature>&);

}
