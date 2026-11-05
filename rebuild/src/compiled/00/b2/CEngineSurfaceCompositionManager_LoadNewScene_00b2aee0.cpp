// Leaf: ?LoadNewScene@CEngineSurfaceCompositionManager_Methods@@QAEDH@Z
#include "engine/CEngineSurfaceCompositionManager.h"  // retyped onto the PDB layout; byte parity re-verified
class CGraphicDataBank {
public:
    void ChangeScene();   // 0x009FD0C0
};
struct Sub; 
struct CEngineSurfaceCompositionManager_Methods : CEngineSurfaceCompositionManager {
    char LoadNewScene(int params);
};
char CEngineSurfaceCompositionManager_Methods::LoadNewScene(int params){ ((CGraphicDataBank*)((Sub*)((char*)this+0x31c)))->ChangeScene(); return 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?LoadNewScene@CEngineSurfaceCompositionManager@@UAE_NABVCEngineComponentLoadNewSceneParams@@@Z); evidence: RTTI vtable slot + PDB name set.
