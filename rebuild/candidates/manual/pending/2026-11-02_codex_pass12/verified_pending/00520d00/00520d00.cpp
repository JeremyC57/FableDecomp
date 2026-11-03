// CThingManager::LoadEntitiesFromScript(const CCharString&,long,long,const std::set<CCharString>&,CASuspendableProcess*,long,std::vector<CThing*>&) -- Load selected script sections and activate their entities.
// Leaf: ?LoadEntitiesFromScript@CThingManager@@QAEXABVCCharString@@JJABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@PAVCASuspendableProcess@@JAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@4@@Z
#define FABLE_RETAIL_UI_ORDINARY_INLINE
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_LOAD_ENTITIES_SCRIPT
#define FABLE_RETAIL_CTHINGMANAGER_LOAD
#define FABLE_RETAIL_CTHINGMANAGER_TEXT
#define FABLE_STLPORT_VISIBLE_PUSH_BACK
#define FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
#define FABLE_STLPORT_USER_TRUE_TAG
#define FABLE_RETAIL_CTHINGMANAGER_BINARY
#define FABLE_RETAIL_CTHINGMANAGER_SAVE
#define FABLE_RETAIL_INVENTORY_MAP_SERIALISE
#define FABLE_RETAIL_ENTITY_PERSIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include <string.h>
#include "rebuild_abi.h"
#include "engine/CThingManager.h"
#include "engine/CLevelLoader.h"
#include "engine/CThing.h"
#include "engine/CPersistContext.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CStringParser.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CQuestManager.h"
#pragma auto_inline(off)
template<> __declspec(noinline) bool CBasicString<char>::operator<(const CBasicString<char>& other)const {return Compare(PData,other.PData)<0;}
#pragma auto_inline(on)

namespace NProgressDisplay {void __fastcall DisplayProgress(const CCharString&,float,bool,bool);}

namespace NGlobalConsole {extern long NoThingsToLoadPerSuspend;}
void CThingManager::LoadEntitiesFromScript(const CCharString& script,long version,long level,const std::set<CCharString>& sections,CASuspendableProcess* psuspend,long inclusions,std::vector<CThing*>& loaded_things)
{
    unsigned long how_many_is_some=NGlobalConsole::NoThingsToLoadPerSuspend;
    SerialiseVersionNumber=version;
    LevelBeingLoaded=level;
    bool loading_with_suspends=psuspend!=0;
    CStringParser parser(script,0,CCharString(""),0);
    long count=0;
    CurrentSectionName="";
    if(version>=2) {
        while(parser.SkipPastString("XXXSectionStart")) {
            parser.SkipWhitespace();
            CCharString name=parser.ReadAsStringUntilString(";");
            parser.SkipPastString(";");
            if(sections.find(name)!=sections.end() || sections.find(CLevelLoader::GetLoadAllSectionName())!=sections.end()) {
                CurrentSectionName=name;
                if(parser.SkipPastNextItemIfIs("XXXSectionEnd;")) {
                    parser.ReadAsStringUntilPastString(";");
                    CurrentSectionName="";
                } else {
                    while(parser.SkipPastNextItemIfIs("NewThing")) {
                        NProgressDisplay::DisplayProgress("Loading entities from script",-1.0f,false,false);
                        CPersistContext thing_context(parser,0,true,0,0,0);
                        CThing* pthing=LoadSingleThing(thing_context,inclusions);
                        if(pthing) loaded_things.push_back(pthing);
                        if(++count>=how_many_is_some) {
                            count=0;
                            if(loading_with_suspends)psuspend->SuspendSuspendableProcess();
                        }
                    }
                    parser.SkipPastString("XXXSectionEnd;");
                }
            } else parser.SkipPastString("XXXSectionEnd;");
        }
    } else {
        CurrentSectionName="NULL";
        while(parser.SkipPastString("NewThing")) {
            NProgressDisplay::DisplayProgress("Loading entities from script",-1.0f,false,false);
            CPersistContext thing_context(parser,0,true,0,0,0);
            CThing* pthing=LoadSingleThing(thing_context,inclusions);
            if(pthing) loaded_things.push_back(pthing);
            ++count;
            if(loading_with_suspends && count>=how_many_is_some) {
                count=0;
                psuspend->SuspendSuspendableProcess();
            }
        }
    }
    if(psuspend)psuspend->SuspendSuspendableProcess();
    ActivateAfterLoading(level);
}
