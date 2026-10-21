// test for 0044b150 __uninitialized_fill_n<CCharString*>
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
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template CCharString* __fastcall __uninitialized_fill_n<CCharString*, unsigned int, CCharString>(CCharString*, unsigned int, const CCharString&, const __false_type&);
}

static int g_copies;
CCharString::CCharString(const CCharString& s) : PData(s.PData) { ++g_copies; }
int main()
{
    static void* buf[6];
    for (int i = 0; i < 6; ++i) buf[i] = (void*)0x99;
    CCharString* first = (CCharString*)buf;
    CCharString x = *(CCharString*)&buf[5]; x.PData = (void*)0x1234; g_copies = 0;
    CCharString* end = std::__uninitialized_fill_n(first, 3u, x, std::__false_type());
    int ok = end == first + 3 && g_copies == 3 && buf[0] == (void*)0x1234 && buf[2] == (void*)0x1234 && buf[3] == (void*)0x99;
    ok &= std::__uninitialized_fill_n(first, 0u, x, std::__false_type()) == first && g_copies == 3;
    printf(ok ? "0044b150_TEST PASS\n" : "0044b150_TEST FAIL\n");
    return ok ? 0 : 1;
}