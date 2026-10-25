// Test for 0x005BAF28 std::__uninitialized_copy<const CCompositeBlendDesc*, CCompositeBlendDesc*>: the source object must
// run the copy constructor once per record of [first, last), in order, into result.., and return result + n.
#include <stdio.h>
#include <string.h>
class CCompositeBlendDesc {
public:
    CCompositeBlendDesc(const CCompositeBlendDesc& other);
    unsigned char _body[0x10];
};
static int g_copies;
static const void* g_from[8];
CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& other)
{
    if (g_copies < 8) g_from[g_copies] = &other;
    ++g_copies;
    memcpy(_body, other._body, sizeof(_body));
}
namespace std {
struct __false_type { __false_type() { } };
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
}
static unsigned char g_src[3][16], g_dst[3][16];
int main()
{
    int i, k;
    for (i = 0; i < 3; ++i) for (k = 0; k < 16; ++k) g_src[i][k] = (unsigned char)(i * 16 + k + 1);
    const CCompositeBlendDesc* s = (const CCompositeBlendDesc*)g_src;
    CCompositeBlendDesc* d = (CCompositeBlendDesc*)g_dst;
    std::__false_type tag;
    CCompositeBlendDesc* e = std::__uninitialized_copy(s, s + 3, d, tag);
    bool ok = e == d + 3 && g_copies == 3 && g_from[0] == s && g_from[1] == s + 1 && g_from[2] == s + 2
        && memcmp(g_src, g_dst, sizeof(g_src)) == 0;
    if (std::__uninitialized_copy(s, s, d, tag) != d || g_copies != 3) ok = false;
    if (!ok) { puts("005baf28_TEST FAIL"); return 1; }
    puts("005baf28_TEST PASS");
    return 0;
}
