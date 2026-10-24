#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
static int g_freed = 0;
static int g_destroyed = 0;
static void TestFree(void* p) { ++g_freed; std::free(p); }

struct SAnimationPair {                         // std::pair<unsigned long, CAnimationEntry>, 0x14 bytes
    unsigned long first;
    long          second[4];                    // CAnimationEntry (four words, landed 0x005DEDC0)
};
struct forward_iterator_tag { forward_iterator_tag() { } };
struct __false_type { __false_type() { } };

void __fastcall DestroyRange(SAnimationPair* first, SAnimationPair* last, const __false_type& tag);   // 0x00454404

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void reserve(unsigned long n);                                                         // 0x005E2B30
    void _M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag);      // 0x005E2C20
    void insert(T* pos, T* first, T* last) { _M_range_insert(pos, first, last, forward_iterator_tag()); }
    void DeallocateMemory();                                                               // 0x005E3170
    void DeallocateUnusedMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<SAnimationPair>::DeallocateUnusedMemory()
{
    if (capacity() > size()) {
        CArray<SAnimationPair> temp;
        temp.reserve(size());
        temp.insert(temp.begin(), begin(), end());
        DeallocateMemory();
        SAnimationPair* p = Exchange(_M_start, temp._M_start);
        SAnimationPair* l = Exchange(_M_finish, temp._M_finish);
        _M_end_of_storage = temp._M_end_of_storage;
        DestroyRange(p, l, __false_type());
        if (p)
            TestFree(p);
    }
}


void __fastcall DestroyRange(SAnimationPair* first, SAnimationPair* last, const __false_type& tag) { g_destroyed += (int)(last - first); }
template<class T> void CArray<T>::reserve(unsigned long n)
{
    _M_start = (T*)std::malloc(n * sizeof(T));
    _M_finish = _M_start;
    _M_end_of_storage = _M_start + n;
}
template<class T> void CArray<T>::_M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag)
{
    for (; first != last; ++first)
        *_M_finish++ = *first;
}
template<class T> void CArray<T>::DeallocateMemory()
{
    DestroyRange(_M_start, _M_finish, __false_type());
    TestFree(_M_start);
    _M_start = _M_finish = _M_end_of_storage = 0;
}
int main()
{
    CArray<SAnimationPair> a;
    a._M_start = (SAnimationPair*)std::malloc(6 * sizeof(SAnimationPair));
    std::memset(a._M_start, 0, 6 * sizeof(SAnimationPair));
    for (int i = 0; i < 2; ++i)
        std::memset(&a._M_start[i], 0x11 * (i + 1), sizeof(SAnimationPair));
    a._M_finish = a._M_start + 2;
    a._M_end_of_storage = a._M_start + 6;
    a.DeallocateUnusedMemory();
    unsigned char want0[sizeof(SAnimationPair)], want1[sizeof(SAnimationPair)];
    std::memset(want0, 0x11, sizeof(SAnimationPair)); std::memset(want1, 0x22, sizeof(SAnimationPair));
    if (a.size() != 2 || a.capacity() != 2 || std::memcmp(&a._M_start[0], want0, sizeof(SAnimationPair)) != 0 ||
        std::memcmp(&a._M_start[1], want1, sizeof(SAnimationPair)) != 0 || g_freed != 1) { std::printf("005e3920_TEST FAIL\n"); return 1; }
    a.DeallocateUnusedMemory();                 // already tight: untouched
    if (a.capacity() != 2 || g_freed != 1) { std::printf("005e3920_TEST FAIL\n"); return 1; }
    std::printf("005e3920_TEST PASS\n");
    return 0;
}