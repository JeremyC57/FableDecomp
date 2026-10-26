#include "engine/CExpressionSubDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CExpressionSubDef_Methods : CExpressionSubDef {
    void Transfer(CPersistContext* ctx);
};
void CExpressionSubDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }