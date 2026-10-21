// test for 00433504 uninitialized_fill_n<CCharString*>
#include <stdio.h>
#include <new>
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& s);                   // 0x0099EC30
};
namespace std {
struct __false_type {};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class ForwardIter, class Size, class T>
__forceinline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}
template CCharString* __fastcall uninitialized_fill_n<CCharString*, unsigned int, CCharString>(CCharString*, unsigned int, const CCharString&);
}

static int g_copies;
CCharString::CCharString(const CCharString& s) : PData(s.PData) { ++g_copies; }
int main()
{
    static void* buf[6];
    for (int i = 0; i < 6; ++i) buf[i] = (void*)0x99;
    CCharString* first = (CCharString*)buf;
    CCharString x = *(CCharString*)&buf[5];
    x.PData = (void*)0x1234; g_copies = 0;
    CCharString* end = std::uninitialized_fill_n(first, 4u, x);
    int ok = end == first + 4 && g_copies == 4 && buf[0] == (void*)0x1234 && buf[3] == (void*)0x1234 && buf[4] == (void*)0x99;
    ok &= std::uninitialized_fill_n(first, 0u, x) == first && g_copies == 4;
    printf(ok ? "00433504_TEST PASS\n" : "00433504_TEST FAIL\n");
    return ok ? 0 : 1;
}