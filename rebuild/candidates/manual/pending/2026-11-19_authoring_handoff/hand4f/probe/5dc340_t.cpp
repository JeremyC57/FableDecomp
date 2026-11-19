// CAnimationSet::GetAnim(const CCharString& name, long entry_num, const CAnimFilterBase& filter) const -- probe
// Leaf: ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CANIMATIONSET_GETNOANIMS
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#include "fable_string.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterBase.h"
#include "engine/CAnimationSet.h"

long CAnimationSet::GetNoAnims(const CCharString& name, const CAnimFilterBase& filter) const
{
    long count = 0;
    TAnimMap::const_iterator first = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator it = first; it != Anims.end() && it->first == first->first; ++it)
        if (filter(it->second))
            ++count;
    return count;
}
bool CAnimationSet::AnimExists(const CCharString& name, const CAnimFilterBase& filter) const
{
    TAnimMap::const_iterator it = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator i = it; i != Anims.end() && i->first == it->first; ++i)
        if (filter(i->second))
            return true;
    return false;
}
const CAnimationEntry& CAnimationSet::GetAnim(const CCharString& name, long entry_num, const CAnimFilterBase& filter) const
{
    TAnimMap::const_iterator start_it = FindFirstAnimWithName(name);
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

CAnimationSet::TAnimMap::const_iterator CAnimationSet::FindFirstAnimWithName(const CCharString& name) const
{
    unsigned long key = name.GetCRC();
    TAnimMap::const_iterator it = Anims.LowerBound(key);
    if (it == Anims.end() || it->first != key)
        return Anims.end();
    return it;
}
