// Behaviour test for 00c29840 (Dinkumware std::vector<CHandleBasedMemoryPool_012b4098::CHandleInfo>::_Ufill): count copies of the value
// are constructed at ptr through _Uninitialized_fill_n / _Uninit_fill_n, and ptr + count comes back.
#include <cstdio>
#include <new>
struct CHeader;
struct CHandleInfo {
    union { CHeader* Header; long NextFreeHandle; };
};
struct _Nonscalar_ptr_iterator_tag { };
template <class _T1, class _T2> inline _Nonscalar_ptr_iterator_tag _Ptr_cat(_T1&, _T2&)
{
    _Nonscalar_ptr_iterator_tag _Cat;
    return (_Cat);
}
template <class _Ty> struct allocator { };
static int g_calls = 0;
template <class _FwdIt, class _Diff, class _Tval, class _Alloc>
void _Uninit_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc&, _Nonscalar_ptr_iterator_tag)   // stands in for 0x00C294B0
{
    ++g_calls;
    for (; 0 < _Count; --_Count, ++_First)
        new ((void*)&*_First) _Tval(_Val);
}
template <class _FwdIt, class _Diff, class _Tval, class _Alloc>
inline void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al)
{
    _Uninit_fill_n(_First, _Count, _Val, _Al, _Ptr_cat(_First, _First));
}
struct CHandleInfoVector {
    allocator<CHandleInfo> _Alval;
    CHandleInfo* _Myfirst;
    CHandleInfo* _Mylast;
    CHandleInfo* _Myend;
    CHandleInfo* _Ufill(CHandleInfo* _Ptr, unsigned int _Count, const CHandleInfo& _Val)
    {
        _Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval);
        return (_Ptr + _Count);
    }
};
#define FAIL() { std::printf("00c29840_TEST FAIL\n"); return 1; }
int main()
{
    CHandleInfo buf[6];
    for (int i = 0; i < 6; ++i) buf[i].NextFreeHandle = -1;
    CHandleInfoVector v;
    v._Myfirst = v._Mylast = buf;
    v._Myend = buf + 6;
    CHandleInfo x;
    x.NextFreeHandle = 42;
    if (v._Ufill(buf, 0u, x) != buf || buf[0].NextFreeHandle != -1 || g_calls != 1) FAIL();
    CHandleInfo* r = v._Ufill(buf + 1, 4u, x);
    if (r != buf + 5 || g_calls != 2) FAIL();
    for (int i = 1; i < 5; ++i)
        if (buf[i].NextFreeHandle != 42) FAIL();
    if (buf[0].NextFreeHandle != -1 || buf[5].NextFreeHandle != -1) FAIL();
    std::printf("00c29840_TEST PASS\n");
    return 0;
}