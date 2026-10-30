// std::vector<CGraphicInfoFrame>::_M_fill_insert(CGraphicInfoFrame* pos, unsigned int n, const CGraphicInfoFrame& x) -- STLport insert of n copies of a graphic frame: inside the spare capacity shift the tail (copy-construct / assign backwards) and fill with a local copy of x, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A00F50, 336 bytes, thiscall `ret 0Ch` (pos, n, &x). Callers: the inline
//   insert(pos, n, x) wrapper 0x00A01280 (jmp), 0x00A0162A and the landed vector<CGraphicInfoFrame>::resize 0x00A016F0
//   (its source names this callee). The catalog label "CGameMapCellArray_ResizeAndInsert" is a guess: the element is
//   CGraphicInfoFrame (the x_copy stores its vtable 0x0129C878).
// Callees: __uninitialized_copy(first, last, result, const __false_type&) @ 0x00A00750, __copy_backward(first, last,
//   result, const random_access_iterator_tag&, int*) @ 0x00A00840 (landed), fill(first, last, val) @ 0x00A00A90
//   (landed, `__fastcall`, `ret 4`), __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x00A004D0 (landed),
//   _M_insert_overflow(pos, x, const __false_type&, n, false) @ 0x00A00C80, CBaseClass::~CBaseClass @ 0x0099A2E0 (the
//   local copy's destructor after the inline CIVCountedPointer release; the frame data's virtual slot 1 at zero).
// Layout: CGraphicInfoFrame (Ego_r.pdb, 0x10 bytes): CBaseClass vptr +0x00, FrameNum +0x04, SmallMipmapLevel +0x06,
//   Data CIVCountedPointer<CFrameData> +0x08 (RefCount +4), GraphicInfo +0x0C; implicit copy (only the derived vtable
//   store), assignment and destructor (no vtable reset).
// Shape: STLport 4.6 `_M_fill_insert(pos, n, x)` verbatim over the __false_type helpers (`_Tp __x_copy = __x;`, the
//   elems_after split, `_M_insert_overflow(pos, x, _IsPODType(), n)`). The helpers are same-TU noinline bodies under
//   `auto_inline(off)` that never touch their tag, so VC7.1 homes the tag temporaries on the live `pos` argument slot
//   and merges the two branches' tag pushes, as retail does.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

template <class T>
class CIVCountedPointer {                           // intrusive: the object's RefCount at +4, virtual slot 1 frees it
public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

class CGraphicInfo;

class CGraphicInfoFrame : public CBaseClass {       // vtable 0x0129C878, 0x10 bytes
public:
    class CFrameData {
    public:
        virtual void v000();
        virtual void DeleteThis();                  // slot 1 (+0x04)
        long RefCount;                              // +0x04
        void AddRef() { ++RefCount; }
        void Release()
        {
            if (--RefCount == 0)
                DeleteThis();
        }
    };
    unsigned short FrameNum;                        // +0x04
    signed char SmallMipmapLevel;                   // +0x06
    CIVCountedPointer<CFrameData> Data;             // +0x08
    CGraphicInfo* GraphicInfo;                      // +0x0C
};

namespace std {
struct __false_type { __false_type() {} };
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                 // 0x00A00750
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                 // 0x00A004D0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x00A00840
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x00A00A90
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _IsPODType;
    typedef __false_type _TrivialAss;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                // 0x00A00C80
    public:
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<CGraphicInfoFrame>::_M_fill_insert(CGraphicInfoFrame*, unsigned int, const CGraphicInfoFrame&);
}