// std::vector::_M_insert_overflow(pair<EHeroMorphType,CParticleMorphs::CEntry>* position, const pair<EHeroMorphType,CParticleMorphs::CEntry>& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for a hero-morph map's (morph type, entry) pairs: allocate old_size + max(old_size, fill_len), copy the head, construct the fill_len copies of x, copy the tail unless inserting at the end, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081C990, 256 bytes, thiscall `ret 14h`.
// Identity: the hero-morph particle map's vector (CParticleMorphs::Morphs, Ego_r.pdb CVectorMap<EHeroMorphType,
//   CParticleMorphs::CEntry>): callers are its CVectorMap::Add push_back paths 0x0081B490 / 0x0081D7D0-family
//   (0x0081B4E4, 0x0081D6BF, 0x0081D888) and the fill insert 0x0081CD00 (0x0081CE36); the head/tail copy 0x0081A270 and
//   the fill 0x0081A2B0 are this pair's helpers (catalogued under propagated _Cons_val names).
//   The catalog's "Vector_Reallocate" is descriptive.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x0081A270 (__fastcall, `ret 8`, head and tail), __uninitialized_fill_n
//   0x0081A2B0, free 0x00BFEA14.
// Layout: pair<EHeroMorphType, CParticleMorphs::CEntry> = 40 bytes (0x66666667 reciprocal, `sar 4`): the key word, then the
//   36-byte entry (copied as one `rep movsd` of 9 dwords).
// Shape: STLport 4.6 `_M_insert_overflow` verbatim: `(max)` by reference into the `push ecx` frame word, allocate
//   inline, the copies and the fill out of line (same-TU noinline bodies that never touch the tag, so all three tag
//   temporaries share the `atend` slot), the single-element pair copy inline (STLport's memberwise pair copy), the
//   trivial _Destroy folded away, `if (p) free(p)`, then _M_set.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum EMeshLocationPoint { MESH_LOCATION_POINT_NONE };
class CDefString {                                   // 4 bytes, an index into the def string table
public:
    long TablePos;
};

class CParticleMorphs {
public:
    class CEntry {                                   // Ego_r.pdb, 36 bytes, trivially copyable
    public:
        EHeroMorphType     MorphType;                // +0x00
        bool               Blending;                 // +0x04
        EMeshLocationPoint Location;                 // +0x08
        CDefString         LocationName;             // +0x0c
        long               LocationIndex;            // +0x10
        long               ParticleIndex;            // +0x14
        long               SecondParticleIndex;      // +0x18
        float              StartAtMorphStrength;     // +0x1c
        float              EndAtMorphStrength;       // +0x20
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator, _ForwardIterator) { }     // trivially destructible element: no loop

#pragma auto_inline(off)
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

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
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

typedef pair<EHeroMorphType, CParticleMorphs::CEntry> TMorphEntry;
template void vector<TMorphEntry>::_M_insert_overflow(TMorphEntry*, const TMorphEntry&, const __false_type&,
                                                      unsigned int, bool);
}