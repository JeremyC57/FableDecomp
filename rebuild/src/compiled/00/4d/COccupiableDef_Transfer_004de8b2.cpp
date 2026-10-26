#include "engine/COccupiableDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct COccupiableDef_Methods : COccupiableDef {
    void Transfer(CPersistContext* ctx);
};
void COccupiableDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(unsigned long*)(reinterpret_cast<char*>(this) + 0x28)); }