#include <cstdio>
#include <stddef.h>
struct CComponentInterface { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } };
struct CObserver_Retail { virtual void v0() { } virtual void ProcessEvent() { } virtual void v2() { } virtual void ObserveEvent(long) { } virtual void StopObservingEvent(long) { } virtual void v5() { } unsigned char _body[0x10]; };
struct CEventSinkInterface { virtual void v0() { } virtual void v1() { } virtual void v2() { } virtual void v3() { } };
static long g_owner = 0; static int g_processed = 0;
struct CClickable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface { unsigned char _body[0x194 - 0x1c]; CClickable_Retail(long owner) { g_owner = owner; } };
struct CNavButton : CClickable_Retail { virtual void v000() { } virtual void ProcessEvent() { ++g_processed; } virtual void v3() { } CNavButton(long owner) : CClickable_Retail(owner) { } };
int main()
{
    CNavButton button(42);
    CObserver_Retail* observer = &button;
    observer->ProcessEvent();
    if (g_owner != 42 || g_processed != 1 || sizeof(CClickable_Retail) != 0x194 || sizeof(button) != 0x194
        || (char*)observer - (char*)&button != 4 || (char*)static_cast<CEventSinkInterface*>(&button) - (char*)&button != 0x18) { std::printf("00558b90_TEST FAIL\n"); return 1; }
    std::printf("00558b90_TEST PASS\n"); return 0;
}
