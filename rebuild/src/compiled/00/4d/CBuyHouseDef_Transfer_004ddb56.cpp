// Leaf: ?Transfer@CBuyHouseDef@@UAEXAAVCPersistContext@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CBuyHouseDef.h under FABLE_RETAIL_CBUYHOUSEDEF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CBuyHouseDef_Methods::Transfer); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CBUYHOUSEDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CBuyHouseDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
void CBuyHouseDef::Transfer(CPersistContext& ctx) { ctx.Transfer(*(bool*)(reinterpret_cast<char*>(this) + 0x25)); }
