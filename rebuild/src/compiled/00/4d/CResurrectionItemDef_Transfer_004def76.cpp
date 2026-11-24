// Leaf: ?Transfer@CResurrectionItemDef@@UAEXAAVCPersistContext@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CResurrectionItemDef.h under FABLE_RETAIL_CRESURRECTIONITEMDEF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CResurrectionItemDef_Methods::Transfer); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CRESURRECTIONITEMDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResurrectionItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
void CResurrectionItemDef::Transfer(CPersistContext& ctx) { ctx.Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }
