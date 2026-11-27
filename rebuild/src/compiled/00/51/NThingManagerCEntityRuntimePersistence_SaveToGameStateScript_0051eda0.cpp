// Leaf: ?SaveToGameStateScript@CEntityRuntimePersistence@NThingManager@@QAEXAAVCCharString@@@Z
// Flags: (none)
// Iteration uses the shared size-built STLport increment copy (004292C0).
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_ENTITY_SAVE_SCRIPT_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_STRINGSAVE
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#include "engine/NThingManager_CEntityRuntimePersistence.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"
CCharString __fastcall GFUIntToCharString(unsigned long);
void NThingManager::CEntityRuntimePersistence::SaveToGameStateScript(CCharString& script)
{
    const CWorldMap& world_map=CMainGameComponent::Get()->PeekWorld().PeekWorldMap();
    for(long c1=0;c1<GamePersistentEntityScripts.size();++c1) {
        const CCharString* pstr=GetGamePersistentScript(c1);
        if (pstr) {
            unsigned long map_uid=world_map.GetMapUID(c1);
            script += "START_THING_LEVEL " + GFUIntToCharString(map_uid) + "\r\n";
            script += *pstr;
            script += "END_THING_LEVEL\r\n";
        }
    }
    script += "\r\nSTART_DEAD_THINGS\r\n";
    CPersistContext context(script);
    typedef std::map<unsigned __int64,CPersistedDeadEntity> DeadEntities;
    for(DeadEntities::iterator it_dead=PersistedDeadEntities.begin();it_dead!=PersistedDeadEntities.end();++it_dead) {
        if(it_dead->second.GamePersistent) {
            script+="START_NEW_DEAD_THING\r\n";
            context.Transfer("UID",it_dead->second.UID,(unsigned __int64)0);
            context.Transfer("SectionNameCRC",it_dead->second.SectionNameCRC,0ul);
            script+="END_NEW_DEAD_THING\r\n";
        }
    }
    script+="END_DEAD_THINGS\r\n\r\n";
}