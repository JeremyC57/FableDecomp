#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* tag, long v) { char buf[32]; std::sprintf(buf, "%s(%ld)", tag, v); std::strcat(g_seq, buf); }
struct CGuiControl { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void SetMode(long m) { note("3", m); } virtual void SetState(long s) { note("4", s); } };
static void* g_focused = 0;
struct CFrontEndManager { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void SetFocusControl(CGuiControl* c) { std::strcat(g_seq, "F"); g_focused = c; } };
static CFrontEndManager g_manager; static CFrontEndManager* GetInstance() { return &g_manager; }
struct CRoot { virtual void v00() { } };
struct CKeyRedefiner;
static CKeyRedefiner* g_pSelected = 0;
struct CKeyRedefiner : CRoot, CGuiControl { void UpdateKeyText() { std::strcat(g_seq, "T"); }
    void CancelSelection() { if (g_pSelected == this) { g_pSelected = 0; UpdateKeyText(); SetState(0x21); SetMode(0x19); GetInstance()->SetFocusControl(this); } } };

int main()
{
    CKeyRedefiner r; CGuiControl* control = &r; g_pSelected = &r; r.CancelSelection();
    if (std::strcmp(g_seq, "T4(33)3(25)F") || g_pSelected != 0 || g_focused != control) { std::printf("00557bd0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; CKeyRedefiner other; g_pSelected = &other; r.CancelSelection(); if (std::strcmp(g_seq, "") || g_pSelected != &other) { std::printf("00557bd0_TEST FAIL\n"); return 1; }
    std::printf("00557bd0_TEST PASS\n");
    return 0;
}