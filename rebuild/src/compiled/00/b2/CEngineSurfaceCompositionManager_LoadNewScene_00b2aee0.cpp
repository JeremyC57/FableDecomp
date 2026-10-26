#include "engine/CEngineSurfaceCompositionManager.h"  // retyped onto the PDB layout; byte parity re-verified
class CGraphicDataBank {
public:
    void ChangeScene();   // 0x009FD0C0
};
struct Sub; 
struct CEngineSurfaceCompositionManager_Methods : CEngineSurfaceCompositionManager {
    char M(int params);
};
char CEngineSurfaceCompositionManager_Methods::M(int params){ ((CGraphicDataBank*)((Sub*)((char*)this+0x31c)))->ChangeScene(); return 0; }