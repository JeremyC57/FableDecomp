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

class CMapInfo { public:                                       // 0x48 bytes
    unsigned char _body[0x48];
};
struct forward_iterator_tag { forward_iterator_tag() { } };
struct __false_type { __false_type() { } };

void __fastcall DestroyRange(CMapInfo* first, CMapInfo* last, const __false_type& tag);   // 0x00515250

template<class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void reserve(unsigned long n);                                                         // 0x00519800
    void _M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag);      // 0x00519CD0
    void insert(T* pos, T* first, T* last) { _M_range_insert(pos, first, last, forward_iterator_tag()); }
    void DeallocateMemory();                                                               // 0x0051A350
    void DeallocateUnusedMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<CMapInfo>::DeallocateUnusedMemory()
{
    if (capacity() > size()) {
        CArray<CMapInfo> temp;
        temp.reserve(size());
        temp.insert(temp.begin(), begin(), end());
        DeallocateMemory();
        CMapInfo* p = Exchange(_M_start, temp._M_start);
        CMapInfo* l = Exchange(_M_finish, temp._M_finish);
        _M_end_of_storage = temp._M_end_of_storage;
        DestroyRange(p, l, __false_type());
        if (p)
            TestFree(p);
    }
}


void __fastcall DestroyRange(CMapInfo* first, CMapInfo* last, const __false_type& tag) { g_destroyed += (int)(last - first); }
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
    CArray<CMapInfo> a;
    a._M_start = (CMapInfo*)std::malloc(6 * sizeof(CMapInfo));
    std::memset(a._M_start, 0, 6 * sizeof(CMapInfo));
    for (int i = 0; i < 2; ++i)
        std::memset(&a._M_start[i], 0x11 * (i + 1), sizeof(CMapInfo));
    a._M_finish = a._M_start + 2;
    a._M_end_of_storage = a._M_start + 6;
    a.DeallocateUnusedMemory();
    unsigned char want0[sizeof(CMapInfo)], want1[sizeof(CMapInfo)];
    std::memset(want0, 0x11, sizeof(CMapInfo)); std::memset(want1, 0x22, sizeof(CMapInfo));
    if (a.size() != 2 || a.capacity() != 2 || std::memcmp(&a._M_start[0], want0, sizeof(CMapInfo)) != 0 ||
        std::memcmp(&a._M_start[1], want1, sizeof(CMapInfo)) != 0 || g_freed != 1) { std::printf("0051b560_TEST FAIL\n"); return 1; }
    a.DeallocateUnusedMemory();                 // already tight: untouched
    if (a.capacity() != 2 || g_freed != 1) { std::printf("0051b560_TEST FAIL\n"); return 1; }
    std::printf("0051b560_TEST PASS\n");
    return 0;
}