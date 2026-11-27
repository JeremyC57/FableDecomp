#include <stdio.h>
#include <string>
#include <vector>
#include <map>

typedef std::string CCharString;
struct CWorldMap {
    std::vector<long> requested;
    unsigned long GetMapUID(long i) const {
        const_cast<CWorldMap*>(this)->requested.push_back(i);
        return 8100UL + 7UL * i;
    }
};
struct CWorld {
    CWorldMap world_map;
    const CWorldMap& PeekWorldMap() const { return world_map; }
};
struct CMainGameComponent {
    CWorld world;
    static CMainGameComponent* Get() { static CMainGameComponent component; return &component; }
    const CWorld& PeekWorld() const { return world; }
};
CCharString __fastcall GFUIntToCharString(unsigned long n) {
    char buffer[32];
    sprintf(buffer,"%lu",n);
    return buffer;
}
struct CPersistContext {
    CCharString& script;
    static int alive;
    CPersistContext(CCharString& s) : script(s) { ++alive; }
    ~CPersistContext() { --alive; }
    void Transfer(const char* name,unsigned __int64& value,const unsigned __int64& defaultValue) {
        char buf[90];
        sprintf(buf,"%s=%I64u(default %I64u)\r\n",name,value,defaultValue);
        script += buf;
    }
    void Transfer(const char* name,unsigned long& value,const unsigned long& defaultValue) {
        char buf[90];
        sprintf(buf,"%s=%lu(default %lu)\r\n",name,value,defaultValue);
        script += buf;
    }
};
int CPersistContext::alive=0;
namespace NThingManager {
class CEntityRuntimePersistence {
public:
    struct CEntityScriptInfo {
        bool Valid;
        CCharString Script;
        CEntityScriptInfo(bool valid,const char* text):Valid(valid),Script(text) {}
    };
    struct CPersistedDeadEntity {
        unsigned __int64 UID;
        bool GamePersistent;
        unsigned long SectionNameCRC;
        CPersistedDeadEntity():UID(0),GamePersistent(false),SectionNameCRC(0) {}
        CPersistedDeadEntity(unsigned __int64 id,bool keep,unsigned long crc):UID(id),GamePersistent(keep),SectionNameCRC(crc) {}
    };
    std::vector<CEntityScriptInfo> GamePersistentEntityScripts;
    std::map<unsigned __int64,CPersistedDeadEntity> PersistedDeadEntities;
    const CCharString* GetGamePersistentScript(long i) const {
        if ((unsigned long)i<GamePersistentEntityScripts.size() && GamePersistentEntityScripts[i].Valid) return &GamePersistentEntityScripts[i].Script;
        return 0;
    }
    void SaveToGameStateScript(CCharString&);
};
}
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
int main() {
    typedef NThingManager::CEntityRuntimePersistence State;
    State state;
    CCharString text("prior");
    const char* prefix="\r\nSTART_DEAD_THINGS\r\n";
    const char* suffix="END_DEAD_THINGS\r\n\r\n";
    state.SaveToGameStateScript(text);
    if(text!=CCharString("prior")+prefix+suffix || CPersistContext::alive) return 1;
    state.GamePersistentEntityScripts.push_back(State::CEntityScriptInfo(false,"skip"));
    state.GamePersistentEntityScripts.push_back(State::CEntityScriptInfo(true,"data\r\n"));
    state.GamePersistentEntityScripts.push_back(State::CEntityScriptInfo(true,""));
    state.PersistedDeadEntities[40]=State::CPersistedDeadEntity(4294967301ui64,true,4294967295ul);
    state.PersistedDeadEntities[3]=State::CPersistedDeadEntity(44,true,0);
    state.PersistedDeadEntities[8]=State::CPersistedDeadEntity(45,false,900);
    text="begin\r\n";
    state.SaveToGameStateScript(text);
    const char* expect="begin\r\nSTART_THING_LEVEL 8107\r\ndata\r\nEND_THING_LEVEL\r\n"
        "START_THING_LEVEL 8114\r\nEND_THING_LEVEL\r\n\r\nSTART_DEAD_THINGS\r\n"
        "START_NEW_DEAD_THING\r\nUID=44(default 0)\r\nSectionNameCRC=0(default 0)\r\nEND_NEW_DEAD_THING\r\n"
        "START_NEW_DEAD_THING\r\nUID=4294967301(default 0)\r\nSectionNameCRC=4294967295(default 0)\r\nEND_NEW_DEAD_THING\r\n"
        "END_DEAD_THINGS\r\n\r\n";
    const std::vector<long>& ids=CMainGameComponent::Get()->world.world_map.requested;
    if(text!=expect || ids.size()!=2 || ids[0]!=1 || ids[1]!=2 || CPersistContext::alive) return 2;
    puts("0051eda0_TEST PASS");
    return 0;
}