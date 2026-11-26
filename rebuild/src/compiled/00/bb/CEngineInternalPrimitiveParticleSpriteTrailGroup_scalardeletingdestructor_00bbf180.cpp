// CEngineInternalPrimitiveParticleSpriteTrailGroup::`scalar deleting destructor'(unsigned int flags) -- the inlined ~CEngineInternalPrimitiveParticleSpriteTrailGroup (no members of its own to destroy) reduces to the base
//   ~CEngineInternalPrimitiveParticleGroup; then operator delete when bit 0 of flags is set; returns this
// Leaf: ?CEngineInternalPrimitiveParticleSpriteTrailGroup_scalar_deleting_destructor@@YIPAXPAVCEngineInternalPrimitiveParticleSpriteTrailGroup@@HI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBF180, `ret 4`; vftable 0x012A5BFC slot 0; calls 0x00BBA140 (the base class destructor) and operator delete 0x00BFE9BC.
// Naming (NF456/NF457, r454b unit with 0x00BBA140; ruling r456b): retail calls the base destructor ??1CEngineInternalPrimitiveParticleGroup@@UAE@XZ (Ego_r + FableWin name sets; virtual)
//   directly, written as the qualified call `group->CEngineInternalPrimitiveParticleGroup::~CEngineInternalPrimitiveParticleGroup()` (direct, not through the vftable);
//   was the free stand-in CEngineInternalPrimitiveParticleSpriteTrailGroup_dtor, which bound no retail address. The self stays the free stand-in: retail's ??_GCEngineInternalPrimitiveParticleSpriteTrailGroup@@UAEPAXI@Z
//   is compiler-generated and only emitted beside the vftable (the ctor-owned thin-row route), which is not this row's object.
// Model (NF457 / NF459, rulings r456b, r457b): the classes come from the shared headers -- the base from engine/CEngineInternalPrimitiveParticleGroup.h with the
//   add-only containers-free view FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_NO_CONTAINERS (NF459, r457b on r451a: no STLport, so no /DEFAULTLIB:"libcp";
//   same members and offsets, sizeof 0xAC; declares the out-of-line virtual destructor 0x00BBA140),
//   the derived class from engine/CEngineInternalPrimitiveParticleSpriteTrailGroup.h (retail key `class`, as both PDBs; the stand-in self's parameter re-keys PAU -> PAV).
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLESPRITETRAILGROUP_VIRTUAL_MODEL
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETCHILDPRIMITIVE_ORDER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_DECLS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_SLOTS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_NO_CONTAINERS
#include "engine/CEngineInternalPrimitiveParticleSpriteTrailGroup.h"

void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveParticleSpriteTrailGroup_scalar_deleting_destructor(CEngineInternalPrimitiveParticleSpriteTrailGroup* group, int edx_unused, unsigned int flags)
{
    group->CEngineInternalPrimitiveParticleGroup::~CEngineInternalPrimitiveParticleGroup();
    if (flags & 1)
        operator delete(group);
    return group;
}
