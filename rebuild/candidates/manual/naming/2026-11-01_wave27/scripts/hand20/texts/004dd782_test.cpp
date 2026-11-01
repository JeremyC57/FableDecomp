// behaviour test of std::__destroy_aux<std::pair<long, CCharString>*>(first, last, const __false_type&) 0x004DD782: every element of [first, last)
// is destroyed through the pair's scalar deleting destructor (0x004DA68A, modelled here) with flag 0; an empty range destroys nothing.
#include <stdio.h>
static int g_calls = 0; static void* g_last = 0; static unsigned int g_lastFlags = 99;
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
}
CCharString::~CCharString() { ++g_calls; g_last = this; g_lastFlags = 0; }
typedef std::pair<long, CCharString> Item;
int main()
{
    Item buf[4];
    int ok = 1;
    std::__destroy_aux(&buf[0], &buf[3], std::__false_type());
    if (g_calls != 3 || g_last != &buf[2].second || g_lastFlags != 0) ok = 0;
    g_calls = 0;
    std::__destroy_aux(&buf[0], &buf[0], std::__false_type());
    if (g_calls != 0) ok = 0;
    if (sizeof(Item) != 8) ok = 0;
    printf(ok ? "DESTROY_RANGE_FLASH_OK\n" : "DESTROY_RANGE_FLASH_FAIL\n");
    return ok ? 0 : 1;
}
