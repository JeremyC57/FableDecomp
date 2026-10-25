// std::vector::insert(CCountedPointer<NShape::CShape>* pos, const CCountedPointer<NShape::CShape>& x) -- STLport single-element insert into CTCShapeManager's shape list: append in place, or open a slot by copying the tail up one and assign x, or grow; returns begin() + the old index
// Leaf: ?insert@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007658B0, length 247 bytes (thiscall, `ret 8`).
// Identity: the catalog label "Vector_Insert_CCountedPointer_MoveAndRealloc" is descriptive. The only caller is the landed
//   CTCShapeManager::AddNewShape 0x00762580 (`PersistantShapes.insert(PersistantShapes.begin(), shape)`), so the vector
//   is vector<CCountedPointer<NShape::CShape> > (8-byte elements {Data, Info}; count block {m_references, destroy member
//   pointer, m_pItem}, released with operator delete 0x00BFE9BC).
// Callees: __copy_backward(first, last, result, random_access_iterator_tag, int*) 0x00764640 (catalog
//   "_Copy_backward<CCountedPointer<C3DGroup2>*,...>", a byte-identical COMDAT), CCountedPointer::operator= 0x00764400
//   (catalog "CCountedPointer<CDiskFileWin32>", the same folded COMDAT), operator delete 0x00BFE9BC (x_copy's inlined
//   destructor), _M_insert_overflow(pos, x, __false_type, 1, false) 0x007656D0.
// Shape: STLport 4.x `insert(iterator, const T&)` verbatim: `n = pos - begin()`; with room, append via _Copy_Construct
//   (inline copy constructor: two words + the count increment under the placement-new null test) or copy-construct
//   finish from finish[-1], ++finish, `T x_copy = x`, `__copy_backward_ptrs(pos, finish - 2, finish - 1, _TrivialAss())`,
//   `*pos = x_copy` and x_copy's inline destructor; else _M_insert_overflow(pos, x, _IsPODType(), 1). __copy_backward is a
//   same-TU `__declspec(noinline)` body that never reads its tag, so VC7.1 homes both tags on the dead `x` slot; the
//   element operator= also carries its same-TU noinline body, which lets VC7.1 treat x_copy as uncaptured and keep its
//   count block in edi across both calls (declared-only, x_copy is reloaded from the frame).
inline void* operator new(unsigned int, void* p) { return p; }
void __cdecl operator delete(void* block);                                         // 0x00BFE9BC

namespace NShape {
class CShape;
}

template <class T> struct SCountBlock {
    long m_references;                                      // +0x00
    void (T::*m_pfnDestroy)();                              // +0x04
    T*   m_pItem;                                           // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    SCountBlock<T>* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->m_references == 0) {
            (Info->m_pItem->*Info->m_pfnDestroy)();
            operator delete(Info);
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other);                    // 0x00764400
};

class NShape::CShape {
public:
    void DeleteData();
};

#pragma auto_inline(off)
template <class T>
__declspec(noinline) CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer<T>& other)
{                                                                                              // 0x00764400
    if (Info != other.Info) {
        if (Info && --Info->m_references == 0) {
            (Info->m_pItem->*Info->m_pfnDestroy)();
            operator delete(Info);
        }
        Data = other.Data;
        Info = other.Info;
        if (Info)
            ++Info->m_references;
    }
    return *this;
}
#pragma auto_inline(on)

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

template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x00764640
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
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x007656D0
    iterator insert(iterator __position, const _Tp& __x);
};

template <class _Tp, class _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(iterator __position, const _Tp& __x)
{
    size_type __n = __position - begin();
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        if (__position == end()) {
            _Copy_Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else {
            _Copy_Construct(this->_M_finish, *(this->_M_finish - 1));
            ++this->_M_finish;
            _Tp __x_copy = __x;
            __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, _TrivialAss());
            *__position = __x_copy;
        }
    }
    else
        _M_insert_overflow(__position, __x, _IsPODType(), 1UL);
    return begin() + __n;
}

template vector<CCountedPointer<NShape::CShape> >::iterator
vector<CCountedPointer<NShape::CShape> >::insert(CCountedPointer<NShape::CShape>*, const CCountedPointer<NShape::CShape>&);

}