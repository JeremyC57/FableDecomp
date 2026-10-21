// test for 00440bd0 __destroy_aux
#include <stdio.h>
#include <string.h>
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
struct CRecordWithCharStringAtC {
    long        Data[3];                                 // +0x00
    CCharString Name;                                    // +0x0c
};
namespace std {
struct __false_type {};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template void __fastcall __destroy_aux<CRecordWithCharStringAtC*>(CRecordWithCharStringAtC*, CRecordWithCharStringAtC*, const __false_type&);
}

static void* g_seen[8]; static int g_n;
CCharString::~CCharString() { if (g_n < 8) g_seen[g_n] = this; ++g_n; }
int main()
{
    static unsigned char buf[5 * 16];
    CRecordWithCharStringAtC* r = (CRecordWithCharStringAtC*)buf;
    int ok = sizeof(CRecordWithCharStringAtC) == 16;
    std::__destroy_aux(r, r + 5, std::__false_type());
    ok &= g_n == 5;
    for (int i = 0; i < 5; ++i) ok &= g_seen[i] == buf + i * 16 + 12;
    g_n = 0;
    std::__destroy_aux(r + 2, r + 2, std::__false_type());
    ok &= g_n == 0;
    printf(ok ? "00440bd0_TEST PASS\n" : "00440bd0_TEST FAIL\n");
    return ok ? 0 : 1;
}