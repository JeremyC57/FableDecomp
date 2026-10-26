#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct SScratchBufferState_00f37520 { long m_0, m_4, m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28, m_2c, m_30, m_34, m_38; bool m_flag; SScratchBufferState_00f37520() : m_0(0), m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0), m_20(0), m_24(0), m_28(0), m_2c(0), m_30(0), m_34(0), m_38(0), m_flag(false) { } };
class CGPUScratchBufferTextureManager : public CBase { public: SScratchBufferState_00f37520 m_state; virtual void v00() { } CGPUScratchBufferTextureManager() : CBase() { } };
int main() {
    CGPUScratchBufferTextureManager* m = new CGPUScratchBufferTextureManager; if (g_base != 1 || m->m_state.m_0 || m->m_state.m_38 || m->m_state.m_flag) { std::printf("00f37520_TEST FAIL\n"); return 1; } delete m;
    std::printf("00f37520_TEST PASS\n"); return 0;
}