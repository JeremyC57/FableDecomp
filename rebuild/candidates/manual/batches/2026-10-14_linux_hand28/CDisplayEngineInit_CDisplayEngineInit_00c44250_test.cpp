#include <cstdio>
#include <string.h>
#include <new>
struct CMainGameComponent; struct CGameDefinitionManager; struct CPlayerManager; struct CWorld; struct CGraphicDataBank; struct CMeshDataBank;
struct CPixelFormat { long FormatIndex; };
struct CDisplayEngineInit {
    CMainGameComponent* Component; CGameDefinitionManager* DefinitionManager; CPlayerManager* PlayerManager; CWorld* World; CGraphicDataBank* MainGraphicBank;
    unsigned long Width, Height; CPixelFormat PixelFormat; CMeshDataBank* MeshBank;
    CDisplayEngineInit() : PlayerManager(0) { Component = 0; DefinitionManager = 0; World = 0; MainGraphicBank = 0; Width = 0; Height = 0; PixelFormat.FormatIndex = 0; MeshBank = 0; }
};
int main()
{
    unsigned char raw[sizeof(CDisplayEngineInit)]; memset(raw, 0xee, sizeof(raw));
    CDisplayEngineInit* o = new (raw) CDisplayEngineInit;
    for (unsigned i = 0; i < sizeof(raw); ++i) if (raw[i] != 0) { std::printf("00c44250_TEST FAIL\n"); return 1; }
    if (sizeof(CDisplayEngineInit) != 36 || (char*)&o->PixelFormat - (char*)o != 0x1c) { std::printf("00c44250_TEST FAIL\n"); return 1; }
    std::printf("00c44250_TEST PASS\n"); return 0;
}
