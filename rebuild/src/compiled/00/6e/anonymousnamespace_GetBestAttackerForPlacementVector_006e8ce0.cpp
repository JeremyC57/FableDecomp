// GetBestAttackerForPlacementVector -- select the best aligned ground-plane attacker
// Flags:
// Retail: 006e8ce0, 294 bytes, anonymous namespace fastcall, three arguments.
// Leaf: ?GetBestAttackerForPlacementVector@?A0xd8b789b9@@YIPBVCThingCreatureBase@@ABV?$list@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@std@@ABVC3DVector@@1@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
namespace {
const CThingCreatureBase* FABLE_FASTCALL GetBestAttackerForPlacementVector(
    const std::list<const CThingCreatureBase*>& closest_attackers,
    const C3DVector& defender_pos, const C3DVector& placement_vector)
{
    const CThingCreatureBase* pbest_creature=0;
    float best_value=-(1.0f+DELTA_NOTIONAL_ZERO);
    for(std::list<const CThingCreatureBase*>::const_iterator it=closest_attackers.begin();it!=closest_attackers.end();++it) {
        const CThingCreatureBase* pcreature_base=*it;
        C3DVector norm_placement_vector(placement_vector);
        norm_placement_vector.Z=0.0f;
        float mag_placement=norm_placement_vector.GetAccurateMagnitude();
        norm_placement_vector/=mag_placement;
        C3DVector defender_to_attacker(pcreature_base->GetPos()-defender_pos);
        defender_to_attacker.Z=0.0f;
        float mag_distance=defender_to_attacker.GetAccurateMagnitude();
        defender_to_attacker/=mag_distance;
        float cos_thet=norm_placement_vector.Dot(defender_to_attacker);
        if(cos_thet < -1.0f) cos_thet=-1.0f;
        else if(cos_thet > 1.0f) cos_thet=1.0f;
        float delta_mag=(float)fabs(mag_placement-mag_distance);
        float this_value=cos_thet;
        if(this_value>best_value) {pbest_creature=pcreature_base;best_value=this_value;}
    }
    return pbest_creature;
}
}
// TU emission anchor only: forces the original internal-linkage function to be emitted.
// This is reconstruction scaffolding, not an original game global or relocation binding.
const CThingCreatureBase* (FABLE_FASTCALL* g_pGetBestAttackerForPlacementVector)(const std::list<const CThingCreatureBase*>&,const C3DVector&,const C3DVector&)=&GetBestAttackerForPlacementVector;