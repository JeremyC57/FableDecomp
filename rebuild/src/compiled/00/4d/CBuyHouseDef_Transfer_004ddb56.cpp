#include "engine/CBuyHouseDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CBuyHouseDef_Methods : CBuyHouseDef {
    void Transfer(CPersistContext* ctx);
};
void CBuyHouseDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(bool*)(reinterpret_cast<char*>(this) + 0x25)); }