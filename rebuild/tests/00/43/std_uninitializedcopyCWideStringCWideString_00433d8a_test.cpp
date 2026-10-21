// test for 00433d8a __uninitialized_copy<CWideString*>
#include <stdio.h>
#include <new>
class CWideString {
public:
    void* PData;
    CWideString(const CWideString& s);                   // 0x0099B720
};
namespace std {
struct __false_type {};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template CWideString* __fastcall __uninitialized_copy<CWideString*, CWideString*>(CWideString*, CWideString*, CWideString*, const __false_type&);
}

static int g_cc;
CWideString::CWideString(const CWideString& s) : PData(s.PData) { ++g_cc; }
int main()
{
    unsigned long srcraw[3] = { 1, 2, 3 };
    CWideString* src = (CWideString*)srcraw;
    unsigned long raw[4] = { 9, 9, 9, 9 };
    CWideString* dst = (CWideString*)raw;
    CWideString* e = std::__uninitialized_copy(src, src + 3, dst, std::__false_type());
    int ok = e == dst + 3 && g_cc == 3 && raw[0] == 1 && raw[1] == 2 && raw[2] == 3 && raw[3] == 9;
    ok = ok && std::__uninitialized_copy(src, src, dst, std::__false_type()) == dst && g_cc == 3;
    printf(ok ? "00433d8a_TEST PASS\n" : "00433d8a_TEST FAIL\n");
    return 0;
}