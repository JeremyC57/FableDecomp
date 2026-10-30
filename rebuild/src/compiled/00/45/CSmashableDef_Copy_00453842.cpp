#pragma optimize("s",on)
// CSmashableDef::Copy @ 0x00453842
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSmashableDef.h"  // retyped onto the PDB layout; byte parity re-verified

// Base copy (CBaseDef::Copy @ 0x431f10) is invoked on self+0. The generated
// header flattens the base subobject, so model the base call through a cast
// to a local CBaseDef declaring only the member.
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

void __fastcall CSmashableDef_Copy(CSmashableDef* self, void* edx, const CSmashableDef* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)src);
    self->Smashable = src->Smashable;
    self->ReplacementObject_Val = src->ReplacementObject_Val;
    self->SmashParticleEmitter = src->SmashParticleEmitter;
}
