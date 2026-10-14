#include <cstdio>
static int g_scene = 0, g_morph = 0;
struct CGraphicDataBank { void ChangeScene() { ++g_scene; } };
struct CEngineData { unsigned char _pad[0x954]; CGraphicDataBank* m_pGraphicBank; };
struct CEngineSurfaceCompositionManager { void UpdateTextureMorphCache() { ++g_morph; } };
static CGraphicDataBank g_bank; static CEngineData g_data; static CEngineData* g_pEngineData = &g_data; static CEngineSurfaceCompositionManager g_scm; static CEngineSurfaceCompositionManager* g_pSurfaceComposition = &g_scm;
struct CEngine { static void UpdateTextureMorphCache() { g_pEngineData->m_pGraphicBank->ChangeScene(); g_pSurfaceComposition->UpdateTextureMorphCache(); } };
int main() { g_data.m_pGraphicBank = &g_bank; CEngine::UpdateTextureMorphCache(); if (g_scene != 1 || g_morph != 1) { std::printf("00b239c0_TEST FAIL\n"); return 1; } std::printf("00b239c0_TEST PASS\n"); return 0; }
