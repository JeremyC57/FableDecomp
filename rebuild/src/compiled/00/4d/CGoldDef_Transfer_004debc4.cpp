#include "engine/CGoldDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CGoldDef_Methods : CGoldDef {
    void Transfer(CPersistContext* ctx);
};
void CGoldDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }