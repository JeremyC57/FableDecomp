// std::vector<CMultiStaticMeshEntryDef>::_M_fill_insert(CMultiStaticMeshEntryDef* pos, unsigned int n, const CMultiStaticMeshEntryDef& x) -- STLport vector insert(pos, n, x) for the 56-byte multi static mesh entry defs: shift the tail in place when capacity allows, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EDFDC, 347 bytes, thiscall `ret 0Ch`, ebp frame (the size-optimised /Gr defs TU).
//   Catalog "Vector_Insert_Range_Impl" is descriptive: vector<CMultiStaticMeshEntryDef>::_M_fill_insert (copy constructor
//   0x004EB864 and operator= 0x004EB831 are CMultiStaticMeshEntryDef's in the manifest).
// Callees: CMultiStaticMeshEntryDef copy constructor @ 0x004EB864 (x_copy and the inline placement-new constructs) and
//   operator= @ 0x004EB831 (the inline __copy_backward count loop and the fills), uninitialized_fill_n @ 0x004EE219
//   (tag-less, __fastcall), the base destructor @ 0x009FC550 after the inline vtable store 0x01230BA0,
//   _M_insert_overflow @ 0x004EE137.
// Layout: CMultiStaticMeshEntryDef (Ego_r.pdb, 0x38): CDefClassBase (0x25 bytes, byte-packed) + Mesh +0x28,
//   OverrideLightingChannel +0x2C, OverrideRenderFadeDistance +0x2D, RenderFadeDistance +0x30, LightingChannel +0x34.
// Shape: the 0x004EDB3E model with every copy helper inline here (`__forceinline` _Construct = placement new with its null
//   test, __uninitialized_copy, __copy_backward_ptrs -> __copy_backward(random_access_iterator_tag, int*), fill).
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* p) { return p; }

class CResource {
public:
    virtual void v00();
    virtual ~CResource();                               // 0x009FC550 (labelled "~CMemoryBlock")
};
class CDefPointeeBase : public CResource {      // vtable 0x01230BA0
public:
    virtual void v00();
    ~CDefPointeeBase() { }
};
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {       // 0x25 bytes, byte-packed (landed 0x004E9C39 / 0x004E9BDA model)
public:
    unsigned char _body[0x21];                       // +0x04..+0x25
    virtual void v00();
    CDefClassBase(const CDefClassBase& other);       // 0x0046644C
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)
class CMultiStaticMeshEntryDef : public CDefClassBase {   // 0x38 bytes (Ego_r.pdb)
public:
    long  Mesh;                                      // +0x28
    bool  OverrideLightingChannel;                   // +0x2c
    bool  OverrideRenderFadeDistance;                // +0x2d
    float RenderFadeDistance;                        // +0x30
    long  LightingChannel;                           // +0x34 (ELightingChannel)
    virtual void v00();
    CMultiStaticMeshEntryDef(const CMultiStaticMeshEntryDef& other);                // 0x004EB864
    CMultiStaticMeshEntryDef& operator=(const CMultiStaticMeshEntryDef& other);     // 0x004EB831
};

namespace std {
struct __false_type { };
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type is_POD_type;
};

template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2>
__forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x004EE219

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

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
    for ( ; __first != __last; ++__first)
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
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    typedef typename __type_traits<_Tp>::is_POD_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x004EE137
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

template void vector<CMultiStaticMeshEntryDef>::_M_fill_insert(CMultiStaticMeshEntryDef*, unsigned int, const CMultiStaticMeshEntryDef&);
}