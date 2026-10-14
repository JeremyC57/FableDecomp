// CEngine::CutscenePreload(bool fast, long) -- unless a new scene is already loading, flag a cutscene preload and arm the scene time-out with the fast (flags 2) or normal (flags 0) cutscene time-out
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23A10, length 51 bytes.
// Callees: none. Layout from engine/CEngine.h (PDB): NewSceneFlag +0x18, NewSceneLoadFlags +0x1c, NewSceneTimeOut +0x20, CutscenePreloadFlag +0x24,
// CutscenePreloadTimeout +0x28, CutsceneFastPreloadTimeout +0x2c. Shape: the preload flag store is common to both arms and hoisted above the branch;
// the float time-outs are copied as dwords; `ret 8` = two arguments, the second unused.
#include "engine/CEngine.h"

struct CEngine_Methods : CEngine {
    void CutscenePreload(bool fast, long unused);
};

void CEngine_Methods::CutscenePreload(bool fast, long unused)
{
    if (!NewSceneFlag) {
        CutscenePreloadFlag = true;
        if (fast) {
            NewSceneLoadFlags = 2;
            NewSceneTimeOut = CutsceneFastPreloadTimeout;
        } else {
            NewSceneLoadFlags = 0;
            NewSceneTimeOut = CutscenePreloadTimeout;
        }
    }
}