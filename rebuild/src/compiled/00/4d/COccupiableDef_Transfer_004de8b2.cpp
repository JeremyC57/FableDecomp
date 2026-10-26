#include "engine/COccupiableDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: void Xfer_00431020(void* sub); };
struct COccupiableDef_Methods : COccupiableDef {
    void Transfer(CPersistContext* ctx);
};
void COccupiableDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer_00431020(reinterpret_cast<char*>(this) + 0x28); }