// std::vector<CShoveHaPennyRegionDef>::_M_insert_overflow(CShoveHaPennyRegionDef* position, const CShoveHaPennyRegionDef& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the shove-ha'penny region defs: allocate old_size + max(old_size, fill_len), copy the head, construct the fill_len copies of x, copy the tail unless inserting at the end, destroy and free the old elements, adopt the new block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D23B0, 277 bytes, thiscall `ret 14h`.
// Identity: the overflow path of vector<CShoveHaPennyRegionDef>::_M_fill_insert 0x007D24F0 (`call 0x007D23B0` with
//   `push 0 ; push n ; push &tag ; push x ; push position`); CTavernGameShoveHaPennyDef::Regions. The catalog's
//   "vector_AllocateAndCopy_CBrainUpdateZone" is a propagated label.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x007D22F0 (__fastcall,
//   `ret 8`, both copies out of line), CDefClassBase(const CDefClassBase&) 0x0046644C (the inline implicit copy
//   constructor of the fill_len == 1 _Construct), __uninitialized_fill_n(first, n, x, const __false_type&) 0x007D2370
//   (__fastcall, `ret 8`), the element's virtual destructor (slot 0, flag 0), free 0x00BFEA14.
// Shape: STLport 4.6 `_M_insert_overflow` verbatim: `(max)(__old_size, __fill_len)` by reference (the old size in the
//   `push ecx` frame word), allocate inline (`n != 0 ? malloc(n * 0x30) : 0`, homed on the dead fill_len slot), the
//   empty tags on the dead atend slot, `if (__fill_len == 1) { _Construct; ++ } else __uninitialized_fill_n`,
//   `if (!__atend)` tail copy, _M_clear (the _Destroy loop's `p->~T()` is the virtual deleting-destructor call with 0,
//   then `if (p) free(p)`), _M_set.
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

inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

// The two out-of-line helpers with their real bodies in the TU (`auto_inline(off)` + noinline, tag unnamed and never
// touched): VC7.1 then sees that the caller's address-taken words do not escape and homes the new block on the dead
// fill_len slot as retail does (declared-only, it gets its own frame word).
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
#pragma auto_inline(on)

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
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
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
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
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

template void vector<CShoveHaPennyRegionDef>::_M_insert_overflow(CShoveHaPennyRegionDef*,
                                                                 const CShoveHaPennyRegionDef&, const __false_type&,
                                                                 unsigned int, bool);
}