#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <utility>
#include <string.h>
#ifndef _MSC_VER
#define __int64 long long
#endif
typedef unsigned __int64 UID;
static const UID first_uid=((UID)1<<36)+7, second_uid=9;
static void check(bool b){if(!b)abort();}
struct CWideString{std::string text;CWideString(const char* s=""):text(s){}};
struct CCharString{
    std::string text;CCharString(const char* s=""):text(s){}
    const char* GetAsCharArray()const{return text.c_str();}
    char* GetAsCharArray(){static char empty=0;return text.empty()?&empty:&text[0];}
    CCharString& operator+=(const CCharString& more){text+=more.text;return *this;}
    void Replace(const CCharString& What,const CCharString& With) {
        CCharString ret_str;
        char* data = (char*)GetAsCharArray();
        char* what = (char*)What.GetAsCharArray();
        char* prev = data;
        char* res = strstr(data,what);
        while (res) {
            *res=0;
            res+=strlen(what);
            ret_str+=CCharString(prev);
            ret_str+=With;
            prev=res;
            res=strstr(res,what);
        }
        ret_str+=CCharString(prev);
        *this=ret_str;
    }
    bool operator==(const char* s)const{return text==s;}
    CWideString ToWideString()const{return CWideString(text.c_str());}
};
static long GFCharStringToInt(const CCharString& s){return atol(s.text.c_str());}
static UID GFCharStringToUHUGE(const CCharString& s){UID v=0;for(unsigned i=0;i<s.text.size();++i){check(s.text[i]>='0'&&s.text[i]<='9');v=v*10+(s.text[i]-'0');}return v;}
static int population,text_pos,binary_pos,sections,dispatch,metadata;
struct CStringParser {
    const char* ReadLineAsString(){static const char* lines[]={"3\r\n","68719476743\r\n","UNATTACHED\r\n","First\r\n","9\r\n","DEF\r\n","An\r\nn\r\n","68719476743\r\n","LAST\r\n","Bob\r\n"};
        if(!population){check(text_pos++==0);return "0\r\n";}check(text_pos<10);return lines[text_pos++];}
};
struct CDataInputStream{
    unsigned int ReadULONG(){check(binary_pos++==0);return population;}
    UID ReadUHUGE(){int i=(binary_pos-1)/3;check((binary_pos++-1)%3==0);return i==1?second_uid:first_uid;}
    CCharString ReadPresizedString(){int i=(binary_pos-1)/3;check((binary_pos++-1)%3==1);return CCharString(i==0?"UNATTACHED":i==1?"DEF":"LAST");}
    CWideString ReadNullTerminatedWString(){int i=(binary_pos-1)/3;check((binary_pos++-1)%3==2);return CWideString(i==0?"First":i==1?"Ann":"Bob");}
};
class CPersistContextReadSection;
class CPersistContext{public:bool binary;int mask;bool* loading;bool* serial;
    bool IsBinary()const{return binary;}
    bool ReadSection(CPersistContextReadSection&,const char* s){check(*loading && *serial && sections<2 && !strcmp(s,sections?"SAVED_NPC_NAMES":"SAVED_ENTITIES"));return (mask&(1<<sections++))!=0;}
    void Transfer(const char* s,long& v,long fallback){check(!strcmp(s,"TNGGameStateVersion") && v==2 && fallback==0 && *loading && *serial);v=4;++metadata;}
    void Transfer(const char* s,UID& v,UID fallback){check(!strcmp(s,"GlobalUIDCount") && v==52 && !fallback && *loading && *serial);v=first_uid;++metadata;}
};
struct CPersistContextReadSection{CPersistContext Context;CStringParser Parser;CDataInputStream InputStream;};
struct CWorld{bool hero;bool IsSerialisingHeroState()const{return hero;}};
struct RuntimePersistence{
    long SaveFormatVersion;
    void LoadFromGameStateBinary(CPersistContext&){check(!dispatch);dispatch=1;}
    void LoadFromGameStateScript(CStringParser&){check(!dispatch);dispatch=2;}
};
class CThingManager{public:
    bool CurrentlyLoading,SerialisingGameState;CWorld World;UID CurrentUniqueID;RuntimePersistence EntityRuntimePersistence;
    std::map<UID,std::pair<CCharString,CWideString> > NPCNameMap;int names;
    void SetNameForThing(UID id,const CWideString& name){check(CurrentlyLoading && SerialisingGameState);if(!names)check(id==first_uid&&name.text=="First");NPCNameMap[id].second=name;++names;}
    void LoadHeroStateBinary(CPersistContext&){check(!dispatch);dispatch=3;}
    void LoadHeroStateScript(CStringParser&){check(!dispatch);dispatch=4;}
    void LoadGameState(CPersistContext&);
};
void CThingManager::LoadGameState(CPersistContext& context)
{
    CurrentlyLoading = true;
    SerialisingGameState = CurrentlyLoading;
    if (!World.IsSerialisingHeroState()) {
        long version = 2;
        context.Transfer("TNGGameStateVersion", version, 0L);
        EntityRuntimePersistence.SaveFormatVersion = version;
        context.Transfer("GlobalUIDCount", CurrentUniqueID, (unsigned __int64)0);
    }
    CPersistContextReadSection section;
    if (context.ReadSection(section, "SAVED_ENTITIES")) {
        if (World.IsSerialisingHeroState()) {
            if (context.IsBinary())
                LoadHeroStateBinary(section.Context);
            else
                LoadHeroStateScript(section.Parser);
        } else {
            if (context.IsBinary())
                EntityRuntimePersistence.LoadFromGameStateBinary(section.Context);
            else
                EntityRuntimePersistence.LoadFromGameStateScript(section.Parser);
        }
    }
    if (context.ReadSection(section, "SAVED_NPC_NAMES")) {
        if (!World.IsSerialisingHeroState()) {
        if (context.IsBinary()) {
            unsigned long map_population = section.InputStream.ReadULONG();
            for (int i = 0; i < map_population; ++i) {
                unsigned __int64 uid = section.InputStream.ReadUHUGE();
                CCharString passport = section.InputStream.ReadPresizedString();
                CWideString stored_name = section.InputStream.ReadNullTerminatedWString();
                SetNameForThing(uid, stored_name);
                NPCNameMap[uid].first = passport;
            }
        } else {
            CCharString str;
            str = section.Parser.ReadLineAsString();
            str.Replace("\r\n", "");
            unsigned long map_population = GFCharStringToInt(str);
            for (int i = 0; i < map_population; ++i) {
                str = section.Parser.ReadLineAsString();
                str.Replace("\r\n", "");
                unsigned __int64 uid = GFCharStringToUHUGE(str);
                str = section.Parser.ReadLineAsString();
                str.Replace("\r\n", "");
                if (str == "UNATTACHED")
                    str = "";
                CCharString passport(str);
                str = section.Parser.ReadLineAsString();
                str.Replace("\r\n", "");
                CWideString stored_name = str.ToWideString();
                SetNameForThing(uid, stored_name);
                NPCNameMap[uid].first = passport;
            }
        }
        }
    }
    SerialisingGameState = false;
    CurrentlyLoading = false;
}

int main(){
    for(int hero=0;hero<2;++hero)for(int binary=0;binary<2;++binary)for(int mask=0;mask<4;++mask)for(int count=0;count<2;++count){
        CThingManager m;CPersistContext ctx;
        ctx.loading=&m.CurrentlyLoading;ctx.serial=&m.SerialisingGameState;ctx.mask=mask;ctx.binary=binary!=0;
        m.CurrentlyLoading=m.SerialisingGameState=false;m.World.hero=hero!=0;m.CurrentUniqueID=52;m.EntityRuntimePersistence.SaveFormatVersion=6;m.names=0;
        population=count?3:0;text_pos=binary_pos=sections=dispatch=metadata=0;
        m.LoadGameState(ctx);
        check(!m.CurrentlyLoading && !m.SerialisingGameState && sections==2 && metadata==(hero?0:2));
        check(m.CurrentUniqueID==(hero?52:first_uid) && m.EntityRuntimePersistence.SaveFormatVersion==(hero?6:4));
        check(dispatch==(mask&1?(hero?2:0)+(binary?1:2):0));
        bool read=(!hero && (mask&2));
        check(text_pos==(read&&!binary?1+population*3:0) && binary_pos==(read&&binary?1+population*3:0));
        check(m.names==(read?population:0));
        if(m.names){check(m.NPCNameMap.size()==2 && m.NPCNameMap[first_uid].first.text=="LAST" && m.NPCNameMap[first_uid].second.text=="Bob");check(m.NPCNameMap[second_uid].first.text=="DEF" && m.NPCNameMap[second_uid].second.text=="Ann");}
        else check(m.NPCNameMap.empty());
    }
    puts("005253f0_TEST PASS");return 0;
}