#include <cstdio>
struct CEngine { unsigned char _pad[0x18]; bool NewSceneFlag; unsigned char _pad19[3]; unsigned long NewSceneLoadFlags; float NewSceneTimeOut; bool CutscenePreloadFlag; unsigned char _pad25[3]; float CutscenePreloadTimeout; float CutsceneFastPreloadTimeout;
    void CutscenePreload(bool fast, long) { if (!NewSceneFlag) { CutscenePreloadFlag = true; if (fast) { NewSceneLoadFlags = 2; NewSceneTimeOut = CutsceneFastPreloadTimeout; } else { NewSceneLoadFlags = 0; NewSceneTimeOut = CutscenePreloadTimeout; } } } };
int main() { CEngine e; e.NewSceneFlag = false; e.CutscenePreloadFlag = false; e.CutscenePreloadTimeout = 5.0f; e.CutsceneFastPreloadTimeout = 2.0f; e.NewSceneLoadFlags = 9; e.NewSceneTimeOut = 0;
    e.CutscenePreload(true, 0); if (!e.CutscenePreloadFlag || e.NewSceneLoadFlags != 2 || e.NewSceneTimeOut != 2.0f) { std::printf("00b23a10_TEST FAIL\n"); return 1; }
    e.CutscenePreload(false, 0); if (e.NewSceneLoadFlags != 0 || e.NewSceneTimeOut != 5.0f) { std::printf("00b23a10_TEST FAIL\n"); return 1; }
    e.NewSceneFlag = true; e.CutscenePreloadFlag = false; e.CutscenePreload(true, 0); if (e.CutscenePreloadFlag || e.NewSceneLoadFlags != 0) { std::printf("00b23a10_TEST FAIL\n"); return 1; } std::printf("00b23a10_TEST PASS\n"); return 0; }
