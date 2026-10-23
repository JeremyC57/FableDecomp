#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct C3DVector { float x, y, z; };
struct CTerrainResult { float GetHeight_0063a390(); };
struct CThingCreatureBase { CTerrainResult* Lookup(const C3DVector& pos); };
float __fastcall CGroundNavigatorExternalInterface_GetTerrainHeightAt(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& pos)
{
    CTerrainResult* r = self->Creature->Lookup(pos);
    return r->GetHeight_0063a390();
}