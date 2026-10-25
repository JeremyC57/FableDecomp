// std::vector::_M_insert_overflow(pair<EBodyLocation,CWoundMorphs::CEntry>* position, const pair<EBodyLocation,CWoundMorphs::CEntry>& x, const __false_type&, unsigned int fill_len, bool atend) -- grow CWoundMorphs' body-location map vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759190, 249 bytes, `ret 0x14`.
// Label correction: catalog "Vector_InsertRange" -> STLport 4.x
//   vector<pair<EBodyLocation, CWoundMorphs::CEntry> >::_M_insert_overflow (the landed CVectorMap::Add 0x00759570 names
//   it; 44-byte element: 0x2E8BA2E9 / `sar edx,3`, `imul 2Ch`).
// Element: pair<EBodyLocation, CWoundMorphs::CEntry (40-byte POD)> with STLport pair's user copy constructor (the key
//   on its own, then the entry as a 10-dword `rep movsd` block); trivially destructible, so _M_clear is only the free.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x007586E0, __uninitialized_fill_n 0x00758720.
// Shape: STLport `_M_insert_overflow` (see 0x006FC6A0); the block copy needs esi/edi/ecx, so new_start and len live in
//   two frame words (`sub esp,8`). The copy / fill helpers are same-TU noinline stand-ins (separate rows).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

enum EBodyLocation { EBodyLocation_FIRST };

class CWoundMorphs {
public:
    class CEntry {                              // Ego_r.pdb, 40 bytes
    public:
        EBodyLocation Location;                 // +0x00
        long  ReplacingTextureIndex;            // +0x04
        long  ReplacingBumpMapIndex;            // +0x08
        long  WoundTextureIndex;                // +0x0c
        long  WoundBumpMapIndex;                // +0x10
        long  ScarTextureIndex;                 // +0x14
        long  ScarBumpMapIndex;                 // +0x18
        long  FadeToScarDuration;               // +0x1c
        long  TotalDuration;                    // +0x20
        float PermanentScarStrength;            // +0x24
    };
};

namespace std {

struct __false_type { __false_type() {} };

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair& o) : first(o.first), second(o.second) {}
};

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x007586E0, 0x00758720).
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    for (; n > 0; --n, ++first)
        _Construct(first, x);
    return first;
}
#pragma auto_inline(on)

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    Alloc _M_alloc() const { return Alloc(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _M_alloc().deallocate(_M_start, (unsigned int)(_M_end_of_storage - _M_start));
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template<class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    T* new_start = _M_alloc().allocate(len);
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<pair<EBodyLocation, CWoundMorphs::CEntry> >;

}