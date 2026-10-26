// Behaviour test for 0x005E2B30 vector<pair<unsigned long, CAnimationEntry> >::reserve(n): when n exceeds the capacity,
// copy the entries into a fresh block (component references taken), destroy the old ones (references dropped), free it.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <stdlib.h>
#include <new>
class CAnimComponentMap { public:                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer { public:              // one word: the intrusively counted object
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

class CDefString { public:                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

class CAnimationEntry { public:                                   // Ego_r.pdb, 0x10 bytes
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

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> TAnimationEntryLess;

struct __false_type { __false_type() { } };                // user constructor: no zero store into the tag byte

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
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
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



static int g_deletes = 0;
void CAnimComponentMap::v00() { }
void CAnimComponentMap::Delete() { ++g_deletes; }

int main()
{
    CAnimComponentMap maps[2];
    maps[0].IVRefCount = 1; maps[1].IVRefCount = 1;
    vector<SAnimationEntryPair> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    int ok = 1;
    v.reserve(2);
    if (v.capacity() != 2 || v.size() != 0 || !v._M_start) ok = 0;
    for (int k = 0; k < 2; ++k) {
        SAnimationEntryPair* e = v._M_finish;
        e->first = 0x1000 + k; e->second.BankIndex = k; e->second.AnimName.TablePos = 10 + k;
        e->second.GroupName.TablePos = 20 + k; e->second.Components.Object = &maps[k];
        ++v._M_finish;
    }
    SAnimationEntryPair* old = v._M_start;
    v.reserve(2);                                           // enough already: unchanged
    if (v._M_start != old) ok = 0;
    v.reserve(6);
    if (v.capacity() != 6 || v.size() != 2 || v._M_start == old) ok = 0;
    for (int k = 0; k < 2 && ok; ++k) {
        SAnimationEntryPair& e = v._M_start[k];
        if (e.first != 0x1000u + k || e.second.BankIndex != k || e.second.AnimName.TablePos != 10 + k ||
            e.second.GroupName.TablePos != 20 + k || e.second.Components.Object != &maps[k]) ok = 0;
    }
    if (maps[0].IVRefCount != 1 || maps[1].IVRefCount != 1 || g_deletes != 0) ok = 0;   // copy +1, destroy -1
    printf(ok ? "005e2b30_TEST PASS\n" : "005e2b30_TEST FAIL\n");
    return ok ? 0 : 1;
}