// CRandomAppearanceMorph::GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& morph_set) const -- for every skeletal morph group: group 0 adds each morph that does not allow variation and a random half of the rest; any other group, with a 49% chance, adds one random morph of the group
// Grade: DIFFER(ours v retail) -- 389v389, ported onto engine/CRandomAppearanceMorph.h + CVectorMap.h + GFRandom.h (PDB names: GetRandomSkeletalMorph is IBE const, the callee is AddSkeletalMorphToMorphSet 0x0086B900, GetKeyList takes std::vector<long>&): same register residue as the local model -- retail this ebp, the map (this+18h) ebx and the UpperBound result spilled to [esp+18h]; ours map ebp, end ebx (c2prio: map temp prio 20 colours before end 4 and this -22; the ebx/ebp choice for the map is the decision to move)
// Leaf: ?GetRandomSkeletalMorph@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@@Z
// Retail: Fable.exe 0x0086B9D0, 389 bytes, thiscall `ret 8` (Ego_r.pdb name above; caller GetRandomMorphSet 0x0086BF7F).
// Callees: CVectorMap<long,CSkeletalMorph>::GetKeyList 0x0086D240 / LowerBound 0x0086A930 / UpperBound 0x0086A980,
//   GFROR13 0x00497890 (inside the inline GFRandom), AddSkeletalMorphToMorphSet 0x0086B900, free 0x00BFEA14 (the vector's deallocate).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CVECTORMAP_GETKEYLIST
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_SKELETAL
#include "engine/CRandomAppearanceMorph.h"
#include "engine/GFRandom.h"

void CRandomAppearanceMorph::GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& morph_set) const
{
    std::vector<long> group_ids;
    SkeletalMorphs.GetKeyList(group_ids);
    for (std::vector<long>::const_iterator it = group_ids.begin(); it != group_ids.end(); ++it) {
        long group_id = *it;
        if (group_id == 0) {
            CVectorMap<long, CSkeletalMorph>::const_iterator end = SkeletalMorphs.UpperBound(group_id);
            for (CVectorMap<long, CSkeletalMorph>::const_iterator morph = SkeletalMorphs.LowerBound(group_id); morph != end; ++morph) {
                if (!morph->second.AllowVariation && GFRandom(100, &seed) < 50)
                    continue;
                AddSkeletalMorphToMorphSet(seed, morph->second, morph_set);
            }
        } else if (GFRandom(100, &seed) > 50) {
            unsigned long count = SkeletalMorphs.UpperBound(group_id) - SkeletalMorphs.LowerBound(group_id);
            unsigned long index = GFRandom(count, &seed);
            AddSkeletalMorphToMorphSet(seed, SkeletalMorphs.LowerBound(group_id)[index].second, morph_set);
        }
    }
}
