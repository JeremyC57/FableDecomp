// test for 0044b120 __uninitialized_copy<CCharString*>
#include <stdio.h>
#include <new>
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& s);                   // 0x0099B720
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
template CCharString* __fastcall __uninitialized_copy<CCharString*, CCharString*>(CCharString*, CCharString*, CCharString*, const __false_type&);
}

static int g_cc;
CCharString::CCharString(const CCharString& s) : PData(s.PData) { ++g_cc; }
int main()
{
    unsigned long srcraw[3] = { 1, 2, 3 };
    CCharString* src = (CCharString*)srcraw;
    unsigned long raw[4] = { 9, 9, 9, 9 };
    CCharString* dst = (CCharString*)raw;
    CCharString* e = std::__uninitialized_copy(src, src + 3, dst, std::__false_type());
    int ok = e == dst + 3 && g_cc == 3 && raw[0] == 1 && raw[1] == 2 && raw[2] == 3 && raw[3] == 9;
    ok = ok && std::__uninitialized_copy(src, src, dst, std::__false_type()) == dst && g_cc == 3;
    printf(ok ? "0044b120_TEST PASS\n" : "0044b120_TEST FAIL\n");
    return 0;
}