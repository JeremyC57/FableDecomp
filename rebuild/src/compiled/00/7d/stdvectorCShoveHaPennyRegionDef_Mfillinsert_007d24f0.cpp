// std::vector<CShoveHaPennyRegionDef>::_M_fill_insert(CShoveHaPennyRegionDef* position, unsigned int n, const CShoveHaPennyRegionDef& x) -- STLport vector insert(pos, n, x) for the shove-ha'penny region defs: with room, shift the tail up (uninitialized copy of the last n or fill of the overhang) and fill the gap from a local copy of x; without room, regrow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D24F0, 337 bytes, thiscall `ret 0Ch`.
// Identity: the vector's resize/insert front ends in this TU; the callees are the CShoveHaPennyRegionDef helpers
//   (0x30-byte element, copy-constructor vtable 0x0124383C). The catalog's "vector_Insert_CBrainUpdateZone" is a
//   propagated label.
// Callees: CDefClassBase(const CDefClassBase&) 0x0046644C (x_copy's inline implicit copy constructor),
//   __uninitialized_copy 0x007D22F0, __copy_backward 0x007D17A0, fill 0x007D1420, __uninitialized_fill_n 0x007D2370,
//   _M_insert_overflow 0x007D23B0 (all __fastcall / thiscall, out of line), ~CResource 0x009FC550 after the
//   CDefPointeeBase vtable store 0x01230BA0 (x_copy's implicit destructor chain).
// Shape: STLport 4.6 `_M_fill_insert` verbatim; every empty tag and both spills (`old_finish - n`, `n - elems_after`)
//   share the dead n argument slot. That frame needs the five out-of-line helpers as same-TU `auto_inline(off)` +
//   noinline template bodies (their real STLport bodies; declared-only, VC7.1 gives the temporaries their own words).
// Layout (Ego_r.pdb names, retail offsets): CResource root (vptr with the virtual destructor 0x009FC550 behind slot
//   0's deleting destructor; six words +0x04..+0x18); CDefPointeeBase (vtable 0x01230BA0, an empty user destructor:
//   its vtable store is the only one a by-value destruction keeps); CDefClassBase, pack(1), 0x25 bytes: PDefManager
//   +0x1C, GlobalIndex +0x20, the Setup / Template / DefaultValsApplied bits at +0x24, an out-of-line user copy
//   constructor 0x0046644C and the implicit assignment; CShoveHaPennyRegionDef (0x30, vtable 0x0124383C): Start
//   +0x28, Score +0x2C, implicit copy constructor / assignment / destructor.
class CResourceList;
class CResource {                                          // 28 bytes
public:
    virtual ~CResource();                                  // 0x009FC550 (labelled "~CMemoryBlock")
    long           m_refCount;                             // +0x04
    CResourceList* ResourceList;                           // +0x08
    CResource*     PrevResource;                           // +0x0C
    CResource*     NextResource;                           // +0x10
    unsigned long  ResourceSize;                           // +0x14
    unsigned long  LastUsedFrame;                          // +0x18
    CResource(const CResource& other);                     // 0x0046649A
};
class CDefPointeeBase : public CResource {                 // vtable 0x01230BA0
public:
    ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {             // 0x25 bytes
public:
    CDefinitionManager* PDefManager;                       // +0x1C
    unsigned long       GlobalIndex;                       // +0x20
    unsigned char       Setup : 1;                         // +0x24 bit 0
    unsigned char       Template : 1;                      // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;            // +0x24 bit 2
    CDefClassBase(const CDefClassBase& other);             // 0x0046644C
};
#pragma pack(pop)
class CShoveHaPennyRegionDef : public CDefClassBase {      // 0x30 bytes, vtable 0x0124383C
public:
    float Start;                                           // +0x28
    float Score;                                           // +0x2C
};

// The two out-of-line copy constructors with their bodies in the TU (noinline): they write only through `this`, so
// VC7.1 knows the x_copy construction leaves the vector alone and keeps the finish pointer it read for the capacity
// test (retail's stale edi in `finish - position`); declared-only, the lane re-reads it. No `auto_inline(off)` here:
// the implicit CDefClassBase destructor is generated with the vtable at this point and would stay out of line.
__declspec(noinline) CResource::CResource(const CResource& other)            // 0x0046649A
    : m_refCount(other.m_refCount), ResourceList(other.ResourceList), PrevResource(other.PrevResource),
      NextResource(other.NextResource), ResourceSize(other.ResourceSize), LastUsedFrame(other.LastUsedFrame)
{
}
__declspec(noinline) CDefClassBase::CDefClassBase(const CDefClassBase& other)  // 0x0046644C
    : CDefPointeeBase(other), PDefManager(other.PDefManager), GlobalIndex(other.GlobalIndex),
      Setup(other.Setup), Template(other.Template), DefaultValsApplied(other.DefaultValsApplied)
{
}

inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x007D22F0
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)                        // 0x007D2370
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)   // 0x007D17A0
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)          // 0x007D1420
{
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
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
#pragma auto_inline(off)
    protected:
    __declspec(noinline) void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                                 size_type __fill_len, bool __atend = false)             // 0x007D23B0
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
    public:
#pragma auto_inline(on)
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
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

template void vector<CShoveHaPennyRegionDef>::_M_fill_insert(CShoveHaPennyRegionDef*, unsigned int,
                                                             const CShoveHaPennyRegionDef&);
}