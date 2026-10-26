#include "engine/CEnemyDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CEnemyDef_Methods : CEnemyDef {
    void Transfer(CPersistContext* ctx);
};
void CEnemyDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }