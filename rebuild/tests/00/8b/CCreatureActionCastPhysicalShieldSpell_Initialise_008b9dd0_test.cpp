#include <stdio.h>
#include <string.h>
static char trace[64];static int at,failures,live;
static void step(char c){trace[at++]=c;trace[at]=0;}
enum ETCInterfaceType { TCI_PHYSICAL_SHIELD_SPELL=147 };
class CTCBase {public:virtual ~CTCBase(){}};
class CTCPhysicalShieldSpell:public CTCBase {public:long SpellLevel,DurationTimer;float ShieldHealth;
 void SetAICasting(long level){step('L');SpellLevel=level;}
 void SetTimeLimit(long time){step('S');DurationTimer=time;}
 void SetShieldHealth(float health){step('V');ShieldHealth=health;}
};
static CTCPhysicalShieldSpell shield;
class CCharString {public:const char* text;CCharString(const char* p,long n=-1):text(p){step('C');++live;if(n!=-1)++failures;}~CCharString(){step('D');--live;}};
class CThingCreatureBase {public:bool has;unsigned long type;
 bool HasTC(ETCInterfaceType t)const{step('H');if(t!=147)++failures;return has;}
 CTCBase* AddTC(const CCharString& name,bool activate,const void* params){step('A');if(strcmp(name.text,"CTCPhysicalShieldSpell")||!activate||params||live!=1)++failures;return &shield;}
 unsigned long GetType()const{step('T');if(live)++failures;return type;}
};
template<class T>class CIntelligentPointer {public:T* item;T* operator->()const{step('G');return item;}};
template<class From,class To>inline To GFDebugPDynamicCast(From value,To){return static_cast<To>(value);}
class CCombatActionBase {public:CIntelligentPointer<CThingCreatureBase> PCreature;void Initialise(){step('B');}};
class CCreatureAction_CastPhysicalShieldSpell:public CCombatActionBase {public:long LevelToCast,TimeLimit;float ShieldHealth;void Initialise();};
void CCreatureAction_CastPhysicalShieldSpell::Initialise()
{
    CCombatActionBase::Initialise();
    if(!PCreature->HasTC(TCI_PHYSICAL_SHIELD_SPELL)) {
        CTCPhysicalShieldSpell* ptc_shield=GFDebugPDynamicCast(PCreature->AddTC("CTCPhysicalShieldSpell",true,0),(CTCPhysicalShieldSpell*)0);
        if(PCreature->GetType()==1) {
            ptc_shield->SetAICasting(LevelToCast);
            ptc_shield->SetTimeLimit(TimeLimit);
        }
        ptc_shield->SetShieldHealth(ShieldHealth);
    }
}
int main(){
 const long levels[]={-1,0,1,100};const long times[]={-10,0,1,10000};const float health[]={-1,0,0.25f,1000};
 for(int h=0;h<2;++h)for(int t=0;t<3;++t)for(int l=0;l<4;++l)for(int d=0;d<4;++d)for(int v=0;v<4;++v){
  CThingCreatureBase creature;creature.has=h!=0;creature.type=t;CCreatureAction_CastPhysicalShieldSpell row;row.PCreature.item=&creature;row.LevelToCast=levels[l];row.TimeLimit=times[d];row.ShieldHealth=health[v];
  shield.SpellLevel=777;shield.DurationTimer=888;shield.ShieldHealth=999;at=0;trace[0]=0;row.Initialise();
  const char* wanted=h?"BGH":(t==1?"BGHCGADGTLSV":"BGHCGADGTV");
  const char* alternate=h?"BGH":(t==1?"BGHGCADGTLSV":"BGHGCADGTV");
  if((strcmp(trace,wanted)&&strcmp(trace,alternate))||live||failures||shield.SpellLevel!=(!h&&t==1?levels[l]:777)||shield.DurationTimer!=(!h&&t==1?times[d]:888)||shield.ShieldHealth!=(h?999:health[v]))return 1;
 }
 puts("008b9dd0_TEST PASS");return 0;
}