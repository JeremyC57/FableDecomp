// CCameraCollisionDef::Copy(CDefClassBase const*)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCameraCollisionDef.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)


// Base-class Copy, called via rel32.

void __fastcall CCameraCollisionDef_Copy(CCameraCollisionDef* self, void* edx, const CDefClassBase* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->CameraCollisionMesh = ((const CCameraCollisionDef*)src)->CameraCollisionMesh;
}