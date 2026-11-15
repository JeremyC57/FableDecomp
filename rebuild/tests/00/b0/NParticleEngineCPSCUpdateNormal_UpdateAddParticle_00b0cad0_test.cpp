// Full selected body; declared-only engine calls have instrumented canned outputs.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cfloat>
static int failures=0;static void check(bool x){if(!x)++failures;}
static bool closef(float x,double y){return std::fabs(double(x)-y)<0.0002*(1+std::fabs(y));}
static unsigned long step(unsigned long&seed){unsigned int x=(unsigned int)seed;x=x*0x24a1u+0x24dfu;x=(x>>13)|(x<<19);seed=x;return seed;}
static unsigned long modelStep(unsigned long&seed){unsigned long long x=((unsigned long long)seed*9377u+9439u)&0xffffffffULL;seed=(unsigned long)(((x/8192u)+(x%8192u)*524288u)&0xffffffffULL);return seed;}
static long roundNear(double x){double lo=std::floor(x),f=x-lo;if(f>0.5||(f==0.5&&std::fmod(lo,2.0)!=0))lo+=1;return(long)lo;}
static const unsigned int invTable[128]={0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,255u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,153u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u};
struct C3DVector {float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}bool IsNonZero()const{return Y*Y+X*X+Z*Z>0.00000001f;}
 const C3DVector operator^(const C3DVector&v)const{return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X);}
 const C3DVector&operator+=(const C3DVector&v){X+=v.X;Y+=v.Y;Z+=v.Z;return*this;}
 C3DVector&AccurateNormalise();C3DVector&Normalise(){float n=X*X+Y*Y+Z*Z;union {float f;unsigned int i;}u;u.f=n;u.i=(((0x17cu-((u.i>>23)&255u))>>1)<<23)|(invTable[(u.i>>17)&127u]<<15);float f=u.f;f=(3.0f-f*f*n)*f*0.5f;X*=f;Y*=f;Z*=f;return*this;}
 static C3DVector GetRandomUnitVector(unsigned long*seed);
};
static bool vecEq(const C3DVector&a,const C3DVector&b){return closef(a.X,b.X)&&closef(a.Y,b.Y)&&closef(a.Z,b.Z);}
struct CRightHandedSet {C3DVector up,forward;void SetForward(const C3DVector&v){forward=v;}void SetUp(const C3DVector&v){up=v;}
 void Init(const C3DVector&u,const C3DVector&f){up=u;forward=f;}const C3DVector&GetForward()const{return forward;}const C3DVector&GetUp()const{return up;}C3DVector GetRight()const{return forward^up;}void MakePerpendicular();};
struct CMatrix3x4 {float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 void Set(const CRightHandedSet&r,const C3DVector&t){C3DVector q=r.GetRight();E11=q.X;E12=q.Y;E13=q.Z;E21=r.forward.X;E22=r.forward.Y;E23=r.forward.Z;E31=r.up.X;E32=r.up.Y;E33=r.up.Z;E41=t.X;E42=t.Y;E43=t.Z;}
};
struct CQuaternion {float X,Y,Z,W;CQuaternion(){}CQuaternion(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}
 void SetIdentity(){X=Y=Z=0;W=1;}void SetFromAxisAngle(const C3DVector&,float);CQuaternion operator*(const CQuaternion&)const;void SetFromRotationMatrix(const CMatrix3x4&);
 void ToRightHandedSet(CRightHandedSet&d)const{float n=X*X+Y*Y+Z*Z+W*W;float s=n!=0?2.0f/n:1.0f;float xs=X*s,ys=Y*s,zs=Z*s;float wx=W*xs,wy=W*ys,wz=W*zs,xx=X*xs,xy=X*ys,xz=X*zs,yy=Y*ys,yz=Y*zs,zz=Z*zs;d.Init(C3DVector(xz-wy,yz+wx,1-(xx+yy)),C3DVector(xy+wz,1-(xx+zz),yz-wx));}
};
static bool quatEq(const CQuaternion&a,const CQuaternion&b){return closef(a.X,b.X)&&closef(a.Y,b.Y)&&closef(a.Z,b.Z)&&closef(a.W,b.W);}
struct Trace {int random,unit,normalise,make,matrix,multiply,axis,allocate,params,emitter;C3DVector axes[5],beforeUp,beforeForward;float angles[5];CQuaternion mulA[2],mulB[2];};
static Trace tr;static long tick;static unsigned long rng;static bool allocateOK,paramOK;static C3DVector gameVector,emitterVector;static float fade;
C3DVector&C3DVector::AccurateNormalise(){++tr.normalise;double f=1.0/std::sqrt(double(X)*X+double(Y)*Y+double(Z)*Z);X=float(X*f);Y=float(Y*f);Z=float(Z*f);return*this;}
C3DVector C3DVector::GetRandomUnitVector(unsigned long*seed){++tr.unit;step(*seed);return C3DVector(0,1,0);}
void CRightHandedSet::MakePerpendicular(){++tr.make;tr.beforeUp=up;tr.beforeForward=forward;up=C3DVector(0,0,1);forward=C3DVector(0,1,0);}
static CQuaternion axisResult(int n){return CQuaternion(0.1f*n,0.2f*n,-0.3f*n,1.0f);}
static CQuaternion mulResult(int n){return CQuaternion(0.3f*n,-0.1f*n,0.2f*n,0.9f);}
static CQuaternion matrixResult(){return CQuaternion(0.2f,-0.3f,0.4f,0.8f);}
void CQuaternion::SetFromAxisAngle(const C3DVector&a,float x){check(tr.axis<5);tr.axes[tr.axis]=a;tr.angles[tr.axis]=x;*this=axisResult(++tr.axis);}
CQuaternion CQuaternion::operator*(const CQuaternion&q)const{check(tr.multiply<2);tr.mulA[tr.multiply]=*this;tr.mulB[tr.multiply]=q;return mulResult(++tr.multiply);}
void CQuaternion::SetFromRotationMatrix(const CMatrix3x4&m){++tr.matrix;check(m.E11==1&&m.E12==0&&m.E13==0&&m.E21==0&&m.E22==1&&m.E23==0&&m.E31==0&&m.E32==0&&m.E33==1&&m.E41==0&&m.E42==0&&m.E43==0);*this=matrixResult();}
static float GFFloatRandom(float range,unsigned long*seed){++tr.random;return float((step(*seed)&65535u)/65536.0)*range;}
static unsigned long GFRandom(unsigned long range,unsigned long*seed){return range?step(*seed)%range:0;}
static float GFGetInterpolatedValue(float a,float b,float t){return(b-a)*t+a;}
static const float DELTA_NOTIONAL_ZERO=0.0001f;
namespace NParticleEngine {
struct CParticle {CQuaternion Orientation,OrientationChange;float Angle; signed char AngleChange;C3DVector PositionBuffer[4],Velocity,Acceleration;short Lifetimer;unsigned char CreationTime;short MaxLife;unsigned char FlickerIndex,FlickerTimeOffset;bool Active;float ClosestAttractorDistance;
 void SetClosestAttractorDistance(float d){ClosestAttractorDistance=d;}void SetPosition(const C3DVector&p){std::memmove(PositionBuffer+1,PositionBuffer,3*sizeof(C3DVector));PositionBuffer[0]=p;}void ClearAcceleration(){Acceleration.X=Acceleration.Y=Acceleration.Z=0;}
 void SetMaxLife(short l){MaxLife=l?l:1;}void SetLifetimer(short t){Lifetimer=t;}void SetCreationTime(float t){CreationTime=(unsigned char)roundNear(float(t*255));}void Activate(){Active=true;for(unsigned long c1=1;c1<4;++c1)PositionBuffer[c1]=PositionBuffer[0];}
 void SetVelocity(const C3DVector&v){Velocity=v;}void SetOrientation(const CQuaternion&q){Orientation=q;}void SetOrientationChange(const CQuaternion&q){OrientationChange=q;}void SetAngle(float a){Angle=a;}void SetAngleChange(float a){AngleChange=(signed char)roundNear(float(a*63.5f));}void SetFlickerIndex(unsigned char i){FlickerIndex=i;}void SetFlickerTimeOffset(unsigned char t){FlickerTimeOffset=t;}
};
static CParticle output;
struct CParticleSystem {CParticle*AllocateParticle(){++tr.allocate;return allocateOK?&output:0;}const C3DVector&PeekEmitterVelocity()const{++tr.emitter;return emitterVector;}};
template<class K,class V>struct CParticleUpdateParameters {bool GetVectorParam(C3DVector&v,const K&)const{++tr.params;if(paramOK)v=gameVector;return paramOK;}};
struct CPSCUpdateNormal {bool EmissionFadeEnable,SetOrientationFromDirection,SetOrientationFromGame,UseRandomInitialRotation,UseRandomRotationAxis,StayWithEmitter;float EmissionFadeMinimum,ParticleLifeSecs,InitialRotationX,InitialRotationY,InitialRotationZ,RotationMinAngleSpeed,RotationMaxAngleSpeed;C3DVector RotationAxis;unsigned long OrientFromGameParam;
 float GetCurrentFadeValue()const{return fade;}void UpdateAddParticle(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,C3DVector,C3DVector,float);
};
static long GetTickRate(){return tick;}static unsigned long&GetRandomSeed(){return rng;}
void CPSCUpdateNormal::UpdateAddParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                         C3DVector position, C3DVector velocity, float interpolate)
{
    unsigned long& seed = GetRandomSeed();
    if (EmissionFadeEnable) {
        float fade_val = GetCurrentFadeValue();
        fade_val = EmissionFadeMinimum + (1.0f - EmissionFadeMinimum) * fade_val;
        if (GFFloatRandom(1.0f, &seed) > fade_val)
            return;
    }
    CParticle* pparticle = system.AllocateParticle();
    if (pparticle == 0)
        return;
    float tick_rate = (float)GetTickRate();
    pparticle->SetClosestAttractorDistance(FLT_MAX);
    pparticle->SetPosition(position);
    pparticle->ClearAcceleration();
    pparticle->SetMaxLife((short)(tick_rate * ParticleLifeSecs));
    pparticle->SetLifetimer(-1);
    pparticle->SetCreationTime(interpolate);
    pparticle->Activate();
    pparticle->SetVelocity(velocity);

    CQuaternion initial_orientation;
    float initial_angle = 0.0f;
    if (SetOrientationFromDirection || SetOrientationFromGame) {
        C3DVector movement_direction(0.0f, 0.0f, 0.0f);
        if (SetOrientationFromGame) {
            params.GetVectorParam(movement_direction, OrientFromGameParam);
        } else {
            if (velocity.IsNonZero())
                movement_direction = velocity;
            if (StayWithEmitter)
                movement_direction += system.PeekEmitterVelocity();
        }
        if (movement_direction.IsNonZero()) {
            movement_direction.AccurateNormalise();
            CRightHandedSet rhs;
            rhs.SetForward(movement_direction.AccurateNormalise());
            C3DVector cross_vec(0.0f,0.0f,1.0f);
            if(fabs(movement_direction.Z)>0.99f) {
                if(fabs(movement_direction.X)>fabs(movement_direction.Y)) cross_vec.Initialise(0.0f,1.0f,0.0f);
                else cross_vec.Initialise(1.0f,0.0f,0.0f);
            }
            rhs.SetUp((movement_direction ^ cross_vec) ^ movement_direction);
            rhs.MakePerpendicular();
            CMatrix3x4 rot_mat;
            rot_mat.Set(rhs, C3DVector(0.0f, 0.0f, 0.0f));
            initial_orientation.SetFromRotationMatrix(rot_mat);
        } else {
            initial_orientation.SetIdentity();
        }
    } else {
        if (UseRandomInitialRotation) {
            C3DVector axis = C3DVector::GetRandomUnitVector(&seed);
            initial_angle = GFFloatRandom(1.0f, &seed);
            initial_orientation.SetFromAxisAngle(axis, initial_angle);
        } else {
            CQuaternion rot_x,rot_y,rot_z;
            initial_angle = InitialRotationX;
            rot_x.SetFromAxisAngle(C3DVector(1.0f, 0.0f, 0.0f), InitialRotationX);
            rot_y.SetFromAxisAngle(C3DVector(0.0f, 1.0f, 0.0f), InitialRotationY);
            rot_z.SetFromAxisAngle(C3DVector(0.0f, 0.0f, 1.0f), InitialRotationZ);
            initial_orientation = rot_x * rot_y * rot_z;
        }
        }
    CQuaternion orientation_change;
    float angle_change = GFGetInterpolatedValue(RotationMinAngleSpeed,RotationMaxAngleSpeed,GFFloatRandom(1.0f,&seed));
    C3DVector rotation_axis;
    if (SetOrientationFromDirection || SetOrientationFromGame) {
        orientation_change.SetIdentity();
        CRightHandedSet rhs;
        initial_orientation.ToRightHandedSet(rhs);
        rotation_axis = rhs.GetForward();
    }
    if (UseRandomRotationAxis) {
        rotation_axis = C3DVector::GetRandomUnitVector(&seed);
        orientation_change.SetFromAxisAngle(rotation_axis, angle_change);
    } else if (RotationAxis.IsNonZero()) {
        rotation_axis = RotationAxis;
        rotation_axis.Normalise();
        orientation_change.SetFromAxisAngle(rotation_axis, angle_change);
    } else {
        orientation_change.SetIdentity();
    }
    pparticle->SetOrientation(initial_orientation);
    pparticle->SetOrientationChange(orientation_change);
    pparticle->SetAngle(initial_angle);
    pparticle->SetAngleChange(angle_change);
    pparticle->SetFlickerIndex((unsigned char)GFRandom(256, &seed));
    pparticle->SetFlickerTimeOffset((unsigned char)GFRandom(256, &seed));
}
}

static C3DVector modelForward(const CQuaternion&q){double n=double(q.X)*q.X+double(q.Y)*q.Y+double(q.Z)*q.Z+double(q.W)*q.W,s=n?2/n:1;return C3DVector(float((double(q.X)*q.Y+double(q.W)*q.Z)*s),float(1-(double(q.X)*q.X+double(q.Z)*q.Z)*s),float((double(q.Y)*q.Z-double(q.W)*q.X)*s));}
int main(){using namespace NParticleEngine;unsigned long cases=0;CParticleSystem system;CParticleUpdateParameters<unsigned long,unsigned long>params;
 const float lives[3]={-1.0f,0.01f,5.0f};
 for(int bits=0;bits<64;++bits)for(int axisMode=0;axisMode<3;++axisMode)for(int velocityMode=0;velocityMode<3;++velocityMode)for(int fm=0;fm<3;++fm)for(int tm=0;tm<2;++tm)for(int lm=0;lm<3;++lm)for(int it=0;it<3;++it)for(int alloc=0;alloc<2;++alloc){
  std::memset(&tr,0,sizeof(tr));std::memset(&output,0x55,sizeof(output));output.Active=false;CParticle before=output;CPSCUpdateNormal u;
  u.EmissionFadeEnable=(bits&1)!=0;u.UseRandomInitialRotation=(bits&2)!=0;u.SetOrientationFromDirection=(bits&4)!=0;u.SetOrientationFromGame=(bits&8)!=0;u.UseRandomRotationAxis=(bits&16)!=0;u.StayWithEmitter=(bits&32)!=0;
  u.EmissionFadeMinimum=0.2f;u.ParticleLifeSecs=lives[lm];u.InitialRotationX=0.1f;u.InitialRotationY=-0.2f;u.InitialRotationZ=0.3f;u.RotationMinAngleSpeed=-0.5f;u.RotationMaxAngleSpeed=1.0f;u.RotationAxis=axisMode==0?C3DVector(0,0,0):(axisMode==1?C3DVector(3,4,0):C3DVector(0,0,-2));u.OrientFromGameParam=123;
  C3DVector velocity=velocityMode==0?C3DVector(0,0,0):(velocityMode==1?C3DVector(1,2,0):C3DVector(0,0,3));C3DVector position(7,-11,4);emitterVector=C3DVector(0.25f,-0.5f,0.75f);gameVector=velocity;paramOK=velocityMode!=0;
  fade=fm*0.5f;tick=tm?60:15;allocateOK=alloc!=0;float interpolate=it*0.5f;unsigned long initial=(unsigned long)(12345+cases*7)&0xffffffffUL;rng=initial;unsigned long expectedSeed=initial;int expectedRandom=0,expectedUnit=0;
  bool spawn=true;if(u.EmissionFadeEnable){double value=(modelStep(expectedSeed)&65535u)/65536.0;++expectedRandom;float threshold=u.EmissionFadeMinimum+(1.0f-u.EmissionFadeMinimum)*fade;if(value>threshold)spawn=false;}
  bool allocateCalled=spawn;spawn=spawn&&allocateOK;
  u.UpdateAddParticle(system,params,position,velocity,interpolate);
  check(tr.allocate==(allocateCalled?1:0));
  if(!spawn){check(std::memcmp(&before,&output,sizeof(output))==0&&tr.axis==0&&tr.unit==0&&tr.random==expectedRandom&&rng==expectedSeed);++cases;continue;}
  CQuaternion expectedOrientation,expectedChange;float expectedAngle=0;int expectedAxes=0,expectedMatrix=0,expectedNorm=0;C3DVector expectedArgs[5];float expectedAngles[5];bool orient=u.SetOrientationFromDirection||u.SetOrientationFromGame;
  if(orient){C3DVector move(0,0,0);if(u.SetOrientationFromGame){if(paramOK)move=gameVector;}else{if(velocityMode!=0)move=velocity;if(u.StayWithEmitter){move.X+=emitterVector.X;move.Y+=emitterVector.Y;move.Z+=emitterVector.Z;}}
   double n=double(move.X)*move.X+double(move.Y)*move.Y+double(move.Z)*move.Z;
   if(n>0.00000001){++expectedMatrix;expectedNorm=2;double f=1/std::sqrt(n);C3DVector forward(float(move.X*f),float(move.Y*f),float(move.Z*f));C3DVector cross(0,0,1);if(std::fabs(forward.Z)>0.99f)cross=std::fabs(forward.X)>std::fabs(forward.Y)?C3DVector(0,1,0):C3DVector(1,0,0);double dot=double(forward.X)*cross.X+double(forward.Y)*cross.Y+double(forward.Z)*cross.Z;C3DVector up(float(cross.X-forward.X*dot),float(cross.Y-forward.Y*dot),float(cross.Z-forward.Z*dot));check(vecEq(tr.beforeForward,forward)&&vecEq(tr.beforeUp,up));expectedOrientation=matrixResult();}
   else expectedOrientation.SetIdentity();
  }else if(u.UseRandomInitialRotation){modelStep(expectedSeed);++expectedUnit;expectedAngle=float((modelStep(expectedSeed)&65535u)/65536.0);++expectedRandom;expectedArgs[expectedAxes]=C3DVector(0,1,0);expectedAngles[expectedAxes++]=expectedAngle;expectedOrientation=axisResult(1);}
  else{expectedAngle=u.InitialRotationX;expectedArgs[0]=C3DVector(1,0,0);expectedAngles[0]=u.InitialRotationX;expectedArgs[1]=C3DVector(0,1,0);expectedAngles[1]=u.InitialRotationY;expectedArgs[2]=C3DVector(0,0,1);expectedAngles[2]=u.InitialRotationZ;expectedAxes=3;expectedOrientation=mulResult(2);check(tr.multiply==2&&quatEq(tr.mulA[0],axisResult(1))&&quatEq(tr.mulB[0],axisResult(2))&&quatEq(tr.mulA[1],mulResult(1))&&quatEq(tr.mulB[1],axisResult(3)));}
  float amount=float((modelStep(expectedSeed)&65535u)/65536.0);++expectedRandom;float speed=float(double(u.RotationMinAngleSpeed)+(double(u.RotationMaxAngleSpeed)-u.RotationMinAngleSpeed)*amount);C3DVector axis;
  if(orient)axis=modelForward(expectedOrientation);
  if(u.UseRandomRotationAxis){modelStep(expectedSeed);++expectedUnit;axis=C3DVector(0,1,0);expectedArgs[expectedAxes]=axis;expectedAngles[expectedAxes++]=speed;expectedChange=axisResult(expectedAxes);}
  else if(axisMode!=0){double len=std::sqrt(double(u.RotationAxis.X)*u.RotationAxis.X+double(u.RotationAxis.Y)*u.RotationAxis.Y+double(u.RotationAxis.Z)*u.RotationAxis.Z);axis=C3DVector(float(u.RotationAxis.X/len),float(u.RotationAxis.Y/len),float(u.RotationAxis.Z/len));expectedArgs[expectedAxes]=axis;expectedAngles[expectedAxes++]=speed;expectedChange=axisResult(expectedAxes);}
  else expectedChange.SetIdentity();
  unsigned char fi=(unsigned char)(modelStep(expectedSeed)%256),ft=(unsigned char)(modelStep(expectedSeed)%256);
  check(rng==expectedSeed&&tr.random==expectedRandom&&tr.unit==expectedUnit&&tr.axis==expectedAxes&&tr.matrix==expectedMatrix&&tr.make==expectedMatrix&&tr.normalise==expectedNorm);check(tr.params==(u.SetOrientationFromGame?1:0)&&tr.emitter==(!u.SetOrientationFromGame&&u.SetOrientationFromDirection&&u.StayWithEmitter?1:0));
  for(int i=0;i<expectedAxes;++i)check(vecEq(tr.axes[i],expectedArgs[i])&&closef(tr.angles[i],expectedAngles[i]));
  for(int i=0;i<4;++i)check(vecEq(output.PositionBuffer[i],position));check(vecEq(output.Velocity,velocity)&&vecEq(output.Acceleration,C3DVector(0,0,0)));short life=(short)(tick*u.ParticleLifeSecs);if(life==0)life=1;
  check(output.MaxLife==life&&output.Lifetimer==-1&&output.CreationTime==(unsigned char)roundNear(interpolate*255.0)&&output.Active&&output.ClosestAttractorDistance==FLT_MAX);check(quatEq(output.Orientation,expectedOrientation)&&quatEq(output.OrientationChange,expectedChange)&&closef(output.Angle,expectedAngle)&&output.AngleChange==(signed char)roundNear(float(speed*63.5f))&&output.FlickerIndex==fi&&output.FlickerTimeOffset==ft);++cases;
 }
 check(cases==62208);std::printf(failures?"00b0cad0_TEST FAIL\n":"00b0cad0_TEST PASS\n");return failures?1:0;
}