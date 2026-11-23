// CEngine::CutscenePreload(bool fast, long) -- unless a new scene is already loading, flag a cutscene preload and arm the scene time-out with the fast (flags 2) or normal (flags 0) cutscene time-out
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23A10, length 51 bytes.
// Callees: none. Layout from engine/CEngine.h (PDB): NewSceneFlag +0x18, NewSceneLoadFlags +0x1c, NewSceneTimeOut +0x20, CutscenePreloadFlag +0x24,
// CutscenePreloadTimeout +0x28, CutsceneFastPreloadTimeout +0x2c. Shape: the preload flag store is common to both arms and hoisted above the branch;
// the float time-outs are copied as dwords; `ret 8` = two arguments, the second unused.
// Leaf: ?CutscenePreload@CEngine@@UAEX_N0@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CEngine.h under FABLE_RETAIL_CINITBASECLASS_VALIDATE, FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE, FABLE_RETAIL_CIENGINE_RENDER, FABLE_RETAIL_CENGINE, FABLE_RETAIL_CENGINE_OVERRIDES with the PDB parameter / return types (was ?CutscenePreload@CEngine_Methods@@QAEX_NJ@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CEngine.h"
void CEngine::CutscenePreload(bool fast, bool unused) {
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
