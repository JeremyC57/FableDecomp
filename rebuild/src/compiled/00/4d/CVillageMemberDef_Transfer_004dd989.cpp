#include "engine/CVillageMemberDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: void Xfer_0043314a(void* sub); };
struct CVillageMemberDef_Methods : CVillageMemberDef {
    void Transfer(CPersistContext* ctx);
};
void CVillageMemberDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer_0043314a(reinterpret_cast<char*>(this) + 0x25); }