// Leaf: ?FindPosZAtIncludingThemes@CGroundNavigatorExternalInterface@@UBEMABVC3DVector@@_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#define FABLE_RETAIL_CGROUNDNAVIGATOREXTERNALINTERFACE_VIRTUAL_MODEL
#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/C3DVector.h"
class CThingCreatureBase { public: float GetHeightZBelowAt(const C3DVector& v, bool a, bool b) const; };
float CGroundNavigatorExternalInterface::FindPosZAtIncludingThemes(const class C3DVector& v, bool inc) const
{
    CGroundNavigatorExternalInterface* self = const_cast<CGroundNavigatorExternalInterface*>(this);
    return self->Creature->GetHeightZBelowAt(v, true, inc);
}
// Flatten route (flatten_pilot v4, NF324): CGroundNavigatorExternalInterface_FindPosZAtIncludingThemes -> CGroundNavigatorExternalInterface::FindPosZAtIncludingThemes, the RTTI-placed PDB member; class model: engine/CGroundNavigatorExternalInterface.h under FABLE_RETAIL_CGROUNDNAVIGATOREXTERNALINTERFACE_VIRTUAL_MODEL.
