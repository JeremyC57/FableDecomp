#include <cstdio>
struct CMessageEventDef { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
extern "C" void __fastcall CMessageEventDef_dtor(CMessageEventDef* message) { (void)message; g_dtor_called++; }
void __cdecl op_delete(void* p) { (void)p; g_delete_called++; }

struct CLBP_Vtbl {
    void* CMessageEventDef_scalar_deleting_destructor(unsigned int flags);
};

void* CLBP_Vtbl::CMessageEventDef_scalar_deleting_destructor(unsigned int flags)
{
    CMessageEventDef* message = (CMessageEventDef*)this;
    CMessageEventDef_dtor(message);
    if (flags & 1) {
        op_delete(message);
    }
    return message;
}

int main() {
    CMessageEventDef obj;
    obj.f0 = 42;
    CLBP_Vtbl* h = (CLBP_Vtbl*)&obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = h->CMessageEventDef_scalar_deleting_destructor(0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    r = h->CMessageEventDef_scalar_deleting_destructor(1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL flag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0045585a_TEST PASS\n");
    return 0;
}