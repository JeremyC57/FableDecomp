class CGPUScratchBufferTextureManager;
class CEngine;
extern char* g_engineSingleton;

CGPUScratchBufferTextureManager* CEngine_GetScratchBufferTextureManager(CEngine* engine)
{
    (void)engine;
    return (CGPUScratchBufferTextureManager*)(g_engineSingleton + 0x3190);
}