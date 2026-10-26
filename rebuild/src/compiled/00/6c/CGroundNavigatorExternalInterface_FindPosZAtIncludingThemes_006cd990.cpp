#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class C3DVector { public: float x, y, z; };
class CThingCreatureBase { public: float FindPosZAt(const C3DVector& v, bool a, bool b); };
float __fastcall CGroundNavigatorExternalInterface_FindPosZAtIncludingThemes(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& v, bool inc)
{
    return self->Creature->FindPosZAt(v, true, inc);
}