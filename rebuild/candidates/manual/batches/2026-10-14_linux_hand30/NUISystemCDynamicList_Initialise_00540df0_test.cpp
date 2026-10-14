#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

static int g_baseInit = 0, g_reset = 0, g_baseCtor = 0, g_getData = 0; static long g_owner = -1;
struct IComponentA { virtual void va00() {} unsigned char s[0x10]; };
struct IComponentB { virtual void vb00() {} };
struct CComponentRoot { virtual void v000() {} virtual void ResetPositions() { g_reset++; } };
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB { unsigned char body[0x15c - 0x1c]; CChangingStateComponent(long o) { g_baseCtor++; g_owner = o; } void Initialise() { g_baseInit++; } };

struct CDynamicList : CChangingStateComponent { long BInitialised; CDynamicList() : CChangingStateComponent(0), BInitialised(0) {} virtual void ResetPositions() { g_reset += 10; }
    void Initialise() { CChangingStateComponent::Initialise(); ResetPositions(); BInitialised = 1; } };
int main() { CDynamicList l; l.Initialise(); if (g_baseInit != 1 || g_reset != 10 || l.BInitialised != 1) { std::printf("00540df0_TEST FAIL\n"); return 1; }
    std::printf("00540df0_TEST PASS\n"); return 0; }