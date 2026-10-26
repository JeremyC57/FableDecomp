#include "engine/CInterestingToVillagersDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CInterestingToVillagersDef_Methods : CInterestingToVillagersDef {
    void Transfer(CPersistContext* ctx);
};
void CInterestingToVillagersDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(float*)(reinterpret_cast<char*>(this) + 0x28)); }