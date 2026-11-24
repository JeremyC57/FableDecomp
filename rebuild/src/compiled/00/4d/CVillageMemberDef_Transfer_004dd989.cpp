// Leaf: ?Transfer@CVillageMemberDef@@UAEXAAVCPersistContext@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CVillageMemberDef.h under FABLE_RETAIL_CVILLAGEMEMBERDEF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CVillageMemberDef_Methods::Transfer); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CVILLAGEMEMBERDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CVillageMemberDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
void CVillageMemberDef::Transfer(CPersistContext& ctx) { ctx.Transfer(*(bool*)(reinterpret_cast<char*>(this) + 0x25)); }
