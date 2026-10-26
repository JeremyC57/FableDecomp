#include "engine/CFireballSpellLevelDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CFireballSpellLevelDef_Methods : CFireballSpellLevelDef {
    void Transfer(CPersistContext* ctx);
};
void CFireballSpellLevelDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }