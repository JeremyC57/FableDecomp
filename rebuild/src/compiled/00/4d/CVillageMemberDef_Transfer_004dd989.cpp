#include "engine/CVillageMemberDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CVillageMemberDef_Methods : CVillageMemberDef {
    void Transfer(CPersistContext* ctx);
};
void CVillageMemberDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(bool*)(reinterpret_cast<char*>(this) + 0x25)); }