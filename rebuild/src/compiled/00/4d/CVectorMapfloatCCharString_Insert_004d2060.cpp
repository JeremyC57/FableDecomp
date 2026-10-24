// CVectorMap<float,CCharString>::Insert(const float& key, const CCharString& value) -- the CSubtitledMessage text map: append a {key, text} pair to the STLport vector at +0 (in place when there is room, else through _M_insert_overflow), destroy the pair temporary and mark the map dirty (+0xd, re-sorted on the next lookup)
// Leaf: Insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2060, 130 bytes, `ret 8`. No direct callers (an unreferenced COMDAT
//   copy of the instantiation).
// Callees: CCharString copy constructor 0x0099EC30, ~CCharString 0x0099EAE0,
//   vector<pair<float,CCharString>>::_M_insert_overflow(pos, x, const __false_type&, 1, true) 0x004D1940 (thiscall,
//   `ret 14h`; catalogued as the descriptive `Vector_Reserve_And_Assign`).
// Label correction: the catalog's `std_vector_push_back_EHeroMorphType_CParticleMorphs_CEntry` is propagated; Ego_r.pdb
//   names the member `?Insert@?$CVectorMap@M...@@QAEXABM...@Z` (the EConversationAttitude twin) and the element is the
//   8-byte pair<float, CCharString> of CVectorMap<float,CCharString,CKeyPairCompareLess<float,CCharString>> (key dword +
//   CCharString copy, as in the same-TU __uninitialized_copy 0x004CF9D0). Same family as the landed CVectorMap Add rows
//   (0x006FCF40: vector +0, empty Compare +0xc, Dirty +0xd).
// Shape: `Vector.push_back(value_type(key, value)); Dirty = true;` with STLport push_back inlined: the pair temporary
//   (key dword, CCharString copy) is built first, the room test re-reads finish after the copy constructor, the in-place
//   construct sits behind the placement-new null test, the full case passes a fresh `__false_type()` tag homed in the
//   dead `key` argument slot; the temporary's ~CCharString and the Dirty store are tail-duplicated into both arms.
#include <stddef.h>

inline void* operator new(size_t, void* where) { return where; }

struct CCharString {                            // 4 bytes: the string-data pointer
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

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
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_t fill_len, bool atend);   // 0x004D1940
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

template void CVectorMap<float, CCharString>::Insert(const float&, const CCharString&);