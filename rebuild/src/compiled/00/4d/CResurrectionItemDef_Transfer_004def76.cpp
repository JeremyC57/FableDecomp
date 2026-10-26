#include "engine/CResurrectionItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
struct CResurrectionItemDef_Methods : CResurrectionItemDef {
    void Transfer(CPersistContext* ctx);
};
void CResurrectionItemDef_Methods::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }