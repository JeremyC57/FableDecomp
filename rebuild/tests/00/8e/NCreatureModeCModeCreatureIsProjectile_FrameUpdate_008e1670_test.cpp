#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0, g_recover = 0;
struct SState { unsigned char pad[0x2c]; unsigned long flags; };
struct CModeBase { virtual ~CModeBase() { } virtual void FrameUpdate() { ++g_base; } };
struct CMode : CModeBase { SState* state; virtual void FrameUpdate() { CModeBase::FrameUpdate(); if (!(state->flags & 0x800000)) Recover(); } void Recover() { ++g_recover; } };
int main()
{
    SState st; st.flags = 0; CMode m; m.state = &st;
    m.FrameUpdate(); if (g_base != 1 || g_recover != 1) { std::printf("008e1670_TEST FAIL\n"); return 1; }
    st.flags = 0x800000; m.FrameUpdate(); if (g_base != 2 || g_recover != 1) { std::printf("008e1670_TEST FAIL\n"); return 1; }
    st.flags = 0x400000; m.FrameUpdate(); if (g_recover != 2) { std::printf("008e1670_TEST FAIL\n"); return 1; }
    std::printf("008e1670_TEST PASS\n");
    return 0;
}