// CCameraCollisionDef::Copy(CDefClassBase const*)
#include "engine/CCameraCollisionDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};


// Base-class Copy, called via rel32.

void __fastcall CCameraCollisionDef_Copy(CCameraCollisionDef* self, void* edx, const CDefClassBase* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->CameraCollisionMesh = ((const CCameraCollisionDef*)src)->CameraCollisionMesh;
}