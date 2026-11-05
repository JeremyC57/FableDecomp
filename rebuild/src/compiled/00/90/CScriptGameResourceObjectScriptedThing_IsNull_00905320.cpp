// Leaf: ?IsNull@CScriptGameResourceObjectScriptedThing_Methods@@QAEEXZ
#include "engine/CScriptGameResourceObjectScriptedThing.h"  // retyped onto the PDB layout; byte parity re-verified
struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    unsigned char IsNull();
};
unsigned char CScriptGameResourceObjectScriptedThing_Methods::IsNull(){ return this->NullResource; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsNull@CScriptGameResourceObjectScriptedThing@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
