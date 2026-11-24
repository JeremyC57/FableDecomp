// Leaf: ?Transfer@COccupiableDef@@UAEXAAVCPersistContext@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/COccupiableDef.h under FABLE_RETAIL_COCCUPIABLEDEF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was COccupiableDef_Methods::Transfer); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_COCCUPIABLEDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/COccupiableDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
void COccupiableDef::Transfer(CPersistContext& ctx) { ctx.Transfer(*(unsigned long*)(reinterpret_cast<char*>(this) + 0x28)); }
