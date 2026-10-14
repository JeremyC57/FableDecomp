#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_lists[2]; static long g_scenes[2]; static long g_n = 0;
struct CResourceList { unsigned char _body[0x3c]; void SetScene(long scene) { g_lists[g_n] = this; g_scenes[g_n++] = scene; } };
struct CGraphicDataBank { unsigned char _pad[0x168]; CResourceList m_textures; CResourceList m_frames; unsigned char _pad1e0[0x88]; long m_scene; void ChangeScene() { m_textures.SetScene(m_scene); m_frames.SetScene(m_scene); } };

int main() {
    CGraphicDataBank b; b.m_scene = 6; b.ChangeScene(); if (g_n != 2 || g_lists[0] != &b.m_textures || g_lists[1] != &b.m_frames || g_scenes[0] != 6 || g_scenes[1] != 6) { std::printf("009fd0c0_TEST FAIL\n"); return 1; }
    std::printf("009fd0c0_TEST PASS\n"); return 0;
}