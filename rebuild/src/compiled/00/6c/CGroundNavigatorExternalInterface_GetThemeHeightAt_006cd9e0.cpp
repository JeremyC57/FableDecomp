// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/C3DVector.h"
struct CTerrainResult { float GetHeight_0063a390(); };
class CThingCreatureBase { public: CTerrainResult* Lookup(const C3DVector& pos); };
float __fastcall CGroundNavigatorExternalInterface_GetTerrainHeightAt(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& pos)
{
    CTerrainResult* r = self->Creature->Lookup(pos);
    return r->GetHeight_0063a390();
}