// test for 00440ba0 __destroy_aux
#include <stdio.h>
#include <string.h>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
struct CRecordWithWideStringAt0 {
    CWideString Text;                                    // +0x00
    long        Data[4];                                 // +0x04
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
template void __fastcall __destroy_aux<CRecordWithWideStringAt0*>(CRecordWithWideStringAt0*, CRecordWithWideStringAt0*, const __false_type&);
}

static void* g_seen[8]; static int g_n;
CWideString::~CWideString() { if (g_n < 8) g_seen[g_n] = this; ++g_n; }
int main()
{
    static unsigned char buf[5 * 20];
    CRecordWithWideStringAt0* r = (CRecordWithWideStringAt0*)buf;
    int ok = sizeof(CRecordWithWideStringAt0) == 20;
    std::__destroy_aux(r, r + 5, std::__false_type());
    ok &= g_n == 5;
    for (int i = 0; i < 5; ++i) ok &= g_seen[i] == buf + i * 20 + 0;
    g_n = 0;
    std::__destroy_aux(r + 2, r + 2, std::__false_type());
    ok &= g_n == 0;
    printf(ok ? "00440ba0_TEST PASS\n" : "00440ba0_TEST FAIL\n");
    return ok ? 0 : 1;
}