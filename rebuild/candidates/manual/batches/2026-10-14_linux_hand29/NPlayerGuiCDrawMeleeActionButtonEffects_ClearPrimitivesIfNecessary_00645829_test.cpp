#include <cstdio>
static int g_destroyed = 0, g_baseClears = 0;
struct CMeleeCombatAbilityDef { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; void Release() { if (--m_refCount == 0) Destroy(); } };
namespace NPlayerGui {
struct CDrawBase { virtual void v00() { } unsigned char _pad04[0x14]; bool Drawing; unsigned char _pad19[0x2b]; void ClearPrimitivesIfNecessary() { ++g_baseClears; } };
struct CDrawMeleeActionButtonEffects : CDrawBase { CMeleeCombatAbilityDef* PMeleeAbilityDef; void ClearPrimitivesIfNecessary() { if (!Drawing) { if (PMeleeAbilityDef) { PMeleeAbilityDef->Release(); PMeleeAbilityDef = 0; } PMeleeAbilityDef = 0; } CDrawBase::ClearPrimitivesIfNecessary(); } }; }
int main() { CMeleeCombatAbilityDef def; def.m_refCount = 1; NPlayerGui::CDrawMeleeActionButtonEffects d; d.PMeleeAbilityDef = &def; d.Drawing = true; d.ClearPrimitivesIfNecessary();
    if (g_baseClears != 1 || d.PMeleeAbilityDef != &def || g_destroyed) { std::printf("00645829_TEST FAIL\n"); return 1; } d.Drawing = false; d.ClearPrimitivesIfNecessary(); if (g_baseClears != 2 || d.PMeleeAbilityDef || g_destroyed != 1) { std::printf("00645829_TEST FAIL\n"); return 1; } std::printf("00645829_TEST PASS\n"); return 0; }
