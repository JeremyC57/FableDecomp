// CThingManager::LoadSingleThing(CPersistContext&,long) -- restore or construct one serialized thing.
// Leaf: ?LoadSingleThing@CThingManager@@AAEPAVCThing@@AAVCPersistContext@@J@Z
#define FABLE_RETAIL_LOAD_SINGLE_LEVEL_ACCESSOR
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_UI_ORDINARY_INLINE
// Flags:
#define FABLE_RETAIL_LOAD_SINGLE_THING
#define FABLE_RETAIL_CTHINGMANAGER_LOAD
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
#include "engine/CThingManager.h"
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
namespace NProgressDisplay {void __fastcall DisplayProgress(const CCharString&,float,bool,bool);}
#pragma auto_inline(off)
template<class T> bool CDefinitionManager::GetDef(const CCharString& name,CDefPointer<const T>* out)const
{
    CDefPointer<const CDefClassBase> def=GetPDefFromInstantiationName(name);
    if(!def.IsNull()){out->SetDynamicCast(def);return true;}
    return false;
}
#pragma auto_inline(on)
CThing* CThingManager::LoadSingleThing(CPersistContext& context,long inclusions)
{
    CCharString name;
    CThing::CSerialiseInfo thing_info;
    CCharString sub_script;
    NProgressDisplay::DisplayProgress("Load Single Thing 1",-1.0f,false,false);
    if(context.IsBinary()) {
        name=context.GetPLoadStream()->ReadNullTerminatedString();
        CThing::SerialiseThingInfo(context,thing_info);
    } else {
        CStringParser& parser=*context.GetPParser();
        parser.SkipWhitespace();
        name=parser.ReadAsStringUntilString(";");
        parser.SkipPastString(";");
        sub_script=parser.ReadAsStringUntilString("EndThing");
        CThing::SerialiseThingInfo(false,sub_script,thing_info);
        parser.SkipPastString("EndThing;");
    }
    NProgressDisplay::DisplayProgress("Load Single Thing 2",-1.0f,false,false);
    bool load_it=true;
    CDefPointer<const CThingBaseDef> pdef;
    if(thing_info.DefinitionType!="NULL") {
        if(PeekDefinitionManager().GetDef(thing_info.DefinitionType,&pdef)) {
            if(!((pdef->IsLoadableGlobal && (inclusions==0 || inclusions==2)) ||
                 (!pdef->IsLoadableGlobal && (inclusions==0 || inclusions==1)))) load_it=false;
        } else {ErrorFlag=true;load_it=false;}
    } else if(inclusions!=0 && inclusions!=2) load_it=false;
    CThing* pout_thing=0;
    if(load_it) {
        bool level_less=false;
        if(!pdef.IsNull()) level_less=pdef->LevelLess;
        unsigned __int64 uid=RemapUIDForSerialisation(false,level_less,GetLevelBeingLoaded(),thing_info.UID);
        CThing* ptemp_thing=GetPThingWithUID(uid);
        bool create_it=true;
        bool is_serialising_hero_state=false;
        if(World.IsSerialisingHeroState() && name=="PlayerCreature") {
            is_serialising_hero_state=true;
            CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            ptemp_thing=phero;
        } else {
            if(ptemp_thing) {
                if(!ptemp_thing->IsToKillOnLevelUnload()) {
                    create_it=false;
                    if(ptemp_thing->IsInLimbo()) {
                        ptemp_thing->SetInLimbo(false);
                        CQuestManager::Get().OnActivateEntity(*ptemp_thing);
                    }
                } else {
                    if(ptemp_thing->IsLevelPersistent()) create_it=false;
                    else CheckAndFixUID(uid);
                }
            }
        }
        if(!is_serialising_hero_state && EntityRuntimePersistence.IsEntityPersistedAsDead(uid)) create_it=false;
        if(create_it) {
            if(!is_serialising_hero_state) {
                NProgressDisplay::DisplayProgress("Load Single Thing: Allocate Class",-1.0f,false,false);
                CClass* pclass=AllocateClass(name);
                if(pclass) ptemp_thing=static_cast<CThing*>(pclass);
            }
        if(ptemp_thing) {
            NProgressDisplay::DisplayProgress("Load Single Thing: Construct Thing",-1.0f,false,false);
            if(context.IsBinary()) ptemp_thing->ConstructFromBinarySave(*context.GetPLoadStream(),thing_info);
            else ptemp_thing->ConstructFromLevelScript(sub_script);
            NProgressDisplay::DisplayProgress("Load Single Thing: Initial Activate",-1.0f,false,false);
            if(is_serialising_hero_state) {
                ptemp_thing->InitialActivate();
                ptemp_thing->OnInitialActivate();
                pout_thing=ptemp_thing;
            } else if(ptemp_thing->OnCreate()) {
                ptemp_thing->SetSerialisationSectionName(CurrentSectionName);
                pout_thing=ptemp_thing;
            } else ptemp_thing->Kill(true);
        }
        } else pdef.Release();
    }
    NProgressDisplay::DisplayProgress("Load Single Thing 3",-1.0f,false,false);
    return pout_thing;
}
