#include <cstdio>
static long g_owner = 0; static int g_init = 0;
namespace NUISystem {
struct IComponentA { virtual void va00() { } unsigned char _state[0x10]; }; struct IComponentB { virtual void vb00() { } }; struct CComponentRoot { virtual void v00() { } };
struct CHoverable : CComponentRoot, IComponentA, IComponentB { unsigned char _body[0x16c - 0x1c]; CHoverable(long owner) { g_owner = owner; } };
struct CDraggableInto : CHoverable { void* m_pTarget; void Initialize() { ++g_init; } CDraggableInto(long owner) : CHoverable(owner), m_pTarget(0) { Initialize(); } }; }
int main() { NUISystem::CDraggableInto d(9); if (g_owner != 9 || d.m_pTarget != 0 || g_init != 1) { std::printf("00558ec0_TEST FAIL\n"); return 1; } std::printf("00558ec0_TEST PASS\n"); return 0; }