#include <cstdio>
static int g_states = 0;
struct CInputState { unsigned char _body[3]; CInputState() { ++g_states; _body[0] = 9; } };
#pragma pack(push, 1)
struct CEditInputProcess { void* __vftable; unsigned char m_enabled; unsigned char _pad05[4]; CInputState m_state; CEditInputProcess() : m_enabled(1), m_state() { } };
#pragma pack(pop)
int main() { CEditInputProcess p; if (p.m_enabled != 1 || g_states != 1 || p.m_state._body[0] != 9) { std::printf("008239d0_TEST FAIL\n"); return 1; } std::printf("008239d0_TEST PASS\n"); return 0; }