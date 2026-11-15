// Production body is copied verbatim below; external engine calls have observable semantic fixtures.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <vector>
static bool bad;
static int stringLive;
class CCharString {public:std::string s;CCharString(){++stringLive;}CCharString(const CCharString&o):s(o.s){++stringLive;}explicit CCharString(const char*x):s(x){++stringLive;}~CCharString(){--stringLive;}CCharString&operator=(const char*x){s=x;return*this;}CCharString&operator=(const CCharString&o){s=o.s;return*this;}};
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector&o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}C3DVector&operator-=(const C3DVector&o){X-=o.X;Y-=o.Y;Z-=o.Z;return*this;}float GetMagnitude()const{return(float)sqrt(X*X+Y*Y+Z*Z);}};
class CRightHandedSet {};
class CThing;
template<class T>class CDefPointer{T*p;public:CDefPointer():p(0){}T*operator->()const{return p;}void Set(T*x){p=x;}};
class CSpecialEffectsDef{public:mutable std::vector<std::string> queries;int fail;long shortID,longID;CSpecialEffectsDef():fail(0),shortID(0),longID(0){}bool GetEffectFromString(const CCharString&s,long&out)const{queries.push_back(s.s);int n=(int)queries.size();if(fail==n)return false;out=n==1?shortID:longID;return true;}};
class CSpecialAbilitiesMultiStrikeDef{public:std::vector<CCharString>AttachmentPoint;bool found;CSpecialEffectsDef*effects;int lookups;CSpecialAbilitiesMultiStrikeDef():found(true),effects(0),lookups(0){}bool GetSubDef(CDefPointer<const CSpecialEffectsDef>*out)const{CSpecialAbilitiesMultiStrikeDef*self=const_cast<CSpecialAbilitiesMultiStrikeDef*>(this);++self->lookups;if(found)out->Set(effects);return found;}};
namespace NParticleEngine{class CParticleEmitter{public:float length,ratio;int reads,sleeps,interpolates,updates;const CParticleEmitter*other;bool always;CParticleEmitter():length(0),ratio(0),reads(0),sleeps(0),interpolates(0),updates(0),other(0),always(false){}void Sleep(){++sleeps;}float GetApproximateTotalSplineLength(){++reads;return length;}void InterpolateTo(const CParticleEmitter&o,float t){++interpolates;other=&o;ratio=t;}void SetAlwaysUpdate(bool x){++updates;always=x;}};}
class CTCGraphicAppearance {public:int pattern,mode,helpers,dummies;std::vector<int>helperPoints,dummyPoints;CTCGraphicAppearance():pattern(0),mode(0),helpers(0),dummies(0){}bool GetHelperPoint(const CCharString&,long,unsigned long,C3DVector&,float);bool GetDummyObject(const CCharString&,long,unsigned long,C3DVector&,CRightHandedSet&,float);};
class CTCDParticleEmitter{public:NParticleEngine::CParticleEmitter emitter;int gets,attachments;CThing*attached;std::string point;CTCDParticleEmitter():gets(0),attachments(0),attached(0){}NParticleEngine::CParticleEmitter*GetPEmitter(){++gets;return&emitter;}void AttachToThing(CThing&w,long type,const CCharString&p,long index,float off){if(type!=10||index!=0||off!=0)bad=true;++attachments;attached=&w;point=p.s;}static CThing*Create(long,const C3DVector&,bool);};
class CThing {public:bool dead;int refs,posCalls,graphicsCalls,getTCs,kills;C3DVector pos;CTCGraphicAppearance*graphics;CTCDParticleEmitter*driver;CThing():dead(false),refs(0),posCalls(0),graphicsCalls(0),getTCs(0),kills(0),pos(13,17,19),graphics(0),driver(0){}bool IsDead()const{return dead;}CTCGraphicAppearance*GetTCGraphicAppearance(){++graphicsCalls;return graphics;}const C3DVector&GetPos()const{++const_cast<CThing*>(this)->posCalls;return pos;}bool GetTC(CTCDParticleEmitter**out){++getTCs;*out=driver;return driver!=0;}void Kill(bool x){if(!x)bad=true;++kills;dead=true;}};
template<class T>bool IS_THING_ALIVE(const T&t){return t!=0&&!t->IsDead();}
template<class T>class CIntelligentPointer{T*p;public:CIntelligentPointer(T*x):p(x){if(p)++p->refs;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p){if(p)++p->refs;}~CIntelligentPointer(){if(p)--p->refs;}CIntelligentPointer&operator=(const CIntelligentPointer&o){if(p)--p->refs;p=o.p;if(p)++p->refs;return*this;}T*Get()const{return p;}};
class CTCMultiStrike {public:CDefPointer<const CSpecialAbilitiesMultiStrikeDef>PDef;long SpellLevel;std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > >ParticleEffects;void ApplyEffectOnWeapon(CThing*w);};
static CThing pool[32];static CTCDParticleEmitter drivers[32];static int created,shortState,longState,blendMode;static long shortID,longID;static CThing*weapon;
static int Point(const CCharString&s){if(s.s.size()<2||s.s[0]!='P'){bad=true;return 0;}return atoi(s.s.c_str()+1);}
static C3DVector Position(int n,int mode){float distance=(mode==3||mode==5)?0.0f:(mode==1?1.0f:(mode==4?2.0f:3.0f));return C3DVector(n%2?distance:0.0f,0,0);}
bool CTCGraphicAppearance::GetHelperPoint(const CCharString&s,long index,unsigned long flags,C3DVector&out,float scale){if(index||flags||scale!=1)bad=true;int n=Point(s);++helpers;helperPoints.push_back(n);out=Position(n,mode);return(pattern&(n%2?4:1))!=0;}
bool CTCGraphicAppearance::GetDummyObject(const CCharString&s,long index,unsigned long flags,C3DVector&out,CRightHandedSet&,float scale){if(index||flags||scale!=1)bad=true;int n=Point(s);++dummies;dummyPoints.push_back(n);out=Position(n,mode);return(pattern&(n%2?8:2))!=0;}
CThing*CTCDParticleEmitter::Create(long id,const C3DVector&pos,bool force){int n=created++;if(n>=32){bad=true;return 0;}bool isLong=(n%2)!=0;if(id!=(isLong?longID:shortID)||!force||pos.X!=weapon->pos.X||pos.Y!=weapon->pos.Y||pos.Z!=weapon->pos.Z)bad=true;int state=isLong?longState:shortState;pool[n].driver=&drivers[n];pool[n].dead=state==2;drivers[n].emitter.length=(blendMode>=2&&blendMode<5)?2.0f:(isLong?5.0f:1.0f);return state==1?0:&pool[n];}

void CTCMultiStrike::ApplyEffectOnWeapon(CThing* w)
{
    if (IS_THING_ALIVE(w)) {
    long effectid = 0;
    long effectidtoblendto = 0;
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    if (PDef->GetSubDef(&pspecial_effects)) {
        CCharString short_effect_name;
        CCharString long_effect_name;
        switch (SpellLevel) {
        case 1:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL2";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL2";
            break;
        case 2:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL3";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL3";
            break;
        case 3:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL4";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL4";
            break;
        case 0:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL1";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL1";
            break;
        default:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL1";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL1";
            break;
        }
        if (!pspecial_effects->GetEffectFromString(short_effect_name, effectid))
            return;
        if (!pspecial_effects->GetEffectFromString(long_effect_name, effectidtoblendto))
            return;
        // the attachment points come in pairs: the short effect is attached at the first and stretched to the second
        long num_pairs = PDef->AttachmentPoint.size() / 2;
        for (long i = 0; i < num_pairs; ++i) {
            long index = i*2;
            CCharString startpoint = PDef->AttachmentPoint[index];
            CCharString endpoint = PDef->AttachmentPoint[index+1];
            C3DVector start_pos;
            C3DVector end_pos;
            CRightHandedSet orientation;
            if (!w->GetTCGraphicAppearance()->GetHelperPoint(startpoint, 0, 0, start_pos, 1.0f) &&
                !w->GetTCGraphicAppearance()->GetDummyObject(startpoint, 0, 0, start_pos, orientation, 1.0f))
                continue;
            if (!w->GetTCGraphicAppearance()->GetHelperPoint(endpoint, 0, 0, end_pos, 1.0f) &&
                !w->GetTCGraphicAppearance()->GetDummyObject(endpoint, 0, 0, end_pos, orientation, 1.0f))
                continue;
                CThing* pfx = CTCDParticleEmitter::Create(effectid, w->GetPos(), true);
                CThing* blend_to = CTCDParticleEmitter::Create(effectidtoblendto, w->GetPos(), true);
                if (IS_THING_ALIVE(pfx) && IS_THING_ALIVE(blend_to)) {
                    CTCDParticleEmitter* ptcEmitter1 = 0;
                    CTCDParticleEmitter* ptcEmitter2 = 0;
                    pfx->GetTC(&ptcEmitter1);
                    blend_to->GetTC(&ptcEmitter2);
                    NParticleEngine::CParticleEmitter* pem1 = ptcEmitter1->GetPEmitter();
                    NParticleEngine::CParticleEmitter* pem2 = ptcEmitter2->GetPEmitter();
                    pem2->Sleep();
                    C3DVector dist_between_attachment_points_vector = start_pos - end_pos;
                    float dist_between_attachment_points = dist_between_attachment_points_vector.GetMagnitude();
                    float long_spline_len = pem2->GetApproximateTotalSplineLength();
                    float short_spline_len = pem1->GetApproximateTotalSplineLength();
                    float distance_between_particle_points = long_spline_len-short_spline_len;
                    float interp = (dist_between_attachment_points-short_spline_len)/distance_between_particle_points;
                    const float MAGIC_FACTOR = 2.0f;
                    pem1->InterpolateTo(*pem2, interp*MAGIC_FACTOR);
                    pem1->SetAlwaysUpdate(true);
                    ptcEmitter1->AttachToThing(*w, 10, startpoint, 0, 0.0f);
                    blend_to->Kill(true);
                    ParticleEffects.push_back(pfx);
                }
        }
    }
    }
}


static bool Close(float a,float b){if(a!=a||b!=b)return a!=a&&b!=b;if(a>1e30f||a< -1e30f||b>1e30f||b< -1e30f)return(a>0)==(b>0);return fabs(a-b)<0.00001;}
static int Fail(int c,const char*s){printf("007ccf50_TEST FAIL %d %s\n",c,s);return 1;}
int main(){const long levels[]={-9,0,1,2,3,4,37};const int counts[]={0,1,2,3,4,8,9};int cases=0,successfulPairs=0,totalCreated=0;
for(int gate=0;gate<6;++gate)for(int l=0;l<7;++l)for(int n=0;n<7;++n)for(int pat=0;pat<16;++pat)for(int state=0;state<9;++state)for(int mode=0;mode<6;++mode,++cases){
 if(stringLive)return Fail(cases,"prior string lifetime");
 bad=false;created=0;shortState=state/3;longState=state%3;blendMode=mode;
 for(int k=0;k<32;++k){if(pool[k].refs)return Fail(cases,"prior pointer lifetime");pool[k]=CThing();drivers[k]=CTCDParticleEmitter();}
 {
  CThing w,prior;weapon=&w;CTCGraphicAppearance appearance;appearance.pattern=pat;appearance.mode=mode;w.graphics=&appearance;w.dead=gate==1;CSpecialEffectsDef effects;CSpecialAbilitiesMultiStrikeDef def;def.effects=&effects;def.found=gate!=2;effects.fail=gate==3?1:(gate==4?2:0);long chosen=levels[l]>=0&&levels[l]<=3?levels[l]:0;effects.shortID=shortID=100+chosen*2;effects.longID=longID=shortID+1;
  for(int j=0;j<counts[n];++j){char name[32];sprintf(name,"P%d",j);def.AttachmentPoint.push_back(CCharString(name));}
  CTCMultiStrike multi;multi.PDef.Set(&def);multi.SpellLevel=levels[l];multi.ParticleEffects.push_back(CIntelligentPointer<CThing>(&prior));
  multi.ApplyEffectOnWeapon(gate==0?0:&w);
  bool look=gate>=2;int queries=gate>=3?(gate==3?1:2):0;bool walk=gate==5;bool start=(pat&3)!=0,end=(pat&12)!=0;int pairs=counts[n]/2;int qualifying=walk&&start&&end?pairs:0;bool retain=shortState==0&&longState==0;int kept=retain?qualifying:0;
  if(bad||def.lookups!=(look?1:0)||(int)effects.queries.size()!=queries)return Fail(cases,"early gates/effect lookup count");
  if(queries){char a[64];sprintf(a,"WEAPON_EFFECT_SHORT_LEVEL%ld",chosen+1);if(effects.queries[0]!=a)return Fail(cases,"short effect selected");if(queries==2){sprintf(a,"WEAPON_EFFECT_LONG_LEVEL%ld",chosen+1);if(effects.queries[1]!=a)return Fail(cases,"long effect selected");}}
  int helpers=walk?pairs*(1+(start?1:0)):0;int dummies=walk?pairs*((pat&1?0:1)+(start&&!(pat&4)?1:0)):0;
  if(appearance.helpers!=helpers||appearance.dummies!=dummies||w.graphicsCalls!=helpers+dummies||w.posCalls!=qualifying*2||created!=qualifying*2)return Fail(cases,"attachment fallback/create call protocol");
  std::vector<int>hp,dp;for(int p=0;walk&&p<pairs;++p){hp.push_back(p*2);if(!(pat&1))dp.push_back(p*2);if(start){hp.push_back(p*2+1);if(!(pat&4))dp.push_back(p*2+1);}}
  if(hp!=appearance.helperPoints||dp!=appearance.dummyPoints)return Fail(cases,"attachment traversal order");
  if((int)multi.ParticleEffects.size()!=kept+1||multi.ParticleEffects[0].Get()!=&prior||prior.refs!=1)return Fail(cases,"retained prefix");
  for(int p=0;p<qualifying;++p){int a=p*2,b=a+1;CTCDParticleEmitter&sd=drivers[a];CTCDParticleEmitter&ld=drivers[b];if(pool[a].getTCs!=(retain?1:0)||pool[b].getTCs!=(retain?1:0)||sd.gets!=(retain?1:0)||ld.gets!=(retain?1:0))return Fail(cases,"live effect component access");if(!retain){if(pool[a].refs||pool[b].refs||pool[a].kills||pool[b].kills||sd.emitter.interpolates||ld.emitter.sleeps)return Fail(cases,"failed effect pair state");continue;}
   float distance=(mode==3||mode==5)?0.0f:(mode==1?1.0f:(mode==4?2.0f:3.0f));float low=(mode>=2&&mode<5)?2.0f:1.0f,high=(mode>=2&&mode<5)?2.0f:5.0f;float expected=2.0f*((distance-low)/(high-low));char point[32];sprintf(point,"P%d",p*2);
   if(ld.emitter.sleeps!=1||sd.emitter.sleeps||sd.emitter.reads!=1||ld.emitter.reads!=1||sd.emitter.interpolates!=1||ld.emitter.interpolates||sd.emitter.other!=&ld.emitter||!Close(sd.emitter.ratio,expected)||sd.emitter.updates!=1||!sd.emitter.always||ld.emitter.updates||sd.attachments!=1||sd.attached!=&w||sd.point!=point||ld.attachments||pool[a].kills||pool[b].kills!=1||!pool[b].dead||pool[a].refs!=1||pool[b].refs||multi.ParticleEffects[p+1].Get()!=&pool[a])return Fail(cases,"blend, attach, kill and retained handle");
  }
  successfulPairs+=kept;totalCreated+=created;
 }
 if(stringLive)return Fail(cases,"string cleanup");
 for(int k=0;k<32;++k)if(pool[k].refs)return Fail(cases,"effect pointer cleanup");
}
if(cases!=254016||!successfulPairs||!totalCreated)return Fail(cases,"case coverage");
 puts("007ccf50_TEST PASS");return 0;}