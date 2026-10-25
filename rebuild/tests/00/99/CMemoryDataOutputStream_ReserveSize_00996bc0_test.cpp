#include <cstdio>
// Behaviour test for 0x00996BC0 CMemoryDataOutputStream::ReserveSize: links against the source object; vector<char>::
// reserve is defined here and records the vector it ran on and the requested size.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void reserve(unsigned int __n);
};
}
static void* g_vec = 0; static unsigned int g_n = 0; static int g_calls = 0;
template <> void std::vector<char, std::allocator<char> >::reserve(unsigned int __n) { g_vec = this; g_n = __n; ++g_calls; }
template <class T> class CArray : public std::vector<T> { };
class CMemoryDataOutputStream {
public:
    void*         _vftable;
    void*         NewLine;
    CArray<char>  Data;
    unsigned long CurrentPosition;
    void ReserveSize(unsigned long size);
};
int main()
{
    if (sizeof(CMemoryDataOutputStream) != 0x18 || (char*)&((CMemoryDataOutputStream*)0)->Data - (char*)0 != 8) { std::printf("00996bc0_TEST FAIL layout\n"); return 1; }
    CMemoryDataOutputStream s; s.CurrentPosition = 7;
    s.ReserveSize(0x8000);
    if (g_calls != 1 || g_vec != &s.Data || g_n != 0x8000 || s.CurrentPosition != 7) { std::printf("00996bc0_TEST FAIL reserve\n"); return 1; }
    s.ReserveSize(0x400);
    if (g_calls != 2 || g_n != 0x400) { std::printf("00996bc0_TEST FAIL second\n"); return 1; }
    std::printf("00996bc0_TEST PASS\n");
    return 0;
}
