#include "engine/CAICreatureWillPowerIndicatorDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CAICreatureWillPowerIndicatorDef_Methods : CAICreatureWillPowerIndicatorDef {
    void Transfer(CPersistContext* ctx);
};
void CAICreatureWillPowerIndicatorDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(float*)(reinterpret_cast<char*>(this) + 0x28)); }