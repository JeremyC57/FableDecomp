#include <stdio.h>
#include <string.h>
#include <math.h>
namespace fixture_stl {
template<class T,class U> struct pair {T first;U second;pair():first(),second(){}pair(const T& a,const U& b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>& o):first(o.first),second(o.second){}};
template<class T,class U>pair<T,U> make_pair(const T& a,const U& b){return pair<T,U>(a,b);}
template<class T>struct vector {typedef T* iterator;int n;T slots[32];vector():n(0){}int size()const{return n;}T& operator[](int i){return slots[i];}const T& operator[](int i)const{return slots[i];}T* begin(){return slots;}T* end(){return slots+n;}void push_back(const T& x){slots[n++]=x;}T* insert(T* it,const T& x){if(it!=end())throw 2;push_back(x);return slots+n-1;}};
}
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector& b)const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);} C3DVector operator*(float k)const{return C3DVector(X*k,Y*k,Z*k);}};
static const double TWO_PI=6.283185482025146484375;
static double GFSin(float a){return sin(a*TWO_PI);}static double GFCos(float a){return cos(a*TWO_PI);}
struct CThing{bool alive;C3DVector pos;const C3DVector& GetPos()const{return pos;}};
template<class T>struct CIntelligentPointer{T* p;CIntelligentPointer(T* t=0):p(t){}T* Get()const{return p;}};
template<class T>bool IS_THING_ALIVE(T p){return p&&p->alive;}
struct CExplosionRing{float RingRadius;long NumExplosions;float AngleOffset,SecondsBeforeSpawnNewRing;};
struct CExplosionDef{float ExplosionRadiusSmall,ExplosionRadiusLarge,SecondsBeforeEffectReplacement;long ExplosionBlendSmall,ExplosionBlendLarge,ExplosionReplacementBlendSmall,ExplosionReplacementBlendLarge;fixture_stl::vector<CExplosionRing> ExplosionRings;};
static const float DELTA_NOTIONAL_ZERO=0.0001f;
struct CMainGameComponent{static long GetConstantFPS(){return 4;}};
struct CWorldMap{float GetHeightZBelowPos(const C3DVector& p){return p.X-p.Y;}};
struct CWorld{long frame;CWorldMap map;CWorldMap& GetWorldMap(){return map;}long GetFrame(){return frame;}};
static CThing effects[16];static int creations,replacements,mode;static float reported;static C3DVector positions[16];
struct CTCDParticleEmitter{static CThing* Create(long a,long b,float blend,const C3DVector& p,bool flag){if(flag||((a!=3||b!=4)&&(a!=5||b!=6)))throw 3;int id=creations++;positions[id]=p;reported=blend;if(a==5)++replacements;effects[id].alive=mode!=1;effects[id].pos=p;return mode==0?0:&effects[id];}};
struct CTCDExplosion{CWorld world;CThing parent;CWorld& GetWorld(){return world;}CThing& GetParentThing(){return parent;}float DamageRadius,inner,outer;int hits;long CurrentExplosionRing,ExplosionEvolutionTimer;bool CreatedEmitter;CExplosionDef* PExplosionDef;fixture_stl::vector<fixture_stl::pair<CIntelligentPointer<CThing>,long> > Emitters;
void UpdateDynamicHitThings(float x,float y){++hits;inner=x;outer=y;}void UpdateDynamicExplosion();};
namespace {
float __fastcall GetExplosionEffectInterpolate(const CExplosionDef& def, float radius)
{
    float result = 0.0f;
    if (def.ExplosionRadiusLarge - def.ExplosionRadiusSmall > DELTA_NOTIONAL_ZERO) {
        if (radius < def.ExplosionRadiusSmall)
            radius = def.ExplosionRadiusSmall;
        else if (radius > def.ExplosionRadiusLarge)
            radius = def.ExplosionRadiusLarge;
        result = (radius - def.ExplosionRadiusSmall) / (def.ExplosionRadiusLarge - def.ExplosionRadiusSmall); if(result<0.0f)result=0.0f; else if(result>1.0f)result=1.0f;
    }
    return result;
}
}

void CTCDExplosion::UpdateDynamicExplosion()
{
 float wanted_rad=DamageRadius;
 CWorldMap& wmap=GetWorld().GetWorldMap();
 const long const_fps=CMainGameComponent::GetConstantFPS();
 ++ExplosionEvolutionTimer;
 long ring_number=0,next_time=0;
 if(false) {} else for (;ring_number<PExplosionDef->ExplosionRings.size()&&next_time<ExplosionEvolutionTimer;++ring_number) {
  const CExplosionRing& explosion_ring=PExplosionDef->ExplosionRings[ring_number];
  long frames_for_ring=(long)(const_fps*explosion_ring.SecondsBeforeSpawnNewRing);
  next_time+=frames_for_ring;
 }
 --ring_number;
 if(ring_number!=CurrentExplosionRing){
  CurrentExplosionRing=ring_number;CreatedEmitter=true;
  C3DVector line_x(1,0,0),line_y(0,1,0);
  long num_explosions_on_ring=PExplosionDef->ExplosionRings[ring_number].NumExplosions;
  float ring_radius=PExplosionDef->ExplosionRings[ring_number].RingRadius;
  float theta=PExplosionDef->ExplosionRings[ring_number].AngleOffset;
  float dtheta=1.0f/num_explosions_on_ring;
  if(false) {} else for(long c2=0;c2<num_explosions_on_ring;++c2){
   C3DVector pos=GetParentThing().GetPos()+(line_x*(float)GFSin(theta)+line_y*(float)GFCos(theta))*ring_radius;
   pos.Z=wmap.GetHeightZBelowPos(pos);
   float interp=GetExplosionEffectInterpolate(*PExplosionDef,wanted_rad);
   CThing* peffect=CTCDParticleEmitter::Create(PExplosionDef->ExplosionBlendSmall,PExplosionDef->ExplosionBlendLarge,interp,pos,false);
   if(IS_THING_ALIVE(peffect))Emitters.push_back(fixture_stl::make_pair(CIntelligentPointer<CThing>(peffect),GetWorld().GetFrame()));
   theta+=dtheta;
  }
  float inner_rad=ring_number>0?wanted_rad+PExplosionDef->ExplosionRings[ring_number-1].RingRadius:0.0f;
  float outer_rad=ring_radius+wanted_rad;UpdateDynamicHitThings(inner_rad,outer_rad);
 }
 if(PExplosionDef->ExplosionReplacementBlendLarge>0&&PExplosionDef->ExplosionReplacementBlendSmall>0){
  if(false) {} else for(fixture_stl::vector<fixture_stl::pair<CIntelligentPointer<CThing>,long> >::iterator it=Emitters.begin();it!=Emitters.end();){
   CThing* pold_effect_thing=it->first.Get();long frame_created=it->second;bool moved_on=false;
   if(IS_THING_ALIVE(pold_effect_thing)&&frame_created>0){
    long frame_for_replacement=(long)(const_fps*PExplosionDef->SecondsBeforeEffectReplacement+frame_created);
    if(GetWorld().GetFrame()>frame_for_replacement){
     it->second=-1;
     float interp=GetExplosionEffectInterpolate(*PExplosionDef,wanted_rad);
     CThing* pnew_effect_thing=CTCDParticleEmitter::Create(PExplosionDef->ExplosionReplacementBlendSmall,PExplosionDef->ExplosionReplacementBlendLarge,interp,pold_effect_thing->GetPos(),false);
     if(IS_THING_ALIVE(pnew_effect_thing)){it=Emitters.insert(Emitters.end(),fixture_stl::make_pair(CIntelligentPointer<CThing>(pnew_effect_thing),-1));moved_on=true;}
    }
   }
   if(!moved_on)++it;
  }
 }
}

static bool close(float a,float b){return fabs(a-b)<0.00001;}
int main(){
 for(int timer=0;timer<8;++timer)for(int oldring=-1;oldring<2;++oldring)for(mode=0;mode<3;++mode)for(int flags=0;flags<32;++flags)for(int radius=-1;radius<=3;++radius){
  CTCDExplosion w;CExplosionDef def;CThing old[2];CExplosionRing ring;
  ring.RingRadius=2;ring.NumExplosions=2;ring.AngleOffset=0;ring.SecondsBeforeSpawnNewRing=1;def.ExplosionRings.push_back(ring);ring.RingRadius=4;ring.NumExplosions=1;ring.AngleOffset=.25f;def.ExplosionRings.push_back(ring);
  w.PExplosionDef=&def;w.world.frame=(flags&1)?7:6;w.parent.pos=C3DVector(10,3,9);w.ExplosionEvolutionTimer=timer;w.CurrentExplosionRing=oldring;w.DamageRadius=(float)radius;w.hits=0;w.CreatedEmitter=false;
  def.ExplosionRadiusSmall=0;def.ExplosionRadiusLarge=(flags&2)?0:2;def.ExplosionBlendSmall=3;def.ExplosionBlendLarge=4;def.ExplosionReplacementBlendSmall=(flags&4)?5:0;def.ExplosionReplacementBlendLarge=6;def.SecondsBeforeEffectReplacement=1;
  for(int i=0;i<2;++i){old[i].alive=i||((flags&8)!=0);old[i].pos=C3DVector(float(50+i),21,9);w.Emitters.push_back(fixture_stl::make_pair(CIntelligentPointer<CThing>(&old[i]),(flags&16)&&!i?-1L:2L));}
  reported=-1;creations=replacements=0;
  w.UpdateDynamicExplosion();
  int next=timer<4?0:1,n=next==oldring?0:next?1:2,nnew=mode==2?n:0;
  bool due=(flags&4)&&(flags&1),due0=due&&(flags&8)&&!(flags&16);int attempts=due?(due0&&mode==2?1:1+(due0?1:0)):0;
  if(w.ExplosionEvolutionTimer!=timer+1||w.CurrentExplosionRing!=next||w.CreatedEmitter!=(n!=0)||w.hits!=(n!=0)||creations!=n+attempts||replacements!=attempts)return 1;
  if(w.Emitters.n!=2+nnew+(mode==2&&attempts?1:0))return 2;
  if(n&&(!close(w.inner,next?radius+2.0f:0.0f)||!close(w.outer,(next?4.0f:2.0f)+radius)))return 3;
  if(n){if(!close(positions[0].X,next?14.0f:10.0f)||!close(positions[0].Y,next?3.0f:5.0f)||!close(positions[0].Z,positions[0].X-positions[0].Y))return 4;}
  float blend=(flags&2)||radius<0?0.0f:radius>2?1.0f:radius*.5f;if(creations&&!close(reported,blend))return 5;
  if(w.Emitters[0].second!=((flags&16)||due0?-1:2)||w.Emitters[1].second!=(due&&!(due0&&mode==2)?-1:2))return 6;
  if(attempts){C3DVector p=positions[n];if(p.X!=(due0?50:51)||p.Y!=21||p.Z!=9)return 7;}
 }
 puts("0080d300_TEST PASS");return 0;}