// Leaf: ?WasRendered@CEngineInternalPrimitiveBase_Methods@@QAEHXZ
#include "engine/CEngineInternalPrimitiveBase.h"  // retyped onto the PDB layout; byte parity re-verified
extern unsigned short g_frame;
struct CEngineInternalPrimitiveBase_Methods : CEngineInternalPrimitiveBase {
    int WasRendered();
};
int CEngineInternalPrimitiveBase_Methods::WasRendered(){ unsigned short v=this->LastFrameRendered; return v == g_frame; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?WasRendered@CEngineInternalPrimitiveBase@@MBE_NXZ); evidence: RTTI vtable slot + PDB name set.
