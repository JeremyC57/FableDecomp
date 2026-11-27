#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static void ck(bool v){if(!v)exit(2);}
struct C3DVector{int x;};
typedef const char* CCharString;
struct CDefString{const char* s;bool operator!=(const char* t)const{return strcmp(s,t)!=0;}const char* ToWideString()const{return s;}};
enum{TCI_EXPRESSION=143,TCI_EMOTEICON=69,OPINION_DEED_TYPE_NONE=0,OPINION_DEED_TYPE_EXPRESSION_GIGGLE=24,OPINION_DEED_TYPE_EXPRESSION_SNEER=15,OPINION_POST_DEED_TYPE_RECIPIENT=1,OPINION_POST_DEED_TYPE_WITNESSES=2};
static struct{int expression,emote,update,added,sound,quest,post,icon,text,read;} logv;
namespace NGlobalConsole{bool DebugExpressionsUseSpriteOverrideText;}
struct Def{CDefString SpriteOverrideText;int OpinionDeedType;unsigned long TextTag;const char* GetInstantiationName(){return "definition";}} def;
struct CTCExpression{void SetPDef(Def* d){ck(d==&def);++logv.expression;}};
struct CTCEmoteIcon{
 enum {EMOTE_PRIORITY_EXPRESSION=5,EMOTE_PRIORITY_DEBUG_LOW=7};
 static float seconds;void SetEmoteText(int p,const char* s,float time,float scale){ck(p==7&&!strcmp(s,"override")&&time==-1&&scale==0.3f);++logv.text;}
 void SetEmoteIcon(int p,int sprite,float sec,float scale,unsigned char alpha){ck(p==5&&sprite==9&&scale==0.5f&&alpha==255);seconds=sec;++logv.icon;}
};float CTCEmoteIcon::seconds=0;
struct CTCSoundPlayer{void PlayAttachedDialogue(unsigned long key,bool a,bool b,bool c,bool d){ck(key==27&&!a&&!b&&!c&&!d);logv.sound=1;}void PlayAttachedCriteria(const char* s,bool a,bool b,bool c,long k){ck(!a&&!b&&!c&&k==-1);if(!strcmp(s,"EXPRESSION_SNEER"))logv.sound=2;else{ck(!strcmp(s,"EXPRESSION_USE"));logv.sound=3;}}};
struct CTCHero{};
struct CTCTargetingBase;struct CTCHeroOpinionDeedLog;
struct CThing{
 int type;C3DVector pos;CTCExpression* ex;CTCEmoteIcon* em;CTCSoundPlayer* snd;CTCHero* h;CTCTargetingBase* tg;CTCHeroOpinionDeedLog* dl;bool child;
 bool HasTC(int k){return k==143?ex!=0:em!=0;}
 void AddTC(const char* s,bool b,void* v){ck(!b&&!v);static CTCExpression t;static CTCEmoteIcon e;if(!strcmp(s,"CTCExpression"))ex=&t;else{ck(!strcmp(s,"CTCEmoteIcon"));em=&e;}++logv.added;}
 bool GetTC(CTCExpression** p){*p=ex;return ex!=0;}bool GetTC(CTCEmoteIcon** p){*p=em;return em!=0;}bool GetTC(CTCTargetingBase** p){*p=tg;return tg!=0;}bool GetTC(CTCHeroOpinionDeedLog** p){*p=dl;return dl!=0;}bool GetTC(CTCSoundPlayer** p){*p=snd;return snd!=0;}bool PeekTC(const CTCHero** p){*p=h;return h!=0;}
 bool IsChild(){return child;}unsigned long GetType(){return type;}const C3DVector& PeekPos(){return pos;}
} creature;
struct CThingAICreature:CThing{} target;
template<class F,class T>T GFDebugPDynamicCast(F f,T){return static_cast<T>(f);}
struct CTCTargetingBase{CThing* p;CThing* GetPTargetedThing(){return p;}} targeting;
struct Effects{bool ReactionRestrictToTarget;};
struct CTCHeroOpinionDeedLog{Effects fx;Effects* PeekPDeedEffectsDef(int t){ck(t==def.OpinionDeedType);++logv.read;return &fx;}
 void PostDeed(int t,const C3DVector& pos,const CThing* thing,CThingAICreature* ai,int post,bool a,bool b,void* c,bool d,long e){ck(t==def.OpinionDeedType&&&pos==&creature.pos&&!a&&!b&&!c&&!d&&!e);if(post==1)ck(thing==&target&&ai==&target);else ck(post==2&&!thing&&!ai);logv.post=post;}
} deed;
struct CQuestManager{static CQuestManager& Get(){static CQuestManager m;return m;}void SetExpressionPerformed(const char* s){ck(!strcmp(s,"definition"));++logv.quest;}};
struct Manager{long ExpressionIndicatorIcon;const Manager& PeekGlobalDef()const{return *this;}} manager;
struct CCreatureActionBase{void Initialise(){++logv.update;}};
struct CCreatureAction_PerformExpression:CCreatureActionBase{CThing* PCreature;Def* PExpressionDef;bool EmoteIconAdded,Extended;long TotalTime;float GetFrameUpdateTime()const{return 0.125f;}const Manager& PeekDefinitionManager()const{return manager;}void Initialise();};
void CCreatureAction_PerformExpression::Initialise()
{
    CCreatureActionBase::Initialise();
    if (!PCreature->HasTC(TCI_EXPRESSION)) {
        PCreature->AddTC("CTCExpression",false,0);
        CTCExpression* ptc_expression;
        PCreature->GetTC(&ptc_expression);
        ptc_expression->SetPDef(PExpressionDef);
    }
    if (!PCreature->HasTC(TCI_EMOTEICON))
        PCreature->AddTC("CTCEmoteIcon",false,0);
    CTCEmoteIcon* ptc_emote_icon;
    PCreature->GetTC(&ptc_emote_icon);
    if (NGlobalConsole::DebugExpressionsUseSpriteOverrideText && PExpressionDef->SpriteOverrideText!="") {
        ptc_emote_icon->SetEmoteText(CTCEmoteIcon::EMOTE_PRIORITY_DEBUG_LOW,PExpressionDef->SpriteOverrideText.ToWideString(),-1.0f,0.3f);
        EmoteIconAdded=true;
    } else if (PeekDefinitionManager().PeekGlobalDef().ExpressionIndicatorIcon>0) {
        float total_time=TotalTime * GetFrameUpdateTime();
        if (Extended) total_time=-1.0f;
        ptc_emote_icon->SetEmoteIcon(CTCEmoteIcon::EMOTE_PRIORITY_EXPRESSION,PeekDefinitionManager().PeekGlobalDef().ExpressionIndicatorIcon,total_time,0.5f,255);
        EmoteIconAdded=true;
    }
    CTCSoundPlayer* ptc_sound;
    if (PCreature->GetTC(&ptc_sound)) {
        if (PExpressionDef->TextTag) {
            if (PExpressionDef->OpinionDeedType!=OPINION_DEED_TYPE_EXPRESSION_GIGGLE || !PCreature->IsChild())
                ptc_sound->PlayAttachedDialogue(PExpressionDef->TextTag,false,false,false,false);
        } else if (PExpressionDef->OpinionDeedType==OPINION_DEED_TYPE_EXPRESSION_SNEER)
            ptc_sound->PlayAttachedCriteria("EXPRESSION_SNEER",false,false,false,-1);
        else ptc_sound->PlayAttachedCriteria("EXPRESSION_USE",false,false,false,-1);
    }
    const CTCHero* ptc_hero;
    if (PCreature->PeekTC(&ptc_hero)) {
        CQuestManager::Get().SetExpressionPerformed(PExpressionDef->GetInstantiationName());
        CTCHeroOpinionDeedLog* ptc_deed_log;
        if (PCreature->GetTC(&ptc_deed_log)) {
            if (PExpressionDef->OpinionDeedType!=OPINION_DEED_TYPE_NONE && ptc_deed_log->PeekPDeedEffectsDef(PExpressionDef->OpinionDeedType)->ReactionRestrictToTarget) {
                CTCTargetingBase* ptc_targeting;
                PCreature->GetTC(&ptc_targeting);
                CThing* pthing=ptc_targeting->GetPTargetedThing();
                if (pthing && pthing->GetType()==1)
                    ptc_deed_log->PostDeed(PExpressionDef->OpinionDeedType,PCreature->PeekPos(),pthing,GFDebugPDynamicCast(pthing,(CThingAICreature*)0),OPINION_POST_DEED_TYPE_RECIPIENT,false,false,0,false,0);
            } else ptc_deed_log->PostDeed(PExpressionDef->OpinionDeedType,PCreature->PeekPos(),0,0,OPINION_POST_DEED_TYPE_WITNESSES,false,false,0,false,0);
        }
    }
}
int main(){
 CTCExpression ex;CTCEmoteIcon em;CTCSoundPlayer snd;CTCHero hero;
 for(int k=0;k<32768;++k){
  int f=k;bool haveEx=f&1,haveEm=f&2,hasSound=f&4,isHero=f&8,hasLog=f&16,restrictTo=f&32,positive=f&64,debug=f&128,over=f&256,extended=f&512,child=f&1024,hasTag=f&2048;
  int targetState=(f>>12)&3,kind=(f>>14)&1;
  for(int dt=0;dt<2;++dt){
   def.OpinionDeedType=dt?(kind?24:15):(kind?0:8);def.SpriteOverrideText.s=over?"override":"";def.TextTag=hasTag?27:0;
   creature.ex=haveEx?&ex:0;creature.em=haveEm?&em:0;creature.snd=hasSound?&snd:0;creature.h=isHero?&hero:0;creature.dl=hasLog?&deed:0;creature.tg=&targeting;creature.child=child;
   target.type=targetState==1?1:3;targeting.p=targetState?&target:0;deed.fx.ReactionRestrictToTarget=restrictTo;manager.ExpressionIndicatorIcon=positive?9:0;NGlobalConsole::DebugExpressionsUseSpriteOverrideText=debug;
   CCreatureAction_PerformExpression action;action.PCreature=&creature;action.PExpressionDef=&def;action.EmoteIconAdded=false;action.Extended=extended;action.TotalTime=40;
   memset(&logv,0,sizeof(logv));action.Initialise();
   bool text=debug&&over,icon=!text&&positive,read=isHero&&hasLog&&def.OpinionDeedType!=0;
   ck(logv.update==1&&logv.added==!haveEx+!haveEm&&logv.expression==!haveEx);
   ck(logv.text==text&&logv.icon==icon&&action.EmoteIconAdded==(text||icon));if(icon)ck(CTCEmoteIcon::seconds==(extended?-1.f:5.f));
   ck(logv.sound==(!hasSound?0:hasTag?(def.OpinionDeedType==24&&child?0:1):def.OpinionDeedType==15?2:3));
   ck(logv.quest==isHero&&logv.read==read&&logv.post==(!isHero||!hasLog?0:read&&restrictTo?(targetState==1?1:0):2));
  }
 }puts("00846710_TEST PASS");return 0;
}