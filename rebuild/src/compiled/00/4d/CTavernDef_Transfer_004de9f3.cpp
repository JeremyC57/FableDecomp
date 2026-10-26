#include "engine/CTavernDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CTavernDef_Methods : CTavernDef {
    void Transfer(CPersistContext* ctx);
};
void CTavernDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }