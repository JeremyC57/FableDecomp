#include <stdio.h>
#include <vector>
#include <string.h>
enum ECreatureGeneratorGenerateType { GENERATE_NORMAL, GENERATE_AMBUSH_DROP_IN, GENERATE_AMBUSH_JUMP_OUT, GENERATE_GENERATOR_ANIMATION, GENERATE_UNDEAD };
enum ETCInterfaceType { TCI_CREATURE_GENERATOR_CREATOR };
struct C3DVector { int X; C3DVector(int x=0):X(x){} };
struct Trace { int hive, undead, rng, defs, subs, nav, live, made, initLive, errors, kind[4], pos[4], slot[4]; } tr;
static int scenario, rarity;
static long g_GeneratedCreatureCount;
struct CTCCreatureHive { void Open(){++tr.hive;} } hive;
struct CTCCreatureNavigation {
    bool IsAbleToNavigateFromPosToPos(const C3DVector& start,const C3DVector& end,float tolerance)const {
        ++tr.nav; if(start.X!=90 || end.X<10 || tolerance!=0.0f) ++tr.errors;
        return !(scenario&512) || end.X%2==0;
    }
} navigation;
struct CCreatureDef { int CreatureGroup; } creatures[3];
struct CCreatureGenerationFamilyDef { std::vector<ECreatureGeneratorGenerateType> GenerationTypes; std::vector<long> Creatures; } family;
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer():Object(0){}
    ~CDefPointer(){if (Object)--tr.live;}
    void Set(T* item){Object=item;++tr.live;}
    T* operator->() const {return Object;}
private:
    CDefPointer(const CDefPointer&); CDefPointer& operator=(const CDefPointer&);
};
struct CThingCreatureDef { int index; bool GetSubDef(CDefPointer<const CCreatureDef>* out)const{++tr.subs;if((rarity&2)&&index==1)return false;out->Set(creatures+index);return true;} } defs[3];
struct CDefinitionManager {
    void GetDef(long id,CDefPointer<const CCreatureGenerationFamilyDef>* out)const{if(id!=9)++tr.errors;out->Set(&family);}
    bool GetDef(long id,CDefPointer<const CThingCreatureDef>* out)const{++tr.defs;if(id<0||id>2){++tr.errors;return false;}if((rarity&1)&&id==0)return false;out->Set(defs+id);return true;}
} definitions;
struct CThing {
    C3DVector pos;
    const C3DVector& GetPos()const{return pos;}
    long GetPlayerNumber()const{return 7;}
    bool HasTC(ETCInterfaceType)const{return (scenario&1)!=0;}
    bool GetTC(CTCCreatureHive** out)const{if(!(scenario&2))return false;*out=&hive;return true;}
    const CDefinitionManager& PeekDefinitionManager()const{return definitions;}
};
struct CThingCreatureBase: CThing {};
struct CThingPlayerCreature: CThingCreatureBase {
    bool PeekTC(const CTCCreatureNavigation** out)const{if(!(scenario&128))return false;*out=&navigation;return true;}
} hero;
struct CPlayer { CThingPlayerCreature* GetPPlayerCharacter()const{return scenario&64?&hero:0;} } player;
static bool IS_THING_ALIVE(const CThingPlayerCreature* h){return h && !(rarity&8);}
struct CWorld {
    unsigned long seed;
    unsigned long* GetWorldSeed(){return &seed;}
    CPlayer* PeekMyPlayer(){return &player;}
    bool IsCreatureGenerationEnabledForGroup(int group)const{return !(rarity&4) || group!=2;}
} world;
static unsigned long GFRandom(unsigned long bound,unsigned long* seed){if(!bound || seed!=&world.seed){++tr.errors;return 0;}++tr.rng;return (*seed)++%bound;}
struct CThingAICreatureInit {
    CThingAICreatureInit(int x,int y,bool z,const char* script){if(x||y||z||strcmp(script,"generator"))++tr.errors;++tr.initLive;}
    ~CThingAICreatureInit(){--tr.initLive;}
};
struct CCreatureActionBase { int kind; CCreatureActionBase(int k):kind(k){} };
struct CCreatureAction_Ambush:CCreatureActionBase { CCreatureAction_Ambush(CThingCreatureBase&,CThing& target,const C3DVector& home):CCreatureActionBase(2){if(target.pos.X!=31||home.X!=72)++tr.errors;} };
struct CCreatureAction_OnGenerate:CCreatureActionBase { CCreatureAction_OnGenerate(CThingCreatureBase&):CCreatureActionBase(1){} };
struct CThingAICreature: CThingCreatureBase {
    int id, flags; static CThingAICreature* Create(long,const C3DVector&,long,const CThingAICreatureInit&);
    void SetAsGameStatePersisted(bool b){if(b)++tr.errors;flags|=1;}
    void SetAsLevelPersistent(bool b){if(b)++tr.errors;flags|=2;}
    void SetCurrentAction(const CCreatureActionBase& action){tr.kind[id]=action.kind;}
} spawned[3];
CThingAICreature* CThingAICreature::Create(long which,const C3DVector& pos,long playerNumber,const CThingAICreatureInit&){
    if(which<0||which>2 || tr.made>=3 || playerNumber!=7 || !tr.initLive){++tr.errors;return spawned;}
    CThingAICreature& creature=spawned[which];creature.id=which;creature.flags=0;tr.slot[tr.made++]=which;tr.pos[which]=pos.X;return &creature;
}
struct CTCCreatureGenerator {
    CThing parent;std::vector<int> CreatureFamilies;std::vector<CThingAICreature*> CreatedCreatures;
    std::vector<C3DVector> OpenPositions,HiddenPositions;
    int CreationCount, limit;bool DoNavCheckOnGeneration;struct {const char* ScriptNameOfAllGeneratedCreatures;} GenParams;
    CThing& GetParentThing(){return parent;} CWorld& GetWorld(){return world;} CWorld& PeekWorld(){return world;}
    const CDefinitionManager& GetDefinitionManager(){return definitions;}
    bool HasHitCreationLimit(){return CreationCount>=limit;}
    void CreateCreaturesUndead(CThingCreatureBase* trigger){++tr.undead;if((trigger!=0)!=((scenario&256)!=0))++tr.errors;}
    void CreateCreatures(CThingCreatureBase*);
};
void CTCCreatureGenerator::CreateCreatures(CThingCreatureBase* pTriggerer)
{
    if (GetParentThing().HasTC(TCI_CREATURE_GENERATOR_CREATOR))
        return;
    CTCCreatureHive* ptc_hive;
    if (GetParentThing().GetTC(&ptc_hive))
        ptc_hive->Open();
    if (CreatureFamilies.empty() || HasHitCreationLimit())
        return;
    long chooser = GFRandom(CreatureFamilies.size(),PeekWorld().GetWorldSeed());
    CDefPointer<const CCreatureGenerationFamilyDef> def;
    GetDefinitionManager().GetDef(CreatureFamilies.at(chooser), &def);
    ECreatureGeneratorGenerateType gtype = def->GenerationTypes[GFRandom(def->GenerationTypes.size(),GetWorld().GetWorldSeed())];
    std::vector<C3DVector>* ppos_list_to_use = 0;
    bool jump_out = false;
    bool drop_in = false;
    switch (gtype) {
    case GENERATE_NORMAL: ppos_list_to_use=&OpenPositions;break;
    case GENERATE_AMBUSH_DROP_IN: ppos_list_to_use=&OpenPositions;drop_in=true;break;
    case GENERATE_AMBUSH_JUMP_OUT: ppos_list_to_use=&HiddenPositions;jump_out=true;break;
    case GENERATE_GENERATOR_ANIMATION: return;
    case GENERATE_UNDEAD: CreateCreaturesUndead(pTriggerer);return;
    default: return;
    }
    if (ppos_list_to_use->empty()) return;
    const CTCCreatureNavigation* pTCNavigate=0;
    C3DVector navPosStart=GetParentThing().GetPos();
    if (DoNavCheckOnGeneration && !jump_out) {
        CThingPlayerCreature* phero=GetWorld().PeekMyPlayer()->GetPPlayerCharacter();
        if (IS_THING_ALIVE(phero)) { phero->PeekTC(&pTCNavigate); navPosStart=phero->GetPos(); }
        if (!pTCNavigate) return;
    }
    for (std::vector<long>::const_iterator it=def->Creatures.begin();it!=def->Creatures.end();++it) {
        CDefPointer<const CThingCreatureDef> pdef;
        if (GetParentThing().PeekDefinitionManager().GetDef(*it,&pdef)) {
            CDefPointer<const CCreatureDef> psub_def;
            if (pdef->GetSubDef(&psub_def) && PeekWorld().IsCreatureGenerationEnabledForGroup(psub_def->CreatureGroup)) {
                const C3DVector& creature_pos=(*ppos_list_to_use)[GFRandom(ppos_list_to_use->size(),GetWorld().GetWorldSeed())];
                if (pTCNavigate && !pTCNavigate->IsAbleToNavigateFromPosToPos(navPosStart,creature_pos,0.0f)) continue;
                CThingAICreatureInit init(0,0,false,GenParams.ScriptNameOfAllGeneratedCreatures);
                CThingAICreature* pcreatedcreature=CThingAICreature::Create(*it,creature_pos,GetParentThing().GetPlayerNumber(),init);
                pcreatedcreature->SetAsGameStatePersisted(false);
                pcreatedcreature->SetAsLevelPersistent(false);
                if (jump_out) {
                    if (pTriggerer)
                        pcreatedcreature->SetCurrentAction(CCreatureAction_Ambush(*pcreatedcreature,*pTriggerer,GetParentThing().GetPos()));
                } else if (drop_in) {
                    pcreatedcreature->SetCurrentAction(CCreatureAction_OnGenerate(*pcreatedcreature));
                }
                CreatedCreatures.push_back(pcreatedcreature);
                ++g_GeneratedCreatureCount;
                ++CreationCount;
                if (HasHitCreationLimit()) break;
            }
        }
    }
}

static bool exercise(unsigned g,int bits,int filters)
{
    scenario=bits;rarity=filters;memset(&tr,0,sizeof(tr));world.seed=0;g_GeneratedCreatureCount=4;
    CTCCreatureGenerator generator;generator.CreationCount=0;generator.limit=(bits&8)?0:((bits&1024)?1:4);
    generator.DoNavCheckOnGeneration=(bits&32)!=0;
    generator.parent.pos=C3DVector(72);hero.pos=C3DVector(90);CThingCreatureBase trigger;trigger.pos=C3DVector(31);
    generator.GenParams.ScriptNameOfAllGeneratedCreatures="generator";
    if(!(bits&4))generator.CreatureFamilies.push_back(9);
    if(!(bits&16))for(int i=0;i<2;++i){generator.OpenPositions.push_back(C3DVector(10+i));generator.HiddenPositions.push_back(C3DVector(20+i));}
    family.GenerationTypes.clear();family.GenerationTypes.push_back(static_cast<ECreatureGeneratorGenerateType>(g));
    generator.CreateCreatures((bits&256)?&trigger:0);
    int made=0,expected[3]={0},positions[3]={0},navigationChecks=0,reads=0,subs=0,picks=0;
    bool active=!(bits&(1|4|8));bool checks=generator.DoNavCheckOnGeneration && g!=2;
    if(active && g<3 && !(bits&16) && (!checks || (bits&64 && bits&128 && !(filters&8)))){
        for(int i=0;i<3;++i){
            ++reads;if((filters&1)&&i==0)continue;
            ++subs;if(((filters&2)&&i==1)||((filters&4)&&i==2)) continue;
            int point=(g==2?20:10)+(picks++%2);
            if(checks){++navigationChecks;if((bits&512) && (point%2))continue;}
            expected[made]=i;positions[made]=point;++made;
            if(made==generator.limit)break;
        }
    }
    if(tr.errors||tr.live||tr.initLive||tr.hive!=int(!(bits&1)&&(bits&2))||tr.undead!=int(active&&g==4)||
       tr.made!=made||generator.CreationCount!=made||g_GeneratedCreatureCount!=4+made||generator.CreatedCreatures.size()!=unsigned(made)||
       tr.nav!=navigationChecks||tr.defs!=reads||tr.subs!=subs||tr.rng!=picks+(active?2:0))return false;
    for(int k=0;k<made;++k){int i=expected[k];if(tr.slot[k]!=i||tr.pos[i]!=positions[k]||tr.kind[i]!=(g==1?1:(g==2 && (bits&256)?2:0))||spawned[i].flags!=3||generator.CreatedCreatures[k]!=spawned+i)return false;}
    return true;
}
int main()
{
    for(int i=0;i<3;++i){family.Creatures.push_back(i);defs[i].index=i;creatures[i].CreatureGroup=i;}
    for(unsigned g=0;g<6;++g)for(int mask=0;mask<2048;++mask)for(int f=0;f<16;++f)if(!exercise(g,mask,f))return 1;
    puts("0074b2b0_TEST PASS");return 0;
}