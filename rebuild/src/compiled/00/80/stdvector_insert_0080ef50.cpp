// std::vector::insert(pair<CIntelligentPointer<CThing>,long>* position, const pair<CIntelligentPointer<CThing>,long>& x) -- STLport single-element insert into the explosion's (thing, emitter id) list: with room, construct at the end (or shift the tail up one slot and assign a local copy of x into the gap); without room, regrow through _M_insert_overflow; return begin() + the old index
// Leaf: ?insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080EF50, 333 bytes, thiscall `ret 8`.
// Identity: the only caller is CTCDExplosion::UpdateDynamicExplosion 0x0080D300 (0x0080D711: `Emitters.insert(
//   Emitters.end(), pair)` on CTCDExplosion::Emitters, retail +0x40, Ego_r.pdb vector<pair<CIntelligentPointer<CThing>,
//   long> >). The catalog's "std_vector_InsertRange" is descriptive.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CIntelligentPointer<CThing> vtable 0x01238C6C, __copy_backward(first, last, result, const
//   random_access_iterator_tag&, int*) 0x0080DFF0 (landed), _M_insert_overflow 0x007893AE (an identical-COMDAT-folded
//   copy with a frame pointer).
// Layout: pair<CIntelligentPointer<CThing>, long> = 12 bytes (the 0x2AAAAAAB reciprocal with `sar 1`): vptr +0,
//   PData +4, second +8.
// Shape: STLport 4.6 `vector::insert(iterator, const T&)` verbatim: `__n = __position - begin()` first, the end-of-storage
//   test, `_Construct(_M_finish, x)` (placement null test + CIntelligentPointer's inline copy constructor) when
//   inserting at the end, else `_Construct(_M_finish, *(_M_finish - 1))`, `++_M_finish`, a local `__x_copy`,
//   `__copy_backward_ptrs` inline to the out-of-line __copy_backward (its tag homed on the dead x slot), the pair's
//   implicit assignment (SetPItem(GetPItem()), then the long) and the implicit x_copy destructor (the base dtor).
inline void* operator new(unsigned int, void* where) { return where; }

class CBaseObject;
class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer<T>& operator=(const CIntelligentPointer<T>& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;                                          // +0x08
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)   // 0x0080DFF0
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x007893AE
    iterator insert(iterator __position, const _Tp& __x)
    {
        size_type __n = __position - begin();
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            if (__position == end()) {
                _Construct(this->_M_finish, __x);
                ++this->_M_finish;
            }
            else {
                _Construct(this->_M_finish, *(this->_M_finish - 1));
                ++this->_M_finish;
                _Tp __x_copy = __x;
                __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, __false_type());
                *__position = __x_copy;
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), 1UL);
        return begin() + __n;
    }
};

template class vector<pair<CIntelligentPointer<CThing>, long> >;
}