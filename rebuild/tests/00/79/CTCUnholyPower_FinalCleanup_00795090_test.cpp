// Behaviour test for 0x00795090 CTCUnholyPower::FinalCleanup(): the same exhaustive model as the landed twin 0x0078FAF0 CTCDivineWrath::FinalCleanup test (identical logic).
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
struct Event { int kind,id,value; Event(int k,int i,int v):kind(k),id(i),value(v){} bool operator==(const Event&o)const{return kind==o.kind&&id==o.id&&value==o.value;} };
static std::vector<Event> trace;
static int mutation;
static void check(bool x){if(!x){std::puts("FAIL");std::exit(1);}}
class CEnginePrimitiveThemeChannelDesc { public: unsigned long value; CEnginePrimitiveThemeChannelDesc(unsigned long v=0):value(v){} };
namespace NGraphicAppearance { class CTCGraphicAppearance { public:int id;unsigned long channel;void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc&c,bool a,bool b){check(a&&b);channel=c.value;trace.push_back(Event(3,id,int(c.value)));}}; }
class CTCVolumeContainmentTracker { public:int id;bool update;void SetUpdateLightingChannels(bool b){update=b;trace.push_back(Event(4,id,b));}};
class CThing { public: int id;bool dead;int kills,fades,refs; CThing():id(0),dead(false),kills(0),fades(0),refs(0){} void Kill(bool b){check(b);++kills;dead=true;trace.push_back(Event(1,id,0));} };
template<class T>class CIntelligentPointer { public: T*p; CIntelligentPointer(T*q=0):p(q){if(p)++p->refs;} CIntelligentPointer(const CIntelligentPointer&o):p(o.p){if(p)++p->refs;} ~CIntelligentPointer(){if(p)--p->refs;} T*operator->()const{return p;}operator T*()const{return p;}void clear(){if(p)--p->refs;p=0;} };
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>&p){return p.p&&!p.p->dead;}
class CThingCreatureBase:public CThing { public:NGraphicAppearance::CTCGraphicAppearance*graphic;CTCVolumeContainmentTracker*volume;bool epic;int resets; CThingCreatureBase():graphic(0),volume(0),epic(true),resets(0){} NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return graphic;}bool GetTC(CTCVolumeContainmentTracker**o){*o=volume;return volume!=0;}void SetUnderInfluenceOfEpicSpell(bool b){epic=b;trace.push_back(Event(5,id,b));}};
class CTCSpecialAbilities { public:static void FadeOrKillEffect(CIntelligentPointer<CThing>&p){trace.push_back(Event(2,p.p?p.p->id:-1,0));if(p.p){++p.p->fades;if(mutation&1)p.p->dead=true;if(mutation&2)p.clear();}}};
class CTCUnholyPower { public:struct PerEffectInfo {CIntelligentPointer<CThing> Effect;PerEffectInfo(CThing*p=0):Effect(p){}};struct PerCreatureInfo {CIntelligentPointer<CThingCreatureBase> ActualCreature;CIntelligentPointer<CThing>Effect;CEnginePrimitiveThemeChannelDesc DefaultChannel;PerCreatureInfo(CThingCreatureBase*a=0,CThing*e=0,int c=0):ActualCreature(a),Effect(e),DefaultChannel(c){}};bool CutSceneCancel;std::vector<PerEffectInfo>HitEffects;std::map<CThingCreatureBase*,PerCreatureInfo>LastFrameCreaturesHit;CThingCreatureBase*parent;CEnginePrimitiveThemeChannelDesc HeroDefaultChannel;CThingCreatureBase&GetParentThing(){return *parent;}void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase>p){trace.push_back(Event(6,p.p?p.p->id:-1,p.p?p.p->refs:0));if(p.p){++p.p->resets;if(mutation&4)p.p->dead=true;}}void FinalCleanup();};
void CTCUnholyPower::FinalCleanup()
{
 {
  for(std::vector<PerEffectInfo>::iterator it=HitEffects.begin(),endit=HitEffects.end();it!=endit;++it) {
   PerEffectInfo& info=*it;
   if(CutSceneCancel) {
    if(IS_THING_ALIVE(info.Effect))info.Effect->Kill(true);
   } else CTCSpecialAbilities::FadeOrKillEffect(info.Effect);
  }
 }
 std::map<CThingCreatureBase*,PerCreatureInfo>::iterator it,endit;
 for(it=LastFrameCreaturesHit.begin(),endit=LastFrameCreaturesHit.end();it!=endit;++it) {
  if(CutSceneCancel) {
   if(IS_THING_ALIVE(it->second.Effect))it->second.Effect->Kill(true);
  } else CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
  CIntelligentPointer<CThingCreatureBase> creature=it->second.ActualCreature;
  if(IS_THING_ALIVE(creature)) {
   NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=creature->GetTCGraphicAppearance();
   if(ptc_graphic_appearance)ptc_graphic_appearance->SetLightingChannel(it->second.DefaultChannel,true,true);
   CTCVolumeContainmentTracker* ptc_volume_contain=0;
   if(creature->GetTC(&ptc_volume_contain))ptc_volume_contain->SetUpdateLightingChannels(true);
   creature->SetUnderInfluenceOfEpicSpell(false);
  }
  ResetCreatureHitState(creature);
 }
 NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=GetParentThing().GetTCGraphicAppearance();
 if(ptc_graphic_appearance)ptc_graphic_appearance->SetLightingChannel(HeroDefaultChannel,true,true);
 CTCVolumeContainmentTracker* ptc_volume_contain=0;
 if(GetParentThing().GetTC(&ptc_volume_contain))ptc_volume_contain->SetUpdateLightingChannels(true);
}

struct Fixture {CThing effects[3];CThingCreatureBase creatures[3];NGraphicAppearance::CTCGraphicAppearance graphics[3];CTCVolumeContainmentTracker volumes[3];CTCUnholyPower w;
 Fixture(int cancel,int count,int profile,int components,int alias){w.CutSceneCancel=cancel!=0;w.parent=&creatures[2];w.HeroDefaultChannel.value=1024;
 for(int i=0;i<3;++i){effects[i].id=10+i;effects[i].dead=((profile>>(i*2))&3)==1;creatures[i].id=20+i;creatures[i].dead=((profile>>(i*2))&3)==2;graphics[i].id=creatures[i].id;graphics[i].channel=0;volumes[i].id=creatures[i].id;volumes[i].update=false;creatures[i].graphic=(components&(1<<i))?&graphics[i]:0;creatures[i].volume=(components&(8<<i))?&volumes[i]:0;}
 for(int i=0;i<count;++i){int q=(profile>>(i*2))&3;CThing*e=q==3?0:&effects[alias?0:i];if(alias==2)e=&creatures[alias==2?2:i];w.HitEffects.push_back(CTCUnholyPower::PerEffectInfo(e));CThingCreatureBase*c=q==0?0:&creatures[alias?2:i];w.LastFrameCreaturesHit.insert(std::make_pair(&creatures[i],CTCUnholyPower::PerCreatureInfo(c,e,16+i)));}}
};
static void oracle(CTCUnholyPower&w){
 for(unsigned i=0;i<w.HitEffects.size();++i){CIntelligentPointer<CThing>&p=w.HitEffects[i].Effect;if(w.CutSceneCancel){if(p.p&&!p.p->dead)p.p->Kill(true);}else CTCSpecialAbilities::FadeOrKillEffect(p);}
 for(std::map<CThingCreatureBase*,CTCUnholyPower::PerCreatureInfo>::iterator k=w.LastFrameCreaturesHit.begin();k!=w.LastFrameCreaturesHit.end();++k){CTCUnholyPower::PerCreatureInfo&r=k->second;if(w.CutSceneCancel){if(r.Effect.p&&!r.Effect.p->dead)r.Effect.p->Kill(true);}else CTCSpecialAbilities::FadeOrKillEffect(r.Effect);
 CIntelligentPointer<CThingCreatureBase> keep(r.ActualCreature);CThingCreatureBase*c=keep.p;if(c&&!c->dead){if(c->graphic)c->graphic->SetLightingChannel(r.DefaultChannel,true,true);if(c->volume)c->volume->SetUpdateLightingChannels(true);c->SetUnderInfluenceOfEpicSpell(false);}w.ResetCreatureHitState(keep);}
 CThingCreatureBase&h=*w.parent;if(h.graphic)h.graphic->SetLightingChannel(w.HeroDefaultChannel,true,true);if(h.volume)h.volume->SetUpdateLightingChannels(true);
}
int main(){unsigned long cases=0;
 for(int cancel=0;cancel<2;++cancel)for(int count=0;count<4;++count)for(int profile=0;profile<64;++profile)for(int comp=0;comp<64;++comp)for(int alias=0;alias<3;++alias)for(mutation=0;mutation<8;++mutation){
 Fixture a(cancel,count,profile,comp,alias),b(cancel,count,profile,comp,alias);trace.clear();a.w.FinalCleanup();std::vector<Event> actual=trace;trace.clear();oracle(b.w);check(actual==trace);
 for(int i=0;i<3;++i){check(a.effects[i].dead==b.effects[i].dead&&a.effects[i].kills==b.effects[i].kills&&a.effects[i].fades==b.effects[i].fades&&a.effects[i].refs==b.effects[i].refs);check(a.creatures[i].dead==b.creatures[i].dead&&a.creatures[i].epic==b.creatures[i].epic&&a.creatures[i].resets==b.creatures[i].resets&&a.creatures[i].refs==b.creatures[i].refs);check(a.graphics[i].channel==b.graphics[i].channel&&a.volumes[i].update==b.volumes[i].update);}check(a.w.HitEffects.size()==unsigned(count)&&a.w.LastFrameCreaturesHit.size()==unsigned(count));++cases;
 }
 check(cases==786432);std::puts("00795090_TEST PASS");return 0;
}