#include "engine/CAICreatureWillPowerIndicatorDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer_00431061(void* sub); };
struct CAICreatureWillPowerIndicatorDef_Methods : CAICreatureWillPowerIndicatorDef {
    void Transfer(CPersistContext* ctx);
};
void CAICreatureWillPowerIndicatorDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer_00431061(reinterpret_cast<char*>(this) + 0x28); }