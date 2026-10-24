#include <cstdio>
enum EEvent { EVENT_4 = 4, EVENT_5 = 5, EVENT_26 = 26 };
static int g_cb = 0;
static void OnOk() { ++g_cb; }
struct COk {
    long Requested; void (*OkCallBack)();
    void ChangeState(unsigned long s) { Requested = (long)s; }
    void ProcessEvent(EEvent event)
    {
        if (event == EVENT_26 || event == EVENT_4) ChangeState(0);
        if (event == EVENT_26 || event == EVENT_4) { if (OkCallBack) OkCallBack(); }
    }
};
int main()
{
    COk o; o.Requested = -1; o.OkCallBack = OnOk;
    o.ProcessEvent(EVENT_5);
    if (o.Requested != -1 || g_cb != 0) { std::printf("0041cb9d_TEST FAIL\n"); return 1; }
    o.ProcessEvent(EVENT_4);
    if (o.Requested != 0 || g_cb != 1) { std::printf("0041cb9d_TEST FAIL\n"); return 1; }
    o.Requested = -1; o.ProcessEvent(EVENT_26);
    if (o.Requested != 0 || g_cb != 2) { std::printf("0041cb9d_TEST FAIL\n"); return 1; }
    o.OkCallBack = 0; o.Requested = -1; o.ProcessEvent(EVENT_26);
    if (o.Requested != 0 || g_cb != 2) { std::printf("0041cb9d_TEST FAIL\n"); return 1; }
    std::printf("0041cb9d_TEST PASS\n");
    return 0;
}