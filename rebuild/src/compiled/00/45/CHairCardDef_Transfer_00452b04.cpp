// CHairCardDef::Transfer(CPersistContext& context) -- persist the hair object
// Retail: Fable.exe (TLC, VC7.1) @ 0x00452B04, virtual `?Transfer@CHairCardDef@@UAEXAAVCPersistContext@@@Z` (Ego_r).
// Callees: CPersistContext::Transfer<long> 0x00431102 (namepass TR2).
// Layout: engine/CHairCardDef.h (HairObject +0x28). The generated header declares no members, so the body stays the
//   equivalent free __fastcall (this in ecx, the context on the stack).
#include "engine/CHairCardDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};

void __fastcall CHairCardDef_Transfer(CHairCardDef* self, int /*edx*/, CPersistContext& context)
{
    context.Transfer(self->HairObject);
}
