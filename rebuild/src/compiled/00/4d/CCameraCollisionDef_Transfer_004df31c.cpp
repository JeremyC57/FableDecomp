#include "engine/CCameraCollisionDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CCameraCollisionDef_Methods : CCameraCollisionDef {
    void Transfer(CPersistContext* ctx);
};
void CCameraCollisionDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }