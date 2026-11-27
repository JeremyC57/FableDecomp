// 005dcd10
// Leaf: ?GetRandomLoadedAnim@CAnimationSet@@QBEPBVCAnimationEntry@@ABVCCharString@@AAK_NABVCAnimFilterBase@@@Z
// Flags: none
#define FABLE_RETAIL_CANIMATIONSET_QUERY_LOADED_PASS29
#define FABLE_RETAIL_CANIMATIONSET_QUERIES
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_C3DANIMATIONINFO_LOADING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterBase.h"
#include "engine/CAnimationSet.h"
#include "engine/GFRandom.h"
#include "fable_assert.h"

#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationInfo.h"

inline bool CAnimationSet::IsAnimLoaded(const CAnimationEntry& entry) const
{
    return CMainGameComponent::Get()->GetWorld().GetMeshBank()->PeekAnimation(entry.BankIndex)->IsLoaded();
}
inline void CAnimationSet::BeginLoading(const CAnimationEntry& entry) const
{
    CMainGameComponent::Get()->GetWorld().GetMeshBank()->PeekAnimation(entry.BankIndex)->BeginLoading(false);
}
const CAnimationEntry* CAnimationSet::GetRandomLoadedAnim(const CCharString& name, unsigned long& seed,
                                                          bool force_something, const CAnimFilterBase& filter) const
{
    std::vector<TAnimMap::const_iterator> found_anims;
    found_anims.reserve(5);
    std::vector<TAnimMap::const_iterator> found_loaded_anims;
    found_loaded_anims.reserve(5);
    TAnimMap::const_iterator start_it = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator it = start_it; it != Anims.end() && it->first == start_it->first; ++it) {
        if (filter(it->second)) {
            found_anims.push_back(it);
            if (IsAnimLoaded(it->second))
                found_loaded_anims.push_back(it);
        }
    }
    long no_anims = found_anims.size();
    FABLE_RETAIL_ASSERT(no_anims>0);
    long index = GFRandom(no_anims, &seed);
    if (IsAnimLoaded(found_anims[index]->second))
        return &found_anims[index]->second;
    BeginLoading(found_anims[index]->second);
    if (!found_loaded_anims.empty()) {
        long loaded_index = GFRandom(found_loaded_anims.size(), &seed);
        return &found_loaded_anims[loaded_index]->second;
    }
    if (force_something)
        return &found_anims[index]->second;
    return 0;
}