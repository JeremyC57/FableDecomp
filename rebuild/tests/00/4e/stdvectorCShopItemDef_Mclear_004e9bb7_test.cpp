// Behaviour test for std::vector<CShopItemDef>::_M_clear (0x004E9BB7): __destroy_aux gets [_M_start, _M_finish) and the block is
// freed; an empty (null) vector destroys an empty range and frees nothing.
#include <cstdio>
#include <cstdlib>

static int g_calls = 0;
static const void* g_first = 0;
static const void* g_last = 0;

class CShopItemDef {
public:
    virtual ~CShopItemDef();
    unsigned char _body[0x44];
};

namespace std {
struct __false_type { __false_type() { } };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <> void __fastcall __destroy_aux<CShopItemDef*>(CShopItemDef* __first, CShopItemDef* __last, const __false_type&)
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
    std::vector<CShopItemDef> v;
    v._M_start = (CShopItemDef*)std::malloc(3 * sizeof(CShopItemDef));
    v._M_finish = v._M_start + 2;
    v._M_end_of_storage = v._M_start + 3;
    CShopItemDef* start = v._M_start;
    v._M_clear();
    if (g_calls != 1 || g_first != start || g_last != start + 2 || sizeof(CShopItemDef) != 0x48) {
        std::printf("004e9bb7_TEST FAIL\n"); return 1;
    }
    std::vector<CShopItemDef> e;
    e._M_start = e._M_finish = e._M_end_of_storage = 0;
    e._M_clear();
    if (g_calls != 2 || g_first != 0 || g_last != 0) { std::printf("004e9bb7_TEST FAIL empty\n"); return 1; }
    std::printf("004e9bb7_TEST PASS\n");
    return 0;
}
