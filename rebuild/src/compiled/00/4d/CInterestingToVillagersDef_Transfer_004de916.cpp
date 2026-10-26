#include "engine/CInterestingToVillagersDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: void Xfer_00431061(void* sub); };
struct CInterestingToVillagersDef_Methods : CInterestingToVillagersDef {
    void Transfer(CPersistContext* ctx);
};
void CInterestingToVillagersDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer_00431061(reinterpret_cast<char*>(this) + 0x28); }