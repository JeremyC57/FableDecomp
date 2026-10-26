#include "engine/CGroundNavigatorExternalInterface.h"

class C3DVector { public: float x, y, z; };
class CThingCreatureBase { public: float FindPosZ(const C3DVector& pos, int flag, bool b); };

float __fastcall FindPosZAt(CGroundNavigatorExternalInterface* self, void* /*edx*/, const C3DVector& pos, bool b)
{
    return self->Creature->FindPosZ(pos, 0, b);
}
