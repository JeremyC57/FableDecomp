// Leaf: ?CameraSetCameraPreloadFlag@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Provider { void* vt; };
class CWorld { public: char pad[0x34]; Provider* prov; };

void CGameScriptInterface::CameraSetCameraPreloadFlag(bool flag) const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    void* local;
    Provider* prov = self->World->prov;
    void** vt = *(void***)prov;
    typedef void (__fastcall *fn_t)(Provider*, void*, void**);
    fn_t fn = (fn_t)vt[9];
    fn(prov, vt, &local);
    *((char*)local + 0x145) = flag;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_CameraSetCameraPreloadFlag -> CGameScriptInterface::CameraSetCameraPreloadFlag, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
