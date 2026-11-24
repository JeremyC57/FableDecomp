// Leaf: ?Transfer@CCameraCollisionDef@@UAEXAAVCPersistContext@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CCameraCollisionDef.h under FABLE_RETAIL_CCAMERACOLLISIONDEF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCameraCollisionDef_Methods::Transfer); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCAMERACOLLISIONDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCameraCollisionDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
void CCameraCollisionDef::Transfer(CPersistContext& ctx) { ctx.Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x28)); }
