// Behaviour test for 0x00519EB0 __destroy_aux<CSeaBodyDesc*>: every element in [first, last) frees its non-null
// Maps block (null blocks are skipped) and then runs ~CCharString, element by element in order; an empty range does nothing.
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_log[16]; static int g_n;
static void* g_freed[8]; static int g_nf;
static void counted_free(void* p) { g_freed[g_nf++] = p; g_log[g_n++] = 100 + g_nf; std::free(p); }
class CCharString {
public:
    int Id;
    ~CCharString() { g_log[g_n++] = Id; }
};
struct __false_type {};
template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) counted_free(p); }
};
template <class T> class _Vector_base {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) allocator<T>().deallocate(_M_start, _M_end_of_storage - _M_start); }
};
template <class T> class vector : public _Vector_base<T> { public: ~vector() {} };
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T> class CArray : public vector<T> {};
struct CSeaBodyDesc { CCharString Identifier; CArray<long> Maps; };

int main()
{
    void* raw = std::malloc(3 * sizeof(CSeaBodyDesc));
    CSeaBodyDesc* a = (CSeaBodyDesc*)raw;
    long* b0 = (long*)std::malloc(8); long* b2 = (long*)std::malloc(12);
    a[0].Identifier.Id = 1; a[0].Maps._M_start = b0; a[0].Maps._M_finish = b0 + 2; a[0].Maps._M_end_of_storage = b0 + 2;
    a[1].Identifier.Id = 2; a[1].Maps._M_start = 0;  a[1].Maps._M_finish = 0;      a[1].Maps._M_end_of_storage = 0;
    a[2].Identifier.Id = 3; a[2].Maps._M_start = b2; a[2].Maps._M_finish = b2 + 1; a[2].Maps._M_end_of_storage = b2 + 3;
    __false_type tag;
    __destroy_aux(a, a, tag);
    int ok = g_n == 0;
    __destroy_aux(a, a + 3, tag);
    // expected: free(b0), ~1, ~2, free(b2), ~3
    ok &= g_n == 5 && g_log[0] == 101 && g_log[1] == 1 && g_log[2] == 2 && g_log[3] == 102 && g_log[4] == 3;
    ok &= g_nf == 2 && g_freed[0] == (void*)b0 && g_freed[1] == (void*)b2;
    std::free(raw);
    if (!ok) { std::printf("00519eb0_TEST FAIL\n"); return 1; }
    std::printf("00519eb0_TEST PASS\n");
    return 0;
}