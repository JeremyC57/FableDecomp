// CMorphVectorMap::UpdateMorphArray(const CArray<CEntry>& entries) -- rebuild the keyed morph map (pair<key, entry>) from an entry array and re-sort it when dirty
// Grade: DIFFER(ours v retail) -- 340v346: retail loads the entry key once (a in ebx, b in eax plus a frame copy, frame 0x28) and walks entries by byte offset; ours reloads each key field per store (frame 0x24); the overflow tag now homes in the arg slot
// Leaf: UpdateMorphArray
#include <new>
struct __false_type { __false_type() { } };
struct SMorphValue { long a, b, c, d; };                      // 16-byte element
#pragma auto_inline(off)
// Same-TU stand-in for __uninitialized_copy 0x005BAF28 (it never reads the tag).
__declspec(noinline) SMorphValue* __fastcall UninitializedCopy(const SMorphValue* first, const SMorphValue* last, SMorphValue* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}
#pragma auto_inline(on)
struct CMorphValueBase {
    SMorphValue* _M_start; SMorphValue* _M_finish; SMorphValue* _M_end_of_storage;
    CMorphValueBase(unsigned long n)
    {
        _M_start = n ? (SMorphValue*)operator new(n * sizeof(SMorphValue)) : 0;
        _M_end_of_storage = _M_start + n;
    }
    ~CMorphValueBase() { if (_M_start) operator delete(_M_start); }
};
struct CMorphValueArray : CMorphValueBase {                  // STLport vector<SMorphValue>
    unsigned long size() const { return _M_finish - _M_start; }
    CMorphValueArray(const CMorphValueArray& x);                                // 0x005BB346 out of line
    CMorphValueArray(const SMorphValue* first, const SMorphValue* last) : CMorphValueBase(last - first)
    {
        _M_finish = UninitializedCopy(first, last, _M_start, __false_type());
    }
};
struct CMorphKey { long A; long B; };
struct CMorphEntry {                                         // 0x14 bytes
    CMorphKey        Key;
    CMorphValueArray Values;
    CMorphEntry(const CMorphKey& key, const CMorphValueArray& values) : Key(key), Values(values._M_start, values._M_finish) { }
};
struct CMorphPair {                                          // 0x1c bytes
    CMorphKey   first;
    CMorphEntry second;
    CMorphPair(const CMorphKey& k, const CMorphEntry& v) : first(k), second(v.Key, v.Values) { }
};
template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
struct CKeyPairCompareLess { };
void __fastcall SortMorphs(CMorphPair* first, CMorphPair* last, CKeyPairCompareLess compare);   // 0x00BEFC20
template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
};
struct CMorphVectorMap {
    CMorphPair* _M_start; CMorphPair* _M_finish; CMorphPair* _M_end_of_storage;
    CKeyPairCompareLess Compare;                              // +0x0c
    bool                Dirty;                                // +0x0d
    CMorphPair* erase(CMorphPair* first, CMorphPair* last);  // 0x00BEF510
    void _M_insert_overflow(CMorphPair* pos, const CMorphPair& x, const __false_type&, unsigned long n, bool atEnd);   // 0x00BEF830
    void push_back(const CMorphPair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    void UpdateMorphArray(const CArray<CMorphEntry>& entries);
};
#pragma auto_inline(off)
// Same-TU stand-ins (out of line in retail): VC7.1 sees they never read the tag argument.
__declspec(noinline) void CMorphVectorMap::_M_insert_overflow(CMorphPair* pos, const CMorphPair& x, const __false_type&, unsigned long n, bool atEnd)
{
    unsigned long size = _M_finish - _M_start;
    unsigned long len = size + (size > n ? size : n);
    CMorphPair* start = (CMorphPair*)operator new(len * sizeof(CMorphPair));
    CMorphPair* finish = start;
    for (CMorphPair* p = _M_start; p != pos; ++p, ++finish)
        _Construct(finish, *p);
    for (unsigned long i = 0; i < n; ++i, ++finish)
        _Construct(finish, x);
    if (!atEnd)
        for (CMorphPair* p = pos; p != _M_finish; ++p, ++finish)
            _Construct(finish, *p);
    for (CMorphPair* p = _M_start; p != _M_finish; ++p)
        p->~CMorphPair();
    if (_M_start)
        operator delete(_M_start);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage = start + len;
}
#pragma auto_inline(on)
void CMorphVectorMap::UpdateMorphArray(const CArray<CMorphEntry>& entries)
{
    long count = entries.size();
    erase(_M_start, _M_finish);
    for (unsigned long i = 0; i < (unsigned long)count; ++i) {
        push_back(CMorphPair(entries[i].Key, entries[i]));
        Dirty = true;
    }
    if (Dirty) {
        SortMorphs(_M_start, _M_finish, Compare);
        Dirty = false;
    }
}
