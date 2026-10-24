// vector<pair<unsigned long, CAnimationEntry> >::reserve(unsigned int n) -- STLport vector reserve for the CRC-keyed animation entries: when the capacity is short, copy into a fresh malloc block, destroy and free the old one
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- same-TU auto_inline(off) noinline bodies for __uninitialized_copy and __destroy_aux (tags unnamed) home both __false_type temporaries on n's live argument slot
// Leaf: ?reserve@?$vector@U?$pair@KUCAnimationEntry@@@@@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2B30, 226 bytes (+ padding to 240), `ret 4` (__fastcall).
// Callees: 0x00BFEA0E malloc, 0x005E16F0 __uninitialized_copy(first, last, result, __false_type) (landed), 0x00454404
//   __destroy_aux(first, last, __false_type) (ICF-shared body; manifest `_Destroy_range<allocator<CFlash>>`), 0x00BFEA14 free.
// Callers: 0x005E3976 (the CVectorMap<unsigned long, CAnimationEntry> of CAnimationSet).
// Shape: STLport `if (capacity() < n) { old_size = size(); if (_M_start) { tmp = _M_allocate_and_copy(n, _M_start,
//   _M_finish); _M_clear(); } else tmp = _M_end_of_storage.allocate(n); _M_set(tmp, tmp + old_size, tmp + n); }` with
//   the malloc allocator (`n != 0 ? malloc(n * 20) : 0`, `if (p) free(p)`) and both helpers inlined; the two empty
//   `__false_type` tag temporaries are homed in n's argument slot (`lea edx,[esp+18h]`) although n is read again for
//   _M_set, and tmp is spilled to the `push ecx` frame word across the copy/destroy calls.
// Label: catalogued as `std_Vector_Reserve`.
#include <stdlib.h>
#include <new>
struct CAnimComponentMap {                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> struct CIVCountedPointer {              // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct CDefString {                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

struct CAnimationEntry {                                   // Ego_r.pdb, 0x10 bytes
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
};

template <class T1, class T2> struct pair {                // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, CAnimationEntry> SAnimationEntryPair;   // 0x14 bytes: CRC key + entry

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> TAnimationEntryLess;

struct __false_type { __false_type() { } };                // user constructor: no zero store into the tag byte

// Same-TU stand-ins for the out-of-line /Gr callees (GOTCHAS empty-tag rule): real bodies, never inlined, the tag
// parameter unnamed -- VC7.1 then homes both __false_type temporaries on n's argument slot as retail.
#pragma auto_inline(off)
__declspec(noinline) SAnimationEntryPair* __fastcall __uninitialized_copy(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result, const __false_type&)   // 0x005E16F0
{
    for (; first != last; ++first, ++result)
        new (result) SAnimationEntryPair(*first);
    return result;
}
__declspec(noinline) void __fastcall __destroy_aux(SAnimationEntryPair* first, SAnimationEntryPair* last, const __false_type&)   // 0x00454404
{
    for (; first != last; ++first)
        first->~SAnimationEntryPair();
}
#pragma auto_inline(on)

template <class T> struct allocator {                      // STLport allocator over malloc / free
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> inline void _Destroy(T* first, T* last)
{
    __destroy_aux(first, last, __false_type());
}

template <class T> struct vector {                         // STLport vector: {start, finish, end of storage}
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    T* _M_end_of_storage;                                  // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    unsigned int capacity() const { return unsigned int(_M_end_of_storage - _M_start); }
    T* _M_allocate_and_copy(unsigned int n, T* first, T* last)
    {
        T* result = allocator<T>().allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        allocator<T>().deallocate(_M_start, _M_end_of_storage - _M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void reserve(unsigned int n);
};

template <class T> void vector<T>::reserve(unsigned int n)
{
    if (capacity() < n) {
        const unsigned int old_size = size();
        T* tmp;
        if (_M_start) {
            tmp = _M_allocate_and_copy(n, _M_start, _M_finish);
            _M_clear();
        }
        else
            tmp = allocator<T>().allocate(n);
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}

template struct vector<SAnimationEntryPair>;