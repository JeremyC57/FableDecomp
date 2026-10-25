// std::vector::_M_insert_overflow(pair<NHitLocations::CHitLocation,long>* position, const pair<NHitLocations::CHitLocation,long>& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the (hit location, index) list: allocate old_size + max(old_size, fill_len), copy the head, construct the fill_len copies of x, copy the tail unless inserting at the end, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00818CA0, 288 bytes, thiscall `ret 14h`.
// Identity: the only caller is push_back 0x00818DC0 (manifest: vector<pair<NHitLocations::CHitLocation,long>>::push_back,
//   whose inline copy is the same 28-byte memberwise copy as the fill_len == 1 arm here). The catalog's
//   "std_vector_InsertAndMove" is descriptive.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x00818180 (__fastcall, `ret 8`, head and tail),
//   __uninitialized_fill_n 0x008181E0, __destroy_aux(first, last, const __false_type&) 0x004E1C5C (COMDAT-folded with
//   the pair<unsigned long,CAnimationEntry> instantiation; it calls the out-of-line element dtor), free 0x00BFEA14.
// Layout: pair<CHitLocation,long> = 28 bytes (0x92492493 reciprocal, `sar 4`): CHitLocation (Ego_r.pdb, 24 bytes:
//   POwnerThing, PDef, PArmourDef -- two ref-counted CDefPointers whose copy bumps the count at +4 --, the two action
//   types, Enabled) then the long.
// Shape: STLport 4.6 `_M_insert_overflow` verbatim; the element's implicit copy constructor inlines in the
//   fill_len == 1 arm, `_M_clear`'s _Destroy goes through `__destroy` to the out-of-line `__destroy_aux` (same-TU
//   noinline body; its tag temporary shares the `atend` slot with the copy tags).
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

class CThing;
class CHitLocationDef;
class CArmourDef;
enum EActionRegisteredType { ACTION_REGISTERED_TYPE_NONE = 0 };

template <class T> class CDefPointer {               // ref-counted definition pointer (count at +4 of the def)
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++*(long*)((char*)Object + 4);
    }
    ~CDefPointer();
};

namespace NHitLocations {
class CHitLocation {                                 // Ego_r.pdb, 24 bytes
public:
    CThing*                           POwnerThing;           // +0x00
    CDefPointer<const CHitLocationDef> PDef;                 // +0x04
    CDefPointer<const CArmourDef>      PArmourDef;           // +0x08
    EActionRegisteredType             HitResponseAction;     // +0x0c
    EActionRegisteredType             BlockResponseAction;   // +0x10
    bool                              Enabled;               // +0x14
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
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

typedef pair<NHitLocations::CHitLocation, long> THitLocationEntry;
template void vector<THitLocationEntry>::_M_insert_overflow(THitLocationEntry*, const THitLocationEntry&,
                                                            const __false_type&, unsigned int, bool);
}