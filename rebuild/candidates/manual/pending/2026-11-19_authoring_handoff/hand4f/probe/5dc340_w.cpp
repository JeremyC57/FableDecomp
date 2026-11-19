// CAnimationSet::GetAnim(const CCharString& name, long entry_num, const CAnimFilterBase& filter) const -- probe
// Leaf: ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CANIMATIONSET_GETNOANIMS
#include "fable_string.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterBase.h"
#include "engine/CAnimationSet.h"

const CAnimationEntry& CAnimationSet::GetAnim(const CCharString& name, long entry_num, const CAnimFilterBase& filter) const
{
    TAnimMap::const_iterator start_it = FindFirstAnimWithName(name);
    __assume(start_it != Anims.end());
    for (TAnimMap::const_iterator it = start_it; ; ++it)
    {
        if (filter(it->second))
        {
            if (entry_num == 0)
                return it->second;
            entry_num--;
        }
    }
}
