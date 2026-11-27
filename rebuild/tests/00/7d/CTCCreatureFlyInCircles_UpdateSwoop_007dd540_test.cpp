#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void ck(bool value) { if (!value) exit(2); }
static int seen[40], count, flags;
static void event(int e) { ck(count < 40); seen[count++]=e; }
static bool bit(int i) { return (flags & (1<<i))!=0; }
#define FABLE_RETAIL_ASSERT(expr) do { if (!(expr)) event(12); } while (0)
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}
 void AccurateNormalise(){float m=(float)sqrt(X*X+Y*Y+Z*Z);ck(m==5);X/=m;Y/=m;Z/=m;}
};
static bool at(const C3DVector&v,float x,float y,float z){return fabs(v.X-x)<.0001&&fabs(v.Y-y)<.0001&&fabs(v.Z-z)<.0001;}
struct CRightHandedSet {};
struct CThing;
struct CTCDExplosion {static CThing* Create(long,const C3DVector&);void SetThingAsImmune(CThing&);};
struct CTCDParticleEmitter {static CThing* Create(long,const C3DVector&,bool);void AttachToThing(CThing&,long,const char*,long,float);};
template<class T> struct CDefPointer {T* ptr;CDefPointer():ptr(0){} T* operator->()const{return ptr;}};
struct CSpecialEffectsDef {bool GetEffectFromString(const char* s,long& out)const{ck(!strcmp(s,"BURNINATE")&&out==0);event(14);out=81;return bit(5);}};
static CSpecialEffectsDef effects;
struct BaseDef {bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* p){event(13);p->ptr=&effects;return bit(4);}};
struct Appearance {bool GetDummyObject(const char* s,int a,int b,C3DVector& pos,CRightHandedSet&){ck(!strcmp(s,"MOUTH_DUMMY")&&!a&&!b);pos=C3DVector(7,4,9);event(11);return bit(3);}};
static Appearance appearance; static BaseDef def;
struct CThing {
 bool dead;
 C3DVector GetPos()const{return C3DVector(9,8,7);}
 Appearance* GetTCGraphicAppearance(){return &appearance;} BaseDef& PeekBaseDef(){return def;}
 bool GetTC(CTCDExplosion**); bool GetTC(CTCDParticleEmitter**);
 void Kill(bool b){ck(b&&!dead);dead=true;event(18);}
};
static CThing creature,target,oldEffect,newEffect,explosion;
static CTCDExplosion boom;static CTCDParticleEmitter emitter;
static bool IS_THING_ALIVE(CThing* p){return p && !p->dead;}
bool CThing::GetTC(CTCDExplosion** p){ck(this==&explosion);event(9);*p=&boom;return bit(2);}
bool CThing::GetTC(CTCDParticleEmitter** p){ck(this==&newEffect);event(16);*p=&emitter;return bit(7);}
void CTCDExplosion::SetThingAsImmune(CThing& c){ck(&c==&creature);event(10);}
void CTCDParticleEmitter::AttachToThing(CThing& c,long type,const char* s,long i,float f){ck(&c==&creature && type==42 && !strcmp(s,"MOUTH_DUMMY") && !i && !f);event(17);}
CThing* CTCDExplosion::Create(long id,const C3DVector& p){ck(id==77&&at(p,40,54,-2));event(8);return bit(1)?&explosion:0;}
CThing* CTCDParticleEmitter::Create(long id,const C3DVector& p,bool mode){ck(id==81&&at(p,7,4,9)&&!mode);event(15);return bit(6)?&newEffect:0;}
struct Spline {
 void GetSplinePosition(C3DVector& p,float t){ck(t>=0&&t<=1.25f);p=C3DVector(4,6,5);event(1);}
 void SetControlPoint(long i,const C3DVector& p){ck(i==1&&at(p,7,11,11));event(2);}
};
struct CWorld{static long GetFrame(){event(3);return 70;}};
struct Map {
 bool IsPosInMap(const C3DVector& p){ck(at(p,40,54,5));event(6);return bit(0);}
 float GetHeightZBelowPos(const C3DVector& p){ck(at(p,40,54,10000));event(7);return -2;}
};
static Map map;
struct CGameDefinitionManager {
 static CGameDefinitionManager& Get(){static CGameDefinitionManager manager;return manager;}
 long GetDefGlobalIndexFromName(const char*s){ck(!strcmp(s,"DRAGON_FIRE_TRAIL"));event(5);return 77;}
};
struct CTCCreatureFlyInCircles {
 enum EFlyMode{eHover,eFly,eSwoop}; EFlyMode Mode;
 float SwoopProgress,SwoopDelta;long LastExplodeTick;Spline SwoopSpline;
 CThing &Creature,*PSwoopTarget,*PBurninationEffect;C3DVector SwoopTargOffset;
 CTCCreatureFlyInCircles():Creature(creature),SwoopTargOffset(-2,3,4){}
 Map& PeekWorldMap(){return map;} Map& GetWorldMap(){return map;}
 void UpdateSwoop(C3DVector&,C3DVector&);
};
void CTCCreatureFlyInCircles::UpdateSwoop(C3DVector& rPos, C3DVector& rFwd)
{
    C3DVector newPos;
    SwoopProgress += SwoopDelta;
    SwoopSpline.GetSplinePosition(newPos, SwoopProgress);
    rFwd = newPos - rPos;
    rFwd.AccurateNormalise();
    rPos = newPos;
    if (SwoopProgress >= 1.0f || !IS_THING_ALIVE(PSwoopTarget)) {
        Mode = eFly;
        PSwoopTarget = 0;
        return;
    }
    if (SwoopProgress < 0.45f) {
        if (SwoopProgress < 0.3f)
            SwoopSpline.SetControlPoint(1, PSwoopTarget->GetPos() + SwoopTargOffset);
        if (CWorld::GetFrame() - LastExplodeTick > 1) {
            long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("DRAGON_FIRE_TRAIL");
            C3DVector explodePos = newPos + rFwd * 60.0f;
            if (PeekWorldMap().IsPosInMap(explodePos)) {
                explodePos.Z = 10000.0f;
                explodePos.Z = GetWorldMap().GetHeightZBelowPos(explodePos);
                CThing* pExplosion = CTCDExplosion::Create(defIndex, explodePos);
                if (pExplosion) {
                    CTCDExplosion* pTCDExplosion;
                    if (pExplosion->GetTC(&pTCDExplosion))
                        pTCDExplosion->SetThingAsImmune(Creature);
                    LastExplodeTick = CWorld::GetFrame();
                }
            }
            if (!IS_THING_ALIVE(PBurninationEffect)) {
                CDefPointer<const CSpecialEffectsDef> pEffects;
                long effect = 0;
                C3DVector mouthPos;
                CRightHandedSet mouthOri;
                if (!Creature.GetTCGraphicAppearance()->GetDummyObject("MOUTH_DUMMY", 0, 0, mouthPos, mouthOri)) {
                    FABLE_RETAIL_ASSERT(false);
                }
                if (Creature.PeekBaseDef().GetSubDef(&pEffects) && pEffects->GetEffectFromString("BURNINATE", effect)) {
                    PBurninationEffect = CTCDParticleEmitter::Create(effect, mouthPos, false);
                    if (IS_THING_ALIVE(PBurninationEffect)) {
                        CTCDParticleEmitter* pTCDEmitter;
                        if (PBurninationEffect->GetTC(&pTCDEmitter))
                            pTCDEmitter->AttachToThing(Creature, 42, "MOUTH_DUMMY", 0, 0.0f);
                    }
                }
            }
        }
    } else if (IS_THING_ALIVE(PBurninationEffect)) {
        PBurninationEffect->Kill(true);
        PBurninationEffect = 0;
    }
}
int main(){
 const float progress[]={.125f,.3f,.449f,.45f,.75f,1.0f,1.25f};
 for(int i=0;i<7;++i)for(int targetState=0;targetState<3;++targetState)
 for(int burnState=0;burnState<3;++burnState)for(int gap=1;gap<=2;++gap)
 for(flags=0;flags<512;++flags){
  const float t=progress[i];
  CTCCreatureFlyInCircles c;c.Mode=c.eSwoop;c.SwoopDelta=.125f;c.SwoopProgress=t-c.SwoopDelta;
  c.LastExplodeTick=70-gap;target.dead=targetState==1;c.PSwoopTarget=targetState?&target:0;
  oldEffect.dead=burnState==1;c.PBurninationEffect=burnState?&oldEffect:0;newEffect.dead=bit(8);
  CThing* result=c.PBurninationEffect;long tick=c.LastExplodeTick;count=0;
  C3DVector pos(1,2,5),fwd;c.UpdateSwoop(pos,fwd);
  ck(at(pos,4,6,5)&&at(fwd,.6f,.8f,0)&&c.SwoopProgress==t);
  int expected[40],n=0;expected[n++]=1;
  bool end=t>=1 || targetState!=2;
  if(!end && t<.45f){
   if(t<.3f)expected[n++]=2;
   expected[n++]=3;
   if(gap>1){
    expected[n++]=5;expected[n++]=6;
    if(bit(0)){expected[n++]=7;expected[n++]=8;if(bit(1)){expected[n++]=9;if(bit(2))expected[n++]=10;expected[n++]=3;tick=70;}}
    if(burnState!=2){expected[n++]=11;if(!bit(3))expected[n++]=12;expected[n++]=13;
     if(bit(4)){expected[n++]=14;if(bit(5)){expected[n++]=15;result=bit(6)?&newEffect:0;
       if(bit(6)&&!bit(8)){expected[n++]=16;if(bit(7))expected[n++]=17;}
     }}
    }
   }
  }else if(!end && burnState==2){expected[n++]=18;result=0;}
  ck(count==n&&c.PBurninationEffect==result&&c.LastExplodeTick==tick);
  ck(c.Mode==(end?c.eFly:c.eSwoop) && c.PSwoopTarget==(end?0:&target));
  for(int j=0;j<n;++j)ck(seen[j]==expected[j]);
 }
 puts("007dd540_TEST PASS");return 0;
}