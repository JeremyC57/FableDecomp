
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
static void check(bool value) { if(!value) exit(1); }
#define FABLE_RETAIL_ASSERT_ARM(x) ((void)0)
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
    C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
    C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}
    C3DVector operator/(float f)const{return *this*(1.0f/f);}
};
static bool close(const C3DVector& a,const C3DVector& b){return fabs(a.X-b.X)<0.0001&&fabs(a.Y-b.Y)<0.0001&&fabs(a.Z-b.Z)<0.0001;}
struct CCharString{};static const CCharString MAIN_FOCAL_POSITION_NAME=CCharString();
static int flags[3],aliveCalls[3],getEmitter,getPos,getAppearance,focusCalls;
struct CTCDParticleEmitter{
 C3DVector GetEmitterPosition(){++getEmitter;return C3DVector(8,4,-12);}
};
namespace NGraphicAppearance{struct CTCGraphicAppearance{
 bool succeeds;
 bool GetHelperPoint(const CCharString& n,long index,unsigned long mask,C3DVector& pos,float time)const {
   check(&n==&MAIN_FOCAL_POSITION_NAME && index==0 && mask==0 && time==1.0f);
   check(close(pos,C3DVector(-2,6,10)));++focusCalls;if(succeeds)pos=C3DVector(-4,2,16);return succeeds;
 }
};}
struct CThing{
 CTCDParticleEmitter* emitter;NGraphicAppearance::CTCGraphicAppearance* graphic;
 bool GetTC(CTCDParticleEmitter** p){*p=emitter;return true;}
 C3DVector GetPos(){++getPos;return C3DVector(-2,6,10);}
 const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){++getAppearance;return graphic;}
};
struct Handle{int id;CThing* thing;CThing* operator->()const{return thing;}};
static bool IS_THING_ALIVE(const Handle& p){++aliveCalls[p.id];return flags[p.id]!=0;}
struct CTCWillLightning{
 enum ELightningState{INACTIVE,DELAYING,ACTIVATING,ACTIVE,DEACTIVATINGCHILDREN,DEACTIVATING,DEAD};
 struct CLightningBeam{
  ELightningState m_lsState;Handle m_tpParticleHandle,m_tcpHome,m_tcpTarget;C3DVector m_vStartPos,m_vEndPos,m_vDestination;
  float m_fTime,m_fTimeBase,m_fActivationTime,m_fDeActivationTime;bool m_bTemporary;
  bool IsTemporary(){return m_bTemporary;}void UpdatePositions();
 };
};
void CTCWillLightning::CLightningBeam::UpdatePositions()
{
    C3DVector vAbsoluteStart(0, 0, 0);
    C3DVector vAbsoluteDestination(0, 0, 0);
    if (IS_THING_ALIVE(m_tpParticleHandle)) {
        CTCDParticleEmitter* tcParticleEmitter = 0;
        if (!m_tpParticleHandle->GetTC(&tcParticleEmitter)) {
            FABLE_RETAIL_ASSERT_ARM(false);
        }
        vAbsoluteStart = tcParticleEmitter->GetEmitterPosition();
    }
    if (IS_THING_ALIVE(m_tcpHome))
        m_vStartPos = vAbsoluteStart;
    switch (m_lsState) {
    case DELAYING:
        m_vEndPos = m_vStartPos;
        break;
    case ACTIVATING:
        if (!IsTemporary() && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
            C3DVector focus = m_tcpTarget->GetPos();
            const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = m_tcpTarget->GetTCGraphicAppearance();
            if (ptc_graphic_appearance && !ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, focus, 1.0f)) {
                FABLE_RETAIL_ASSERT_ARM(false);
            }
            m_vDestination = focus;
        }
        m_vEndPos = (m_vDestination - m_vStartPos) * (m_fTime - m_fTimeBase) / m_fActivationTime + m_vStartPos;
        break;
    case ACTIVE:
        if (!IsTemporary() && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
            if (!IsTemporary() && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
                C3DVector focus = m_tcpTarget->GetPos();
                const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = m_tcpTarget->GetTCGraphicAppearance();
                if (ptc_graphic_appearance && !ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, focus, 1.0f)) {
                    FABLE_RETAIL_ASSERT_ARM(false);
                }
                m_vDestination = focus;
            }
        }
        m_vEndPos = m_vDestination;
        break;
    case DEACTIVATINGCHILDREN:
        if (!IsTemporary() && IS_THING_ALIVE(m_tcpTarget) && IS_THING_ALIVE(m_tcpHome)) {
            if (!IsTemporary() && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
                C3DVector focus = m_tcpTarget->GetPos();
                const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = m_tcpTarget->GetTCGraphicAppearance();
                if (ptc_graphic_appearance && !ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, focus, 1.0f)) {
                    FABLE_RETAIL_ASSERT_ARM(false);
                }
                m_vDestination = focus;
            }
        }
        m_vEndPos = m_vDestination;
        break;
    case DEACTIVATING:
        m_vEndPos = (m_vDestination - m_vStartPos) * (m_fDeActivationTime - (m_fTime - m_fTimeBase)) / m_fDeActivationTime + m_vStartPos;
        break;
    }
}

static C3DVector interpolate(const C3DVector& a,const C3DVector& b,float f){
 return C3DVector(a.X+(b.X-a.X)*f,a.Y+(b.Y-a.Y)*f,a.Z+(b.Z-a.Z)*f);
}
int main(){
 int cases=0;for(int state=0;state<7;++state)for(int mode=0;mode<64;++mode)for(int phase=-1;phase<=5;++phase){
  CTCWillLightning::CLightningBeam b;CThing home,target,particle;CTCDParticleEmitter emitter;NGraphicAppearance::CTCGraphicAppearance appearance;
  appearance.succeeds=(mode&32)!=0;target.graphic=(mode&16)?&appearance:0;particle.emitter=&emitter;
  b.m_tpParticleHandle.id=0;b.m_tpParticleHandle.thing=&particle;b.m_tcpHome.id=1;b.m_tcpHome.thing=&home;b.m_tcpTarget.id=2;b.m_tcpTarget.thing=&target;
  for(int j=0;j<3;++j){flags[j]=(mode>>j)&1;aliveCalls[j]=0;}
  getEmitter=getPos=getAppearance=focusCalls=0;
  b.m_lsState=static_cast<CTCWillLightning::ELightningState>(state);b.m_bTemporary=(mode&8)!=0;
  b.m_fTimeBase=3;b.m_fTime=3+phase;b.m_fActivationTime=4;b.m_fDeActivationTime=2;
  b.m_vStartPos=C3DVector(3,-6,9);b.m_vEndPos=C3DVector(7,12,-8);b.m_vDestination=C3DVector(-12,3,5);
  b.UpdatePositions();
  const bool refresh=state>=2&&state<=4&&!(mode&8)&&flags[1]&&flags[2];
  int h=1,t=0;if(state>=2&&state<=4&&!(mode&8)){
   if(state==4){++t;if(flags[2])++h;}else{++h;if(flags[1])++t;}
   if(refresh&&state>=3){++h;++t;}
  }
  check(aliveCalls[0]==1&&aliveCalls[1]==h&&aliveCalls[2]==t);
  check(getEmitter==flags[0]&&getPos==(int)refresh&&getAppearance==(int)refresh&&focusCalls==(refresh&&(mode&16)));
  C3DVector start=flags[1]?(flags[0]?C3DVector(8,4,-12):C3DVector()):C3DVector(3,-6,9);
  C3DVector dest=refresh?(((mode&48)==48)?C3DVector(-4,2,16):C3DVector(-2,6,10)):C3DVector(-12,3,5);
  C3DVector end(7,12,-8);switch(state){case 1:end=start;break;case 2:end=interpolate(start,dest,phase/4.0f);break;case 3:case 4:end=dest;break;case 5:end=interpolate(start,dest,1.0f-phase/2.0f);break;}
  check(close(b.m_vStartPos,start)&&close(b.m_vEndPos,end)&&close(b.m_vDestination,dest));++cases;
 }check(cases==3136);puts("0079f970_TEST PASS");return 0;
}