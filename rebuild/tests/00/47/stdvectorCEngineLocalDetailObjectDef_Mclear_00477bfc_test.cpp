// Behaviour test for 0x00477BFC vector<CEngineLocalDetailObjectDef>::_M_clear: destroys every element through the range
// destroy, then frees the block; a null block destroys an empty range and frees nothing.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dtors = 0;
static int g_frees = 0;
class CEngineLocalDetailObjectDef {
public:
    virtual ~CEngineLocalDetailObjectDef() { ++g_dtors; }
    unsigned char _body[0x84];
};
static void TestFree(void* p) { ++g_frees; std::free(p); }
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) TestFree(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _ForwardIterator> void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{ for ( ; __first != __last; ++__first) (&*__first)->~CEngineLocalDetailObjectDef(); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start; _Tp* _M_finish; _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
int main()
{
    std::vector<CEngineLocalDetailObjectDef> v;
    v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    v._M_clear();
    if (g_dtors != 0 || g_frees != 0) { std::printf("00477bfc_TEST FAIL\n"); return 1; }
    v._M_start = (CEngineLocalDetailObjectDef*)std::malloc(3 * sizeof(CEngineLocalDetailObjectDef));
    for (int i = 0; i < 2; ++i) new (v._M_start + i) CEngineLocalDetailObjectDef();
    v._M_finish = v._M_start + 2;
    v._M_end_of_storage._M_data = v._M_start + 3;
    v._M_clear();
    if (g_dtors != 2 || g_frees != 1 || sizeof(CEngineLocalDetailObjectDef) != 0x88) { std::printf("00477bfc_TEST FAIL\n"); return 1; }
    std::printf("00477bfc_TEST PASS\n");
    return 0;
}
