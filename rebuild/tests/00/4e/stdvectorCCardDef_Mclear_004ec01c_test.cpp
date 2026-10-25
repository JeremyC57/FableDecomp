// Behaviour test for std::vector<CCardDef>::_M_clear (0x004EC01C): __destroy_aux gets [_M_start, _M_finish) and the block is
// freed; an empty (null) vector destroys an empty range and frees nothing.
#include <cstdio>
#include <cstdlib>

static int g_calls = 0;
static const void* g_first = 0;
static const void* g_last = 0;

class CCardDef {
public:
    virtual ~CCardDef();
    unsigned char _body[0x40];
};

namespace std {
struct __false_type { __false_type() { } };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <> void __fastcall __destroy_aux<CCardDef*>(CCardDef* __first, CCardDef* __last, const __false_type&)
{
    ++g_calls; g_first = __first; g_last = __last;
}
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void _M_clear();
};
}

int main()
{
    std::vector<CCardDef> v;
    v._M_start = (CCardDef*)std::malloc(3 * sizeof(CCardDef));
    v._M_finish = v._M_start + 2;
    v._M_end_of_storage = v._M_start + 3;
    CCardDef* start = v._M_start;
    v._M_clear();
    if (g_calls != 1 || g_first != start || g_last != start + 2 || sizeof(CCardDef) != 0x44) {
        std::printf("004ec01c_TEST FAIL\n"); return 1;
    }
    std::vector<CCardDef> e;
    e._M_start = e._M_finish = e._M_end_of_storage = 0;
    e._M_clear();
    if (g_calls != 2 || g_first != 0 || g_last != 0) { std::printf("004ec01c_TEST FAIL empty\n"); return 1; }
    std::printf("004ec01c_TEST PASS\n");
    return 0;
}
