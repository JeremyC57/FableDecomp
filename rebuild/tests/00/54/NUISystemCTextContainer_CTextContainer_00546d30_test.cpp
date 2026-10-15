#include <cstdio>
#include <cstring>
static char g_seq[8] = "";
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CComponentInterface { virtual void v000() { } virtual ~CComponentInterface() { } };
struct CObserver_Retail { virtual void ProcessEvent() { } unsigned char _body[0x10]; };
struct CEventSinkInterface { virtual void v0() { } };
struct CChangingStateComponent : CComponentInterface, CObserver_Retail, CEventSinkInterface {
    long Owner;
    CChangingStateComponent(long owner) : Owner(owner) { std::strcat(g_seq, "B"); }
};
struct CTextContainer : CChangingStateComponent {
    virtual void v000() { } virtual void ProcessEvent() { } virtual void v0() { }
    C2DBoxF Window;
    CTextContainer(long owner) : CChangingStateComponent(owner) { GetDataFromDef(); }
    void GetDataFromDef() { std::strcat(g_seq, "G"); Window.TLX = 1; Window.TLY = 2; Window.BRX = 3; Window.BRY = 4; }
};
int main()
{
    CTextContainer t(7);
    if (std::strcmp(g_seq, "BG") || t.Owner != 7 || t.Window.BRY != 4) { std::printf("00546d30_TEST FAIL\n"); return 1; }
    std::printf("00546d30_TEST PASS\n");
    return 0;
}