#include <cstdio>
// CBaseInputPin::Notify: E_POINTER for a null sender, else NOERROR (the quality message is ignored).
struct Quality { int Type; int Proportion; long long Late; long long TimeStamp; };
static int Notify(void* pSender, Quality q) { (void)q; if (pSender == 0) return (int)0x80004003; return 0; }
int main()
{
    Quality q; q.Type = 0; q.Proportion = 1000; q.Late = 5; q.TimeStamp = 6; int f;
    if (Notify(0, q) != (int)0x80004003 || Notify(&f, q) != 0) { std::printf("00ca8e70_TEST FAIL\n"); return 1; }
    std::printf("00ca8e70_TEST PASS\n");
    return 0;
}