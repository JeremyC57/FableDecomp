// CThingManager::SaveGameState(CPersistContext& context) -- save persistent entity and NPC-name state.
// Leaf: ?SaveGameState@CThingManager@@QAEXAAVCPersistContext@@@Z
// Grade: DIFFER(1500v1553) -- inline budget: CPersistContext::Transfer<long>/<unsigned __int64> stay calls (retail inlines them: CheckCRC 0x00404500, WriteSLONG/WriteUHUGE, traits readers 0x0040FE60/0x0044B890) even with explicit inline specializations; frame 0x4c vs 0x50
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00524290/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00524290 probes in probe/00524290.log
#define FABLE_RETAIL_CTHINGMANAGER_SAVE
#define FABLE_RETAIL_ENTITY_PERSIST
#define FABLE_RETAIL_CTHINGMANAGER_BINARY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CARRAY
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_CONSTRUCTORS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CQuestManager.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistContextWriteSection.h"
#include "engine/CPersistTraits.h"
CCharString __fastcall GFIntToCharString(long);
CCharString __fastcall GFUIntToCharString(unsigned long);
CCharString __fastcall GFUHUGEToCharString(unsigned __int64);
CCharString __fastcall operator+(const char*,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
#pragma auto_inline(off)
void CPersistContext::CheckCRC(const char* name) {
    if(Mode==MODE_LOAD_BINARY) PLoadStream->ReadULONG();
    else if(Mode==MODE_SAVE_BINARY) PSaveStream->WriteULONG(CCharString(name).GetCRC());
}
template<> void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream,long& val) { val=stream.ReadSLONG(); }
void CPersistTraits<unsigned __int64>::TransferBinaryIn(CMemoryDataInputStream& stream,unsigned __int64& val) { val=stream.ReadType<unsigned __int64>(); }
#pragma auto_inline(on)
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }
void CThingManager::SaveGameState(CPersistContext& context) {
    bool binary=context.IsBinary();
    long version=2;
    NThingManager::CEntityRuntimePersistence old_persistence;
    if(World.IsSerialisingHeroState()) {
        old_persistence=EntityRuntimePersistence;
        EntityRuntimePersistence.Clear();
        RuntimeSaveGameHero(binary);
    } else {
        CThingPlayerCreature* phero=PlayerManager.GetMainPlayer().GetPControlledCreature();
        if(IS_THING_ALIVE(phero)) CQuestManager::Get().ResetPositionOfAllQuestCards();
        RuntimeSaveGamePersistentEntitiesInLoadedMaps(binary);
    }
    if(binary) {
        if(!World.IsSerialisingHeroState()) {
            context.Transfer(version);
            context.Transfer(CurrentUniqueID);
        }
        CPersistContextWriteSection section;
        context.StartSection(section,"SAVED_ENTITIES");
        EntityRuntimePersistence.SaveToGameStateBinary(context);
        context.EndSection(section);
        context.StartSection(section,"SAVED_NPC_NAMES");
        if(!World.IsSerialisingHeroState()) {
            context.GetPSaveStream()->WriteULONG(NPCNameMap.size());
            for(std::map<unsigned __int64,std::pair<CCharString,CWideString> >::iterator it=NPCNameMap.begin();it!=NPCNameMap.end();++it) {
                context.GetPSaveStream()->WriteUHUGE(it->first);
                if(it->second.first!="") context.GetPSaveStream()->WritePresizedString(it->second.first);
                else context.GetPSaveStream()->WritePresizedString("UNATTACHED");
                context.GetPSaveStream()->WriteNullTerminatedWString(it->second.second);
            }
        }
        context.EndSection(section);
    } else {
        CCharString script;
        if(!World.IsSerialisingHeroState()) {
            script+="TNGGameStateVersion "+GFIntToCharString(2)+";\r\n";
            script+="GlobalUIDCount "+GFIntToCharString((long)CurrentUniqueID)+";\r\n\r\n";
        }
        script+="START_SAVED_ENTITIES\r\n\r\n";
        EntityRuntimePersistence.SaveToGameStateScript(script);
        script+="END_SAVED_ENTITIES\r\n\r\n";
        context.WriteString(script);
        script="START_SAVED_NPC_NAMES\r\n\r\n";
        if(!World.IsSerialisingHeroState()) {
            script+=GFUIntToCharString(NPCNameMap.size())+"\r\n";
            for(std::map<unsigned __int64,std::pair<CCharString,CWideString> >::iterator it=NPCNameMap.begin();it!=NPCNameMap.end();++it) {
                script+=GFUHUGEToCharString(it->first)+"\r\n";
                if(it->second.first!="") script+=it->second.first+"\r\n";
                else script+="UNATTACHED\r\n";
                script+=it->second.second.ToCharString()+"\r\n";
            }
        }
        script+="END_SAVED_NPC_NAMES\r\n\r\n";
        context.WriteString(script);
    }
    if(World.IsSerialisingHeroState()) EntityRuntimePersistence=old_persistence;
}
