#include "engine/CExpressionSubDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: void Xfer(void* sub); };
struct CExpressionSubDef_Methods : CExpressionSubDef {
    void Transfer(CPersistContext* ctx);
};
void CExpressionSubDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }