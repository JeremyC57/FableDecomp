// Leaf: ??_GCEngineInternalPrimitiveParticleGroup@@UAEPAXI@Z
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_PARTICLE_GROUP_DESTRUCTOR_PASS29
#include "fable_compiler.h"
#include "engine/CEnginePrimitiveEnums.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_ORIGINAL_TYPES
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_DECLS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_SLOTS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETCHILDPRIMITIVE_ORDER
#include "engine/CEngineInternalPrimitiveParticleGroup.h"
// Inline-budget: 00BBA1E0 expands both list destructors. The list<long> base destructor
// and constructor are original out-of-line STLport copies at 0042AB73 and 0042AB58.
namespace std {
template<> _List_base<long,allocator<long> >::_List_base(const allocator<long>&);
template<> _List_base<long, allocator<long> >::~_List_base();


}


#include "engine/CEngineInternalPrimitiveParticleDecalGroup.h"
CEngineInternalPrimitiveParticleDecalGroup::CEngineInternalPrimitiveParticleDecalGroup()
    : CEngineInternalPrimitiveParticleGroup(ENGINE_PRIMITIVE_PARTICLE_DECAL_GROUP, EPF_DISABLE_OCCLUSION_TESTING) { }

CEngineInternalPrimitiveParticleGroup::CEngineInternalPrimitiveParticleGroup(EEnginePrimitiveType type, unsigned long flags)
    : CEngineInternalPrimitiveBase(type, flags)
{
}