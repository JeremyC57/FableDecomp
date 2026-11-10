// CAnimationSet::GetRandomAnim(const CCharString& name, unsigned long& seed, const CAnimFilterBase& filter, long avoidBankIndex) const -- a random filtered animation among those named `name`, avoiding the one in bank slot avoidBankIndex unless it is the only candidate
// Grade: DIFFER(278v264) -- register allocation: retail this=ebp, iterator=edi homed at [esp+28h], &anim=ebx, Anims.end() in esi
// Leaf: ?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DCC00, 264 bytes, __thiscall, `ret 10h`. Ego_r.pdb / FableWin.pdb
//   ?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z.
// Callees: vector<const pair<unsigned long, CAnimationEntry>*>::reserve 0x005E15E0 and ::_M_insert_overflow 0x005E1500,
//   CAnimationSet::FindFirstAnimWithName(const CCharString&) const 0x005DC020, the filter's operator() (slot 0), GFROR13 0x00497890
//   (GFRandom inline), free 0x00BFEA14 (the vector's storage).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterBase.h"
#include "engine/CAnimationSet.h"
#include "engine/GFRandom.h"

const CAnimationEntry& CAnimationSet::GetRandomAnim(const CCharString& name, unsigned long& seed, const CAnimFilterBase& filter,
                                                    long avoidBankIndex) const
{
    std::vector<TAnimMap::const_iterator> candidates;
    TAnimMap::const_iterator avoided = Anims.end();
    candidates.reserve(5);
    TAnimMap::const_iterator it = FindFirstAnimWithName(name);
    TAnimMap::const_iterator first = it;
    while (it != Anims.end() && it->first == first->first) {
        const CAnimationEntry& anim = it->second;
        if (filter(anim)) {
            if (anim.BankIndex == avoidBankIndex)
                avoided = it;
            else
                candidates.push_back(it);
        }
        ++it;
    }
    TAnimMap::const_iterator chosen = candidates.empty() ? avoided : candidates[GFRandom(candidates.size(), &seed)];
    return chosen->second;
}
