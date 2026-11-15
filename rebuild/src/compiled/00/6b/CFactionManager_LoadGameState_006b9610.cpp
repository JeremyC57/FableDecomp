// 006b9610 CFactionManager::LoadGameState
// Flags: none
// Leaf: ?LoadGameState@CFactionManager@@QAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN
#define FABLE_RETAIL_CCRC_PASS22_BODY
#define FABLE_RETAIL_CFACTIONMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_SET_RANGE
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/CFaction.h"
#include "engine/CFactionManager.h"
CCharString FABLE_FASTCALL GFIntToCharString(long value);

void CFactionManager::LoadGameState(CPersistContext& context)
{
    long count;
    context.Transfer("NumberOfFactions",count,0L);
    for(long index=0;index<count;++index) {
        CFaction faction;
        context.Transfer("Faction"+GFIntToCharString(index),faction);
        TFactionMap::iterator found=Factions.Find(faction.GetName().GetCRC());
        if(found!=Factions.end())
            found->second->InitialiseFromFaction(faction);
    }
}