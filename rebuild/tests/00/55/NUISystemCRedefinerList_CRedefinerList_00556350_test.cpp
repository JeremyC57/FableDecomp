#include <cstdio>
struct CComponent { int id; };
static CComponent* g_parent = 0;
struct CUIPrimaryBase { virtual void p00() { } virtual ~CUIPrimaryBase() { } };
struct CUISecondaryBaseA { virtual void a00() { } virtual ~CUISecondaryBaseA() { } unsigned char _body[0x10]; };
struct CUISecondaryBaseB { virtual void b00() { } virtual ~CUISecondaryBaseB() { } };
struct CScrollableList : CUIPrimaryBase, CUISecondaryBaseA, CUISecondaryBaseB {
    CComponent* m_parent;
    CScrollableList(CComponent* parent) : m_parent(parent) { g_parent = parent; }
};
struct CRedefinerList : CScrollableList {
    bool IsActivated; bool RefreshAfterFade; float FadeOutTime; bool HaveUndefinedControls;
    CComponent* PUndefinedWarning; CComponent* PAcceptButton;
    virtual void p00() { } virtual void a00() { } virtual void b00() { }
    CRedefinerList(CComponent* parent)
        : CScrollableList(parent), IsActivated(false), RefreshAfterFade(false), FadeOutTime(0.0f), HaveUndefinedControls(false), PUndefinedWarning(0), PAcceptButton(0) { }
};
int main()
{
    CComponent c; c.id = 1;
    CRedefinerList* l = new CRedefinerList(&c);
    if (g_parent != &c || l->m_parent != &c || l->IsActivated || l->RefreshAfterFade || l->FadeOutTime != 0.0f || l->HaveUndefinedControls
        || l->PUndefinedWarning != 0 || l->PAcceptButton != 0) { std::printf("00556350_TEST FAIL\n"); return 1; }
    delete l;
    std::printf("00556350_TEST PASS\n");
    return 0;
}