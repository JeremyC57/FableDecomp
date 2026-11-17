// CRandomAppearanceMorph::GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& morph_set) const -- for every skeletal morph group: group 0 adds each morph that allows variation and a random half of the rest; any other group, with a 49% chance, adds one random morph of the group
// Leaf: ?GetRandomSkeletalMorph@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086B9D0, 389 bytes, thiscall `ret 8` (Ego_r.pdb / FableWin.pdb protected const).
// Ego_d random_appearance_morph.obj: locals keys (vector<long>); { it_group { group { upper_bound { it } } { no_morphs, it } } } --
//   spelled here in the same scopes; the else arm's no_morphs is the inline CVectorMap::Count (UpperBound, then LowerBound).
// Same-TU bodies (visible, out of line; the shared CVectorMap.h opt-ins): the const LowerBound (default noinline body; = retail
//   0x0086A930, MATCH at its own address), the const UpperBound (overlay FABLE_RETAIL_CVECTORMAP_UPPERBOUND_BODY; MATCH at 0x0086A980) and
//   GetKeyList (overlay FABLE_RETAIL_CVECTORMAP_GETKEYLIST_BODY; RELOCATION_MATCH at 0x0086D240). With them c2 knows the calls write
//   only keys, so *seed is not reloaded after UpperBound / LowerBound (0x0086BAD4) and `this` keeps ebp (the draft's residue).
// Callees: GetKeyList 0x0086D240, UpperBound 0x0086A980, LowerBound 0x0086A930, GFROR13 0x00497890 (shared inline GFRandom),
//   AddSkeletalMorphToMorphSet 0x0086B900, vector<long>::_M_insert_overflow 0x00440990 (GetKeyList), free 0x00BFEA14.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND_BODY
#define FABLE_RETAIL_CVECTORMAP_COUNT
#define FABLE_RETAIL_CVECTORMAP_GETKEYLIST
#define FABLE_RETAIL_CVECTORMAP_GETKEYLIST_BODY
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_SKELETAL
#include "engine/CRandomAppearanceMorph.h"
#include "engine/GFRandom.h"

void CRandomAppearanceMorph::GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& morph_set) const
{
    std::vector<long> keys;
    SkeletalMorphs.GetKeyList(keys);
    for (std::vector<long>::iterator it_group = keys.begin(); it_group != keys.end(); ++it_group) {
        long group = *it_group;
        if (group == 0) {
            CVectorMap<long, CSkeletalMorph>::const_iterator upper_bound = SkeletalMorphs.UpperBound(group);
            for (CVectorMap<long, CSkeletalMorph>::const_iterator it = SkeletalMorphs.LowerBound(group); it != upper_bound; ++it) {
                if (it->second.AllowVariation || GFRandom(100, &seed) >= 50)
                    AddSkeletalMorphToMorphSet(seed, it->second, morph_set);
            }
        } else if (GFRandom(100, &seed) > 50) {
            long no_morphs = SkeletalMorphs.Count(group);
            CVectorMap<long, CSkeletalMorph>::const_iterator it = SkeletalMorphs.LowerBound(group) + GFRandom(no_morphs, &seed);
            AddSkeletalMorphToMorphSet(seed, it->second, morph_set);
        }
    }
}
