#include "engine/CTCPhysicsBase.h"  // retyped onto the PDB layout; byte parity re-verified

struct CPhysicsMeshTypeView {
    unsigned char _pad_0x00[0x6c];
    unsigned char Flags;
};

typedef int (__fastcall *PhysicsMeshTypeFn)(CTCPhysicsBase* physics);
struct CTCPhysicsBaseVTable {
    unsigned char _pad_0x000[0x174];
    PhysicsMeshTypeFn GetPhysicsMeshType;
};

bool __fastcall IsPhysicsMeshTypeCylinder(CTCPhysicsBase* physics)
{
    if (((CTCPhysicsBaseVTable*)physics->__vftable)->GetPhysicsMeshType(physics) == 0 &&
        (((CPhysicsMeshTypeView*)physics->sub)->Flags & 8))
        return true;
    return false;
}
