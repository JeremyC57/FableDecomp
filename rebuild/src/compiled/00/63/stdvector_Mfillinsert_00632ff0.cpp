// std::vector::_M_fill_insert(NUISystem::CUIStateDef* __position, unsigned int __n, const NUISystem::CUIStateDef& __x) -- (vector<NUISystem::CUIStateDef>) STLport fill-insert of n copies of a UI state def: shift in place when the capacity suffices (x copied first), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCUIStateDef@NUISystem@@V?$allocator@VCUIStateDef@NUISystem@@@std@@@std@@QAEXPAVCUIStateDef@NUISystem@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00632FF0, 509 bytes, thiscall, `ret 0Ch`.
// Identity (proven): every element call is a bound NUISystem::CUIStateDef member -- copy constructor 0x006250B0 (stores
//   the vptr 0x0125871C of RTTI .?AVCUIStateDef@NUISystem@@), operator= 0x00625350 -- the overflow callee is the landed
//   vector<NUISystem::CUIStateDef>::_M_insert_overflow 0x006331F0, the fill helper 0x00633340 copy-constructs the same
//   0x7C-byte record, and x_copy's inline destructor is CUIStateDef's (ChildrenNotAffected at +0x6C, the parent def's
//   SubDefInfoMap vector at +0x29, then the CDefPointeeBase vtable 0x01230BA0 and ~CResource 0x009FC550: the body of
//   the landed ~CUIStateDef 0x00624DC0). Its one caller 0x00632F6D is the vector's resize. The bound spelling
//   ?Insert@CRetailVector_elem124@@... is invented.
// Callees: CUIStateDef(const CUIStateDef&) 0x006250B0, CUIStateDef::operator= 0x00625350, uninitialized_fill_n
//   0x00633340 (__fastcall, three arguments), free 0x00BFEA14, CResource::~CResource 0x009FC550,
//   _M_insert_overflow 0x006331F0.
// Shape: STLport 4.6 vector.c `_M_fill_insert` verbatim; __uninitialized_copy, __copy_backward and fill inline, the
//   uninitialized_fill_n front end out of line (VC7.1 inlines a loop only under __forceinline). The def chain as the landed ~CUIStateDef: byte-packed, the derived defs
//   `__declspec(novtable)` (only CDefPointeeBase's vtable store survives). The overflow tag is value-initialised
//   (`mov byte [esp+23h],0`: __false_type without a user constructor in this TU).
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __true_type { };
struct __false_type { };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<unsigned long> { typedef __true_type has_trivial_destructor; };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x006331F0
};
} // namespace std

class CResource { public:                                  // retail 0x1C bytes (with CIVCountedPointeeBase)
    unsigned char _body[0x1C - 4];
    virtual ~CResource();                                  // 0x009FC550
};
class CDefPointeeBase : public CResource { public:          // vtable 0x01230BA0
    ~CDefPointeeBase() { }
};
#pragma pack(push, 1)                                      // def classes are byte packed in retail
class CDefinitionManager;
class __declspec(novtable) CDefClassBase : public CDefPointeeBase { public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                       // +0x1C
    unsigned long       GlobalIndex;                       // +0x20
    unsigned char       Flags;                             // +0x24 Setup / Template / DefaultValsApplied
};
#pragma pack(pop)
namespace std { template <> struct __type_traits<std::pair<unsigned long, CDefClassBase::CSubDefInfo> >
                { typedef __true_type has_trivial_destructor; }; }
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap { public:   // retail 0x10 bytes
    std::vector<std::pair<K, V> > Vector;                  // +0x00
    unsigned char                 Sorted[4];               // +0x0C
};
class CDefString { public: long TablePos; };
#pragma pack(push, 1)
class __declspec(novtable) CParentDefClassBase : public CDefClassBase { public:
    CDefString InstantiationName;                          // +0x25
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
namespace NUISystem {
#pragma pack(push, 1)
class __declspec(novtable) CUIStateDef : public CParentDefClassBase {   // retail 0x7C bytes (vtable 0x0125871C)
public:
    unsigned char _members[0x6C - 0x39];                   // +0x39 GraphicIndex .. UpdateTime
    std::vector<unsigned long> ChildrenNotAffected;        // +0x6C
    bool          LinearChange;                            // +0x78
    unsigned char _pad79[3];
    CUIStateDef(const CUIStateDef& other);                 // 0x006250B0
    CUIStateDef& operator=(const CUIStateDef& other);      // 0x00625350
};
#pragma pack(pop)
}

namespace std {
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);   // 0x00633340
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
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

typedef NUISystem::CUIStateDef _Elem;
template void vector<_Elem>::_M_fill_insert(_Elem*, unsigned int, const _Elem&);
} // namespace std