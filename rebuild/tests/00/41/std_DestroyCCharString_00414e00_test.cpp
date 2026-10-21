// behaviour test for 0x00414E00
#include <stdio.h>
static int g_log[8]; static int g_n = 0;
class CCharString {
public:
    void* PData;
    ~CCharString() { g_log[g_n++] = (int)(long)PData; }
};
namespace std {
struct __false_type {};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
inline void __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class ForwardIter>
void __fastcall _Destroy(ForwardIter first, ForwardIter last)
{
    __destroy_aux(first, last, __false_type());
}
template void __fastcall _Destroy<CCharString*>(CCharString*, CCharString*);
}

int main()
{
    static long raw[3] = { 11, 22, 33 }; CCharString* a = (CCharString*)raw;
    std::_Destroy(a, a + 3);
    int ok = g_n == 3 && g_log[0] == 11 && g_log[1] == 22 && g_log[2] == 33;
    std::_Destroy(a, a); ok &= g_n == 3;
    printf(ok ? "00414e00_TEST PASS\n" : "00414e00_TEST FAIL\n");
    return ok ? 0 : 1;
}