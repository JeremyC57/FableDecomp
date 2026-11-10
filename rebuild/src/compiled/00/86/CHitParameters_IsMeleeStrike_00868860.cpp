// CHitParameters::IsMeleeStrike() const -- true when the hit's damage map holds a melee or an unarmed-melee entry
// Leaf: ?IsMeleeStrike@CHitParameters@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868860, length 87 bytes, thiscall.
// Identity (proven): (1) Ego_r.pdb / FableWin.pdb list ?IsMeleeStrike@CHitParameters@@QBE_NXZ beside the other CHitParameters strike
//   tests (IsShotStrike, IsLightningStrike, IsDrainStrike...); (2) the body looks up keys 0 and 1 -- engine/EDamageAttribute.h
//   DAMAGE_MELEE / DAMAGE_MELEE_UNARMED -- in the map at +0x30, Ego_r.pdb CHitParameters::DamageMap (map<EDamageAttribute,float>,
//   Ego_r offset 48), through the map<EDamageAttribute,float> const find 0x006DAB90. Previously landed as the descriptive
//   CStateMapOwner00868860::HasStateZeroOrOne_00868860; engine/CHitParameters.h already declares IsMeleeStrike() const and the landed
//   callers CThingObject::OnHit 0x007039A0 / CThingCreatureBase::OnHit 0x00667D30 call 0x00868860 under it.
// Callees: std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute,float>, ...>::find<EDamageAttribute>(const EDamageAttribute&)
//   const 0x006DAB90 (STLport 4.6 member template, hidden const_iterator result), twice.
// Shape: each find's iterator lives in its own scope, so the two hidden results and keys share one 8-byte frame; end() (the header
//   +0x30) is read once after the first find and kept in edi for both tests. The tree find is declaration only
//   (FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE: STLport 4.6 member template, called out of line).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE
#include "engine/CThing.h"                     // CIntelligentPointer<CThing> members need the complete CThing
#include "engine/CHitParameters.h"

bool CHitParameters::IsMeleeStrike() const
{
    typedef std::map<EDamageAttribute, float>::const_iterator const_iterator;
    const_iterator end;
    {
        const_iterator melee = DamageMap.find(DAMAGE_MELEE);
        end = DamageMap.end();
        if (melee != end)
            return true;
    }
    {
        const_iterator unarmed = DamageMap.find(DAMAGE_MELEE_UNARMED);
        return unarmed != end ? true : false;
    }
}
