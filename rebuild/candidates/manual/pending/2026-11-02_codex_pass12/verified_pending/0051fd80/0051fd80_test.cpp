// Standalone C++03 behavior fixture. The selected production member body is pasted unchanged.
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#ifndef _MSC_VER
#define __int64 long
typedef char HostLongMustBe64Bit[(sizeof(long)==8)?1:-1];
#define __fastcall
#endif
static std::vector<std::string> Events;
class CCharString{public:std::string Text;CCharString(){}CCharString(const char*s,long n=-1):Text(n<0?std::string(s):std::string(s,n)){}operator const char*()const{return Text.c_str();}bool operator==(const char*s)const{return Text==s;}bool operator!=(const char*s)const{return Text!=s;}};
class CThingBaseDef{public:bool IsLoadableGlobal,LevelLess;mutable long RefCount;CThingBaseDef():IsLoadableGlobal(false),LevelLess(false),RefCount(1){}};
template<class T>class CDefPointer{T*Object;CDefPointer(const CDefPointer&);CDefPointer&operator=(const CDefPointer&);public:CDefPointer():Object(0){}~CDefPointer(){Release();}void Release(){if(Object){--Object->RefCount;Object=0;Events.push_back("Release");}}void Set(T*p){assert(!Object);Object=p;++Object->RefCount;}bool IsNull()const{return !Object;}T*operator->()const{return Object;}};
class CGameDefinitionManager{public:bool Found;CThingBaseDef*Def;mutable unsigned Calls;CGameDefinitionManager():Found(true),Def(0),Calls(0){}bool GetDef(const CCharString&name,CDefPointer<const CThingBaseDef>*out)const{assert(name.Text=="OBJECT_DEF");++Calls;Events.push_back("GetDef");if(!Found)return false;out->Set(Def);return true;}};
class CStringParser{public:CCharString Name,Definition;unsigned Step;CStringParser():Name("Object"),Definition("OBJECT_DEF"),Step(0){}void SkipWhitespace(){assert(Step++==0);}CCharString ReadAsStringUntilString(const CCharString&delim){if(delim.Text==";"){assert(Step++==1);return Name;}assert(delim.Text=="EndThing"&&Step++==3);return CCharString("script");}bool SkipPastString(const CCharString&delim){if(delim.Text==";")assert(Step++==2);else assert(delim.Text=="EndThing;"&&Step++==4);return true;}};
class CMemoryDataInputStream{public:CCharString Name;CMemoryDataInputStream():Name("Object"){}CCharString ReadNullTerminatedString(){return Name;}};
class CPersistContext{public:bool Binary;CStringParser Parser;CMemoryDataInputStream Stream;CPersistContext():Binary(false){}bool IsBinary()const{return Binary;}CStringParser*GetPParser(){return &Parser;}CMemoryDataInputStream*GetPLoadStream(){return &Stream;}};
class CClass{public:virtual~CClass(){}};
static CCharString InputDefinition;static unsigned __int64 InputUID;static unsigned SerialiseBinary,SerialiseText;
class CThing:public CClass{public:
 class CSerialiseInfo{public:unsigned __int64 UID;CCharString DefinitionType;signed char Player;};
 bool KillOnUnload,Persistent,Limbo,CreateSucceeds;unsigned ConstructBinary,ConstructText,Initials,OnInitials,Creates,Kills,Sections,LimboClears;
 CThing():KillOnUnload(false),Persistent(false),Limbo(false),CreateSucceeds(true),ConstructBinary(0),ConstructText(0),Initials(0),OnInitials(0),Creates(0),Kills(0),Sections(0),LimboClears(0){}
 static void __fastcall SerialiseThingInfo(CPersistContext&context,CSerialiseInfo&out){assert(context.Binary);++SerialiseBinary;out.UID=InputUID;out.DefinitionType=InputDefinition;out.Player=0;}
 static void __fastcall SerialiseThingInfo(bool saving,CCharString&text,CSerialiseInfo&out){assert(!saving&&text.Text=="script");++SerialiseText;out.UID=InputUID;out.DefinitionType=InputDefinition;out.Player=0;}
 bool IsToKillOnLevelUnload()const{return KillOnUnload;}bool IsLevelPersistent()const{return Persistent;}bool IsInLimbo()const{return Limbo;}void SetInLimbo(bool b){assert(!b);Limbo=false;++LimboClears;Events.push_back("ActivateLimbo");}
 void ConstructFromBinarySave(CMemoryDataInputStream&,const CSerialiseInfo&i){assert(i.UID==InputUID);++ConstructBinary;Events.push_back("Construct");}void ConstructFromLevelScript(const char*s){assert(std::string(s)=="script");++ConstructText;Events.push_back("Construct");}
 void InitialActivate(){++Initials;Events.push_back("InitialActivate");}void OnInitialActivate(){++OnInitials;Events.push_back("OnInitialActivate");}bool OnCreate(){++Creates;Events.push_back("OnCreate");return CreateSucceeds;}void SetSerialisationSectionName(const CCharString&n){assert(n.Text=="SECTION");++Sections;}void Kill(bool b){assert(b);++Kills;Events.push_back("Kill");}
};
class CThingPlayerCreature:public CThing{};
class CPlayer{public:CThingPlayerCreature*Hero;CPlayer():Hero(0){}CThingPlayerCreature*GetPControlledCreature()const{return Hero;}};
class CPlayerManager{public:CPlayer Player;CPlayer&GetMainPlayer(){return Player;}};
class CMainGameComponent{public:static CMainGameComponent*Instance;CPlayerManager Players;static CMainGameComponent*Get(){return Instance;}CPlayerManager&GetPlayerManager(){return Players;}};CMainGameComponent*CMainGameComponent::Instance=0;
class CQuestManager{public:static CQuestManager Instance;unsigned Activations;CQuestManager():Activations(0){}static CQuestManager&Get(){return Instance;}void OnActivateEntity(CThing&t){assert(!t.Limbo);++Activations;}};CQuestManager CQuestManager::Instance;
class CWorld{public:bool HeroState;CWorld():HeroState(false){}bool IsSerialisingHeroState()const{return HeroState;}};
namespace NThingManager{class CEntityRuntimePersistence{public:bool Dead;mutable unsigned Checks;mutable unsigned __int64 CheckedUID;CEntityRuntimePersistence():Dead(false),Checks(0),CheckedUID(0){}bool IsEntityPersistedAsDead(unsigned __int64 uid)const{++Checks;CheckedUID=uid;Events.push_back("DeadCheck");return Dead;}};}
namespace NProgressDisplay{void __fastcall DisplayProgress(const CCharString&text,float value,bool a,bool b){assert(value==-1.0f&&!a&&!b);Events.push_back(text.Text);}}
class CThingManager{public:
 CGameDefinitionManager DefinitionManager;CWorld World;NThingManager::CEntityRuntimePersistence EntityRuntimePersistence;bool ErrorFlag;long LevelBeingLoaded;CCharString CurrentSectionName;CThing*Existing;CThing*Allocated;unsigned Allocations,Fixes;mutable unsigned Remaps;mutable bool RemapLevelLess;mutable unsigned __int64 RemappedUID;
 CThingManager():ErrorFlag(false),LevelBeingLoaded(7),CurrentSectionName("SECTION"),Existing(0),Allocated(0),Allocations(0),Fixes(0),Remaps(0),RemapLevelLess(false),RemappedUID(0){}
 const CGameDefinitionManager&PeekDefinitionManager()const{return DefinitionManager;}
 long GetLevelBeingLoaded()const{return LevelBeingLoaded;}
 unsigned __int64 RemapUIDForSerialisation(bool saving,bool less,unsigned long level,unsigned __int64 uid)const{assert(!saving&&level==7&&uid==InputUID);++Remaps;RemapLevelLess=less;RemappedUID=uid+100;return RemappedUID;}
 CThing*GetPThingWithUID(unsigned __int64 uid){assert(uid==RemappedUID);return Existing;}
 void CheckAndFixUID(unsigned __int64&uid){assert(uid==RemappedUID);++Fixes;uid+=19;}
 CClass*AllocateClass(const CCharString&name){assert(name.Text=="Object"||name.Text=="PlayerCreature");++Allocations;Events.push_back("Allocate");return Allocated;}
 CThing*LoadSingleThing(CPersistContext&,long);
};

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


static unsigned Count(const char*s){unsigned n=0;for(unsigned i=0;i<Events.size();++i)if(Events[i]==s)++n;return n;}
int main(){unsigned cases=0;
 for(unsigned binary=0;binary<2;++binary)for(unsigned mode=0;mode<18;++mode)for(unsigned inclusion=0;inclusion<4;++inclusion){
 Events.clear();SerialiseBinary=SerialiseText=0;CQuestManager::Instance.Activations=0;CThingManager m;CThingBaseDef def;CThing oldThing,newThing;CThingPlayerCreature hero;CMainGameComponent mainGame;mainGame.Players.Player.Hero=&hero;CMainGameComponent::Instance=&mainGame;
 CPersistContext context;context.Binary=binary!=0;InputUID=0x12345678u;InputUID=(InputUID<<32)|0x87654321u;InputDefinition="OBJECT_DEF";m.DefinitionManager.Def=&def;m.Allocated=&newThing;
 bool expectLoad=true,expectHero=false,expectAllocate=true,expectConstruct=true,expectCreate=true,expectReleaseEarly=false;CThing*expected=&newThing;
 switch(mode){
 case 0:break;
 case 1:def.IsLoadableGlobal=true;break;
 case 2:InputDefinition="NULL";break;
 case 3:m.DefinitionManager.Found=false;expectLoad=false;break;
 case 4:m.EntityRuntimePersistence.Dead=true;expectAllocate=false;expectConstruct=false;expectReleaseEarly=true;expected=0;break;
 case 5:m.Allocated=0;expectConstruct=false;expected=0;break;
 case 6:newThing.CreateSucceeds=false;expectCreate=false;expected=0;break;
 case 7:m.Existing=&oldThing;expectAllocate=false;expectConstruct=false;expectReleaseEarly=true;expected=0;break;
 case 8:m.Existing=&oldThing;oldThing.Limbo=true;expectAllocate=false;expectConstruct=false;expectReleaseEarly=true;expected=0;break;
 case 9:m.Existing=&oldThing;oldThing.KillOnUnload=true;break;
 case 10:m.Existing=&oldThing;oldThing.KillOnUnload=true;oldThing.Persistent=true;expectAllocate=false;expectConstruct=false;expectReleaseEarly=true;expected=0;break;
 case 11:m.Existing=&oldThing;oldThing.KillOnUnload=true;m.Allocated=0;expected=&oldThing;break;
 case 12:m.Existing=&oldThing;oldThing.KillOnUnload=true;m.EntityRuntimePersistence.Dead=true;expectAllocate=false;expectConstruct=false;expectReleaseEarly=true;expected=0;break;
 case 13:m.World.HeroState=true;context.Stream.Name=context.Parser.Name="PlayerCreature";expectHero=true;expectAllocate=false;expected=&hero;break;
 case 14:m.World.HeroState=true;context.Stream.Name=context.Parser.Name="PlayerCreature";m.EntityRuntimePersistence.Dead=true;m.Existing=&oldThing;oldThing.Limbo=true;expectHero=true;expectAllocate=false;expected=&hero;break;
 case 15:m.World.HeroState=true;break;
 case 16:context.Stream.Name=context.Parser.Name="PlayerCreature";break;
 case 17:def.LevelLess=true;break;
 }
 if(mode==2)expectLoad=(inclusion==0||inclusion==2);
 else if(mode!=3)expectLoad=def.IsLoadableGlobal?(inclusion==0||inclusion==2):(inclusion==0||inclusion==1);
 if(!expectLoad){expectHero=false;expectAllocate=false;expectConstruct=false;expected=0;expectReleaseEarly=false;}
 CThing*result=m.LoadSingleThing(context,inclusion);assert(result==expected);assert(def.RefCount==1);assert(m.ErrorFlag==(mode==3));assert(m.Remaps==(expectLoad?1u:0u));if(expectLoad)assert(m.RemapLevelLess==def.LevelLess);assert(m.DefinitionManager.Calls==(mode==2?0u:1u));assert(m.Allocations==(expectAllocate?1u:0u));assert(SerialiseBinary==binary&&SerialiseText==1-binary);assert(context.Parser.Step==(binary?0u:5u));
 unsigned fixes=expectLoad&&(mode==9||mode==11||mode==12)?1u:0u;assert(m.Fixes==fixes);unsigned deadChecks=expectLoad&&!expectHero?1u:0u;assert(m.EntityRuntimePersistence.Checks==deadChecks);if(deadChecks)assert(m.EntityRuntimePersistence.CheckedUID==InputUID+100+19*fixes);
 assert(CQuestManager::Instance.Activations==(expectLoad&&mode==8?1u:0u));assert(oldThing.LimboClears==(expectLoad&&mode==8?1u:0u));
 CThing*constructed=expectHero?static_cast<CThing*>(&hero):(mode==11?&oldThing:&newThing);assert(oldThing.ConstructBinary+newThing.ConstructBinary+hero.ConstructBinary==(expectConstruct&&binary?1u:0u));assert(oldThing.ConstructText+newThing.ConstructText+hero.ConstructText==(expectConstruct&&!binary?1u:0u));if(expectConstruct){assert(constructed->Initials==(expectHero?1u:0u));assert(constructed->OnInitials==(expectHero?1u:0u));assert(constructed->Creates==(expectHero?0u:1u));assert(constructed->Sections==(!expectHero&&expectCreate?1u:0u));assert(constructed->Kills==(!expectHero&&!expectCreate?1u:0u));}
 assert(Count("Load Single Thing 1")==1&&Count("Load Single Thing 2")==1&&Count("Load Single Thing 3")==1);assert(Count("Load Single Thing: Allocate Class")==(expectAllocate?1u:0u));assert(Count("Load Single Thing: Construct Thing")==(expectConstruct?1u:0u));assert(Count("Load Single Thing: Initial Activate")==(expectConstruct?1u:0u));assert(Events[0]=="Load Single Thing 1");
 unsigned finish=0,release=0;for(unsigned i=0;i<Events.size();++i){if(Events[i]=="Load Single Thing 3")finish=i;if(Events[i]=="Release")release=i;}if(mode!=2&&mode!=3){assert(Count("Release")==1);assert(expectReleaseEarly?release<finish:release>finish);}else assert(Count("Release")==0);
 ++cases;
 }
 assert(cases==144);std::puts("0051fd80_TEST PASS");return 0;
}
