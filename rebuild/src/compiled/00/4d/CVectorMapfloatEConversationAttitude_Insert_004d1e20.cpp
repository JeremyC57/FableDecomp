// CVectorMap<float,EConversationAttitude>::Insert(const float& key, const EConversationAttitude& value) -- the CSubtitledMessage attitude map: append a {key, attitude} pair to the STLport vector at +0 (in place when there is room, else through _M_insert_overflow) and mark the map dirty (+0xd, re-sorted on the next lookup)
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- a same-TU noinline _M_insert_overflow stand-in (0x004D1250, tag unnamed) restores retail's register choice and drops the frame re-read of the value word (101 -> 95)
// Leaf: ?Insert@?$CVectorMap@MW4EConversationAttitude@@@@QAEXABMABW4EConversationAttitude@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D1E20, 95 bytes, `ret 8`. No direct callers (an unreferenced COMDAT
//   copy of the instantiation).
// Callee: vector<pair<float,EConversationAttitude>>::_M_insert_overflow(pos, x, const __false_type&, 1, true)
//   0x004D1250 (thiscall, `ret 14h`; malloc/free only = the POD 8-byte element; catalogued as `std::vector_Realloc_And_Copy`).
// Label correction: the catalog's `std_vector_push_back_pair_64bit_v2` is descriptive; Ego_r.pdb names the member
//   `?Insert@?$CVectorMap@MW4EConversationAttitude@@...@@QAEXABMABW4EConversationAttitude@@@Z`. Same family as the landed CVectorMap Add rows
//   (0x006FCF40: vector +0, empty Compare +0xc, Dirty +0xd).
// Shape: `Vector.push_back(value_type(key, value)); Dirty = true;` with STLport push_back inlined: the pair temporary
//   (key and value words) is built in the frame first, the in-place construct sits behind the placement-new null test
//   and bumps finish with a memory `add`, the full case passes a fresh `__false_type()` tag homed in the dead `key`
//   argument slot; the Dirty store is tail-duplicated into both arms. Tried: pair temporary, make_pair, a trivial
//   user ~pair() -- all give the same 101-byte allocation.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);          // 0x00BFEA14

inline void* operator new(size_t, void* where) { return where; }

enum EConversationAttitude { ATTITUDE_NONE };

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {     // STLport pair
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class T> struct allocator { allocator() { } };

template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};

template <class T> struct vector {              // STLport vector (12 bytes in retail)
    T*                   _M_start;              // +0
    T*                   _M_finish;             // +4
    _STLP_alloc_proxy<T> _M_end_of_storage;     // +8
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_t fill_len, bool atend);   // 0x004D1250
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

// _M_insert_overflow @ 0x004D1250: a same-TU stand-in with the STLport reallocation shape (GOTCHAS empty-tag rule:
// the callee that receives the empty __false_type gets a body, never inlined, the tag unnamed).
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, size_t fill_len, bool atend)
{
    size_t old_size = _M_finish - _M_start;
    size_t len = old_size + (old_size > fill_len ? old_size : fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != position; ++p, ++new_finish)
        _Construct(new_finish, *p);
    for (size_t i = 0; i < fill_len; ++i, ++new_finish)
        _Construct(new_finish, x);
    if (!atend)
        for (T* p = position; p != _M_finish; ++p, ++new_finish)
            _Construct(new_finish, *p);
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage._M_data = new_start + len;
}
#pragma auto_inline(on)

template <class K, class V> struct CKeyPairCompareLess { };   // empty

template <class K, class V> struct CVectorMap {
    vector<pair<K, V> >       Vector;           // +0x00
    CKeyPairCompareLess<K, V> Compare;          // +0x0c
    bool                      Dirty;            // +0x0d
    void Insert(const K& key, const V& value)
    {
        Vector.push_back(pair<K, V>(key, value));
        Dirty = true;
    }
};

template void CVectorMap<float, EConversationAttitude>::Insert(const float&, const EConversationAttitude&);