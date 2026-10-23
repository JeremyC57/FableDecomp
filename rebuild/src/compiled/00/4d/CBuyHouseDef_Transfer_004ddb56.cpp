#include "engine/CBuyHouseDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer_0043314a(void* sub); };
struct CBuyHouseDef_Methods : CBuyHouseDef {
    void Transfer(CPersistContext* ctx);
};
void CBuyHouseDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer_0043314a(reinterpret_cast<char*>(this) + 0x25); }