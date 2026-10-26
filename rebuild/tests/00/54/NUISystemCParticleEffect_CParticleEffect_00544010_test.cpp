#include <cstdio>
static long g_owner = 0; static int g_def = 0;
namespace NUISystem {
struct IComponentA { virtual void va00() { } unsigned char _state[0x10]; }; struct IComponentB { virtual void vb00() { } }; struct CComponentRoot { virtual void v00() { } };
class CChangingStateComponent : public CComponentRoot, public IComponentA, public IComponentB { public: unsigned char _body[0x15c - 0x1c]; CChangingStateComponent(long owner) { g_owner = owner; } };
class CParticleEffect : public CChangingStateComponent { public: void* m_pEffect; void GetDataFromDef() { ++g_def; } CParticleEffect(long owner) : CChangingStateComponent(owner), m_pEffect(0) { GetDataFromDef(); } }; }
int main() { NUISystem::CParticleEffect e(7); if (g_owner != 7 || e.m_pEffect != 0 || g_def != 1) { std::printf("00544010_TEST FAIL\n"); return 1; } std::printf("00544010_TEST PASS\n"); return 0; }