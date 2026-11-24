// Leaf: ?Transfer@CAICreatureWillPowerIndicatorDef@@UAEXAAVCPersistContext@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CAICreatureWillPowerIndicatorDef.h under FABLE_RETAIL_CAICREATUREWILLPOWERINDICATORDEF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CAICreatureWillPowerIndicatorDef_Methods::Transfer); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CAICREATUREWILLPOWERINDICATORDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CAICreatureWillPowerIndicatorDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public: template <class T> void Transfer(T& value); };
void CAICreatureWillPowerIndicatorDef::Transfer(CPersistContext& ctx) { ctx.Transfer(*(float*)(reinterpret_cast<char*>(this) + 0x28)); }
