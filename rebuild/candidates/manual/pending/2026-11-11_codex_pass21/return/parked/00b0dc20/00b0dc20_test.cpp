// Complete Update body; deterministic portable doubles at dependency boundaries.
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <string>
struct C3DVector {float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}C3DVector&operator+=(const C3DVector&v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}C3DVector&operator-=(const C3DVector&v){X-=v.X;Y-=v.Y;Z-=v.Z;return *this;}C3DVector&operator*=(float f){X*=f;Y*=f;Z*=f;return *this;}C3DVector operator^(const C3DVector&v)const{return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X);}bool IsNonZero()const{return X!=0||Y!=0||Z!=0;}void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}C3DVector&AccurateNormalise(){float m=(float)sqrt(X*X+Y*Y+Z*Z);if(m!=0){X/=m;Y/=m;Z/=m;}return *this;}};
static void Equal(const C3DVector&a,const C3DVector&b){assert(a.X==b.X&&a.Y==b.Y&&a.Z==b.Z);}
struct C2DVector{float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
struct CRightHandedSet {C3DVector Up,Forward;int PerpendicularCalls;CRightHandedSet(const C3DVector&u=C3DVector(),const C3DVector&f=C3DVector()):Up(u),Forward(f),PerpendicularCalls(0){}void SetUp(const C3DVector&v){Up=v;}void SetForward(const C3DVector&v){Forward=v;}void MakePerpendicular(){++PerpendicularCalls;}};
struct CMatrix3x4 {int Mode;CMatrix3x4():Mode(0){}void SetFromVectorAndAngle(const C3DVector&axis,float angle){Equal(axis,C3DVector(1,0,0));assert(angle==-0.25f);Mode=1;}void TransformVector(C3DVector&v)const{if(Mode==1){float y=v.Y;v.Y=v.Z;v.Z=-y;}}void TransformPoint(C3DVector&v)const{if(Mode==2)v*= -1.0f;}void Set(const CRightHandedSet&,const C3DVector&t){Equal(t,C3DVector());Mode=3;}};
struct CQuaternion {int Mode;CQuaternion():Mode(0){}void ToRightHandedSet(CRightHandedSet&dest)const{dest.SetUp(C3DVector(0,0,1));dest.SetForward(C3DVector(0,1,0));}void SetIdentity(){Mode=1;}void SetFromAxisAngle(const C3DVector&n,float a){assert(n.IsNonZero()&&a==0.5f);Mode=2;}void ToMatrix(CMatrix3x4&m,const C3DVector&t)const{assert(Mode==2);Equal(t,C3DVector());m.Mode=2;}void SetFromRotationMatrix(const CMatrix3x4&m){assert(m.Mode==3);Mode=3;}};
struct CCharString{std::string text;CCharString(const char*s=""):text(s){}bool IsEmpty()const{return text.empty();}};
template<class T>struct CIntelligentPointer {T*p;CIntelligentPointer(T*x=0):p(x){}T*Get()const{return p;}};
namespace NFractal {struct CPerlinNoise{mutable int Calls;CPerlinNoise():Calls(0){}float GetGaussian2DNoise(const C2DVector&v)const{++Calls;return v.X+v.Y;}};}
namespace NParticleEngine {
struct CParticleEmitter {bool TwoD,Dead;int Updates;CParticleEmitter():TwoD(false),Dead(false),Updates(0){}bool IsEmitter2D()const{return TwoD;}bool IsDead()const{return Dead;}void Update(){++Updates;}};
struct CParticle {C3DVector Position,Velocity,Acceleration,OldOffset;bool Active,Random,RandomAllocationFails;int Lifetimer,AngleUpdates,OrientationUpdates,OrientationSets,RandomCreates;float CreationTime;CQuaternion Orientation,Change;CParticle():Active(true),Random(false),RandomAllocationFails(false),Lifetimer(0),AngleUpdates(0),OrientationUpdates(0),OrientationSets(0),RandomCreates(0),CreationTime(0){}bool IsActive()const{assert(Active);return Active;}const C3DVector&GetVelocity()const{return Velocity;}const C3DVector&GetAcceleration()const{return Acceleration;}const C3DVector&GetPosition()const{return Position;}int GetLifetimer()const{return Lifetimer;}float GetCreationTime()const{return CreationTime;}bool HaveRandomAccelerationData()const{return Random;}void CreateRandomAccelerationData(unsigned long&){++RandomCreates;if(!RandomAllocationFails)Random=true;}unsigned char GetRandomAccelerationTimeOffsetX()const{return 1;}unsigned char GetRandomAccelerationTimeOffsetY()const{return 2;}unsigned char GetRandomAccelerationTimeOffsetZ()const{return 3;}unsigned char GetRandomAccelerationIndexX()const{return 4;}unsigned char GetRandomAccelerationIndexY()const{return 5;}unsigned char GetRandomAccelerationIndexZ()const{return 6;}C3DVector GetOldRandomOffset()const{return OldOffset;}void SetOldRandomOffset(const C3DVector&v){OldOffset=v;}void SetPosition(const C3DVector&v){Position=v;}void SetVelocity(const C3DVector&v){Velocity=v;}void ClearAcceleration(){Acceleration=C3DVector();}void IncrementLifetimer(){++Lifetimer;}void UpdateAngle(){++AngleUpdates;}void UpdateOrientation(){++OrientationUpdates;}const CQuaternion&GetOrientation()const{return Orientation;}void SetOrientation(const CQuaternion&q){Orientation=q;++OrientationSets;}void SetOrientationChange(const CQuaternion&q){Change=q;}};
struct CDecal {bool Landscape,Active;int MaxLife,Lifetimer,Index,Time;C3DVector Position;CRightHandedSet Set;CDecal():Landscape(false),Active(false),MaxLife(0),Lifetimer(-1),Index(-1),Time(-1){}void SetLandscapeOnlyDecal(bool b){Landscape=b;}void SetMaxLife(int n){MaxLife=n;}void SetLifetimer(int n){Lifetimer=n;}void SetPosition(const C3DVector&v){Position=v;}void SetRightHandedSet(const CRightHandedSet&s){Set=s;}void Activate(){Active=true;}void SetFlickerIndex(unsigned char n){Index=n;}void SetFlickerTimeOffset(unsigned char n){Time=n;}};
struct CAEnvironmentInteraction {bool WindActive,Collision,Landscape;int CollisionQueries;C3DVector Gravity,Wind,Contact,Normal,LastPos,LastVelocity;CAEnvironmentInteraction():WindActive(false),Collision(false),Landscape(true),CollisionQueries(0),Contact(8,4,2),Normal(0,0,1){}C3DVector GetGravityVector()const{return Gravity;}bool IsWindActive()const{return WindActive;}C3DVector GetWindVelocity()const{return Wind;}bool IsParticleCollidingAt(const C3DVector&p,const C3DVector&v,bool b,C3DVector&pos,C3DVector&normal,bool&land){assert(!b);++CollisionQueries;LastPos=p;LastVelocity=v;pos=Contact;normal=Normal;land=Landscape;return Collision;}};
struct CParticleEmitterDatabase {int Handle,Lookups;CParticleEmitterDatabase():Handle(7),Lookups(0){}long GetEmitterTemplateHandleFromName(const CCharString&n){assert(!n.IsEmpty());++Lookups;return Handle;}};
struct CParticleEmitterManager {bool Decals,Fail;int Creates;CParticleEmitter Created;NFractal::CPerlinNoise Noise;CParticleEmitterManager():Decals(true),Fail(false),Creates(0){}const NFractal::CPerlinNoise&PeekPerlinNoise()const{return Noise;}bool GetEnableDecals()const{return Decals;}CParticleEmitter*CreateDecalEmitter(long n,const C3DVector&,bool b,const CRightHandedSet&s){assert(n==7&&b);Equal(s.Up,C3DVector(0,0,1));Equal(s.Forward,C3DVector(0,1,0));++Creates;return Fail?0:&Created;}};
static CAEnvironmentInteraction Environment;static CParticleEmitterDatabase Database;static CParticleEmitterManager Manager;static unsigned long Seed;static int RandomCalls;
static float GetOneOverTickRate(){return 0.5f;}static long GetTickRate(){return 2;}static unsigned long&GetRandomSeed(){return Seed;}static CAEnvironmentInteraction&GetEnvironmentInteraction(){return Environment;}static CParticleEmitterDatabase&GetParticleEmitterDatabase(){return Database;}static CParticleEmitterManager&GetParticleEmitterManager(){return Manager;}
static unsigned long GFRandom(unsigned long range,unsigned long*p){assert(range==256&&p==&Seed);++RandomCalls;return (unsigned long)RandomCalls;}
static float GFPower(float a,float b){return (float)pow(a,b);}
template<class K,class V>struct CParticleUpdateParameters {bool Found,OrientFound;C3DVector Accel,Orient;mutable int Reads,OrientReads;CParticleUpdateParameters():Found(false),OrientFound(false),Reads(0),OrientReads(0){}bool GetVectorParam(C3DVector&v,const K&k)const{if(k==1){++Reads;if(Found)v=Accel;return Found;}assert(k==2);++OrientReads;if(OrientFound)v=Orient;return OrientFound;}};
struct CParticleSystem {CParticleEmitter Parent;std::list<CParticle*>Particles;bool Local,FailDecal,TimeEnabled;float Alpha,Time;int Deletes,DecalAllocations;C3DVector EmitterVelocity,Translation;CDecal Decal;CParticleSystem():Local(false),FailDecal(false),TimeEnabled(false),Alpha(1),Time(-1),Deletes(0),DecalAllocations(0),Translation(10,0,0){}const CParticleEmitter&GetParentEmitter()const{return Parent;}std::list<CParticle*>&GetParticleList(){return Particles;}void SetGlobalAlpha(float a){Alpha=a;}void SetNormalisedTime(bool b,float t){TimeEnabled=b;Time=t;}std::list<CParticle*>::iterator DeleteParticle(std::list<CParticle*>::iterator&it){++Deletes;return Particles.erase(it);}void TransformVectorIfLocal(C3DVector&v)const{if(Local)v*=2;}void TransformPosIfLocal(C3DVector&v)const{if(Local)v+=Translation;}void InverseTransformPosIfLocal(C3DVector&v)const{if(Local)v-=Translation;}const C3DVector&PeekEmitterVelocity()const{return EmitterVelocity;}CDecal*AllocateDecal(){++DecalAllocations;return FailDecal?0:&Decal;}};
struct CPSCUpdateNormal {bool UseSystemLifeSecs,SystemAlphaFadeEnable,UseAccelerationParam,UseParticleLifeSecs,RandomisePosEnable,ParticleCollideWithGround,ParticleCreateDecal,ParticleCreateDecalEmitter,ParticleDieOnCollision,SetOrientationFromDirection,SetOrientationFromGame,StayWithEmitter;float SystemLifeSecs,SystemAlphaFadeMinimum,AirResistance,GravityFactor,WindFactor,ParticleAccelerationScale,ParticleLifeSecs,ParticleBounce,Fade;C3DVector ParticleAcceleration,RandomisePosSpeed,RandomisePosScale;unsigned long AccelerationParam,OrientFromGameParam;int LifeTimer;CCharString DecalEmitterName;std::list<CIntelligentPointer<CParticleEmitter> >DecalEmitterList;
CPSCUpdateNormal():UseSystemLifeSecs(false),SystemAlphaFadeEnable(false),UseAccelerationParam(false),UseParticleLifeSecs(false),RandomisePosEnable(false),ParticleCollideWithGround(false),ParticleCreateDecal(false),ParticleCreateDecalEmitter(false),ParticleDieOnCollision(false),SetOrientationFromDirection(false),SetOrientationFromGame(false),StayWithEmitter(false),SystemLifeSecs(4),SystemAlphaFadeMinimum(0),AirResistance(0),GravityFactor(0),WindFactor(0),ParticleAccelerationScale(1),ParticleLifeSecs(2),ParticleBounce(0.5f),Fade(0.5f),RandomisePosScale(1,1,1),AccelerationParam(1),OrientFromGameParam(2),LifeTimer(0){}float GetCurrentFadeValue()const{return Fade;}void Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,bool&);};
}

namespace NParticleEngine {
void CPSCUpdateNormal::Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& keep_alive)
{
    const float timestep = GetOneOverTickRate();
    CAEnvironmentInteraction& environment = GetEnvironmentInteraction();
    float time_secs = LifeTimer * timestep;
    float normalised_time = 0.0f;
    if (UseSystemLifeSecs) {
        if (time_secs >= SystemLifeSecs) {
            keep_alive = false;
            return;
        }
        if (SystemLifeSecs > 0.0f) {
            normalised_time = time_secs / SystemLifeSecs;
            if (normalised_time < 0.0f)
                normalised_time = 0.0f;
            else if (normalised_time > 1.0f)
                normalised_time = 1.0f;
        }
        if (SystemAlphaFadeEnable) {
            float system_alpha = GetCurrentFadeValue() * (1.0f - SystemAlphaFadeMinimum) + SystemAlphaFadeMinimum;
            system.SetGlobalAlpha(system_alpha);
        }
    }
    system.SetNormalisedTime(UseSystemLifeSecs, normalised_time);
    float speed_preserve_factor;
    if (AirResistance > 0.0f)
        speed_preserve_factor = GFPower(1.0f - AirResistance, timestep);
    else
        speed_preserve_factor = 1.0f;
    C3DVector global_acceleration(0.0f, 0.0f, 0.0f);
    if (fabs(GravityFactor) > 0.0f)
        global_acceleration = environment.GetGravityVector() * GravityFactor * timestep;
    if (fabs(WindFactor) > 0.0f && environment.IsWindActive())
        global_acceleration += environment.GetWindVelocity() * WindFactor * timestep;
    if (system.GetParentEmitter().IsEmitter2D()) {
        CMatrix3x4 trans;
        trans.SetFromVectorAndAngle(C3DVector(1.0f, 0.0f, 0.0f), -0.25f);
        trans.TransformVector(global_acceleration);
    }
    C3DVector system_accelecration(0.0f, 0.0f, 0.0f);
    if (!UseAccelerationParam || !params.GetVectorParam(system_accelecration, AccelerationParam))
        system_accelecration = ParticleAcceleration;
    system_accelecration *= ParticleAccelerationScale;
    long max_particle_life = (long)(GetTickRate() * ParticleLifeSecs);
    unsigned long& seed = GetRandomSeed();
    for (std::list<CParticle*>::iterator it_particle = system.GetParticleList().begin();
         it_particle != system.GetParticleList().end();) {
        CParticle& particle = **it_particle;
        if (!particle.IsActive())
            continue;
        if (UseParticleLifeSecs && particle.GetLifetimer() >= max_particle_life) {
            it_particle = system.DeleteParticle(it_particle);
            continue;
        }
        C3DVector local_acceleration = system_accelecration + particle.GetAcceleration();
        C3DVector velocity = particle.GetVelocity();
        local_acceleration *= timestep;
        C3DVector pos = particle.GetPosition();
        C3DVector offset;
        if (RandomisePosEnable) {
            if (!particle.HaveRandomAccelerationData())
                particle.CreateRandomAccelerationData(seed);
            if (particle.HaveRandomAccelerationData()) {
                float factor = 1.0f;
                const NFractal::CPerlinNoise& perlin_noise = GetParticleEmitterManager().PeekPerlinNoise();
                C2DVector noise_lookup(time_secs * RandomisePosSpeed.X + particle.GetRandomAccelerationTimeOffsetX(), particle.GetRandomAccelerationIndexX());
                offset.X = factor * RandomisePosScale.X * perlin_noise.GetGaussian2DNoise(noise_lookup);
                noise_lookup = C2DVector(time_secs * RandomisePosSpeed.Y + particle.GetRandomAccelerationTimeOffsetY(), particle.GetRandomAccelerationIndexY());
                offset.Y = factor * RandomisePosScale.Y * perlin_noise.GetGaussian2DNoise(noise_lookup);
                noise_lookup = C2DVector(time_secs * RandomisePosSpeed.Z + particle.GetRandomAccelerationTimeOffsetZ(), particle.GetRandomAccelerationIndexZ());
                offset.Z = factor * RandomisePosScale.Z * perlin_noise.GetGaussian2DNoise(noise_lookup);
                local_acceleration -= particle.GetOldRandomOffset();
                local_acceleration += offset;
            }
        }
        system.TransformVectorIfLocal(local_acceleration);
        C3DVector acceleration = local_acceleration + global_acceleration;
        if (particle.GetLifetimer() >= 0) {
            velocity += acceleration;
            pos += velocity;
            velocity *= speed_preserve_factor;
        } else {
            float fraction = 1.0f - particle.GetCreationTime();
            velocity += acceleration * fraction;
            pos += velocity * fraction;
            offset *= fraction;
            velocity *= GFPower(speed_preserve_factor, fraction);
        }
        if (RandomisePosEnable && particle.HaveRandomAccelerationData())
            particle.SetOldRandomOffset(offset);
        C3DVector collide_pos;
        C3DVector collide_normal;
        C3DVector test_pos = particle.GetPosition();
        system.TransformPosIfLocal(test_pos);
        bool landscape_collision = true;
        if (ParticleCollideWithGround && environment.IsParticleCollidingAt(test_pos, velocity, false, collide_pos, collide_normal, landscape_collision)) {
            if (GetParticleEmitterManager().GetEnableDecals() && ParticleCreateDecal) {
                CDecal* decal = system.AllocateDecal();
                if (decal != 0) {
                    decal->SetLandscapeOnlyDecal(landscape_collision);
                    decal->SetMaxLife(-1);
                    decal->SetLifetimer(0);
                    decal->SetPosition(collide_pos);
                    CRightHandedSet rhset;
                    rhset.SetUp(collide_normal);
                    if (landscape_collision) {
                        C3DVector cross_vec(0.0f,0.0f,1.0f);
                    if (fabs(collide_normal.Z)>0.99f) {
                        if (fabs(collide_normal.X)>=fabs(collide_normal.Y)) cross_vec.Initialise(0.0f,1.0f,0.0f);
                        else cross_vec.Initialise(1.0f,0.0f,0.0f);
                    }
                    rhset.SetForward((collide_normal ^ cross_vec) ^ collide_normal);
                        rhset.MakePerpendicular();
                    }
                    decal->SetRightHandedSet(rhset);
                    decal->Activate();
                    decal->SetFlickerIndex(GFRandom(256, &seed));
                    decal->SetFlickerTimeOffset(GFRandom(256, &seed));
                }
            }
            if (ParticleCreateDecalEmitter && !DecalEmitterName.IsEmpty()) {
                long type = GetParticleEmitterDatabase().GetEmitterTemplateHandleFromName(DecalEmitterName);
                if (type != 0) {
                    CParticleEmitter* pemitter = GetParticleEmitterManager().CreateDecalEmitter(type, collide_pos, true,
                        CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
                    if (pemitter != 0)
                        DecalEmitterList.push_back(pemitter);
                }
            }
            if (ParticleDieOnCollision) {
                it_particle = system.DeleteParticle(it_particle);
                continue;
            }
            system.InverseTransformPosIfLocal(collide_pos);
            pos = collide_pos;
            if (landscape_collision) {
                velocity.X *= ParticleBounce;
                velocity.Y *= ParticleBounce;
                velocity.Z = -(velocity.Z * ParticleBounce);
            } else {
                CQuaternion rot;
                rot.SetFromAxisAngle(collide_normal, 0.5f);
                CMatrix3x4 rot_mat;
                rot.ToMatrix(rot_mat, C3DVector(0.0f, 0.0f, 0.0f));
                rot_mat.TransformPoint(velocity);
                velocity = velocity * ParticleBounce;
            }
        }
        if (SetOrientationFromDirection || SetOrientationFromGame) {
            C3DVector movement_direction(0.0f, 0.0f, 0.0f);
            if (SetOrientationFromGame)
                params.GetVectorParam(movement_direction, OrientFromGameParam);
            else {
                if (velocity.IsNonZero())
                    movement_direction = velocity;
                if (StayWithEmitter)
                    movement_direction += system.PeekEmitterVelocity();
            }
            if (movement_direction.IsNonZero()) {
                movement_direction.AccurateNormalise();
                CRightHandedSet old_rhs;
                particle.GetOrientation().ToRightHandedSet(old_rhs);
                CRightHandedSet rhs;
                rhs.SetForward(movement_direction.AccurateNormalise());
                C3DVector cross_vec(0.0f,0.0f,1.0f);
                    if (fabs(movement_direction.Z)>0.99f) {
                        if (fabs(movement_direction.X)>fabs(movement_direction.Y)) cross_vec.Initialise(0.0f,1.0f,0.0f);
                        else cross_vec.Initialise(1.0f,0.0f,0.0f);
                    }
                    rhs.SetUp((movement_direction ^ cross_vec) ^ movement_direction);
                rhs.MakePerpendicular();
                CMatrix3x4 rot_mat;
                rot_mat.Set(rhs, C3DVector(0.0f, 0.0f, 0.0f));
                CQuaternion orient;
                orient.SetFromRotationMatrix(rot_mat);
                particle.SetOrientation(orient);
                CQuaternion id;
                id.SetIdentity();
                particle.SetOrientationChange(id);
            }
        } else {
            particle.UpdateOrientation();
        }
        particle.UpdateAngle();
        particle.SetPosition(pos);
        particle.SetVelocity(velocity);
        particle.ClearAcceleration();
        particle.IncrementLifetimer();
        ++it_particle;
        keep_alive = true;
    }
    for (std::list<CIntelligentPointer<CParticleEmitter> >::iterator emitterIterator = DecalEmitterList.begin();
         emitterIterator != DecalEmitterList.end();) {
        if ((*emitterIterator).Get() != 0 && !(*emitterIterator).Get()->IsDead()) {
            (*emitterIterator).Get()->Update();
            ++emitterIterator;
        } else
            emitterIterator = DecalEmitterList.erase(emitterIterator);
    }
    ++LifeTimer;
}

}
using namespace NParticleEngine;
static void Reset(){Environment=CAEnvironmentInteraction();Database=CParticleEmitterDatabase();Manager=CParticleEmitterManager();RandomCalls=0;Seed=0;}
int main(){
// Ordinary and first-fraction integration; drag and two gravity coordinate modes.
for(int mode=0;mode<6;++mode){Reset();CPSCUpdateNormal u;CParticleSystem s;CParticle p;CParticleUpdateParameters<unsigned long,unsigned long>a;bool keep=false;s.Particles.push_back(&p);p.Velocity=C3DVector(2,0,0);p.Acceleration=C3DVector(2,0,0);u.ParticleAcceleration=C3DVector(2,0,0);if(mode==1){p.Lifetimer=-1;p.CreationTime=0.5f;}if(mode==2)u.AirResistance=0.75f;if(mode==3){u.GravityFactor=1;Environment.Gravity=C3DVector(0,0,-2);}if(mode==4){u.GravityFactor=1;Environment.Gravity=C3DVector(0,0,-2);s.Parent.TwoD=true;}if(mode==5)s.Local=true;u.Update(s,a,keep);C3DVector velocity(4,0,0),pos(4,0,0);if(mode==1){velocity=C3DVector(3,0,0);pos=C3DVector(1.5f,0,0);}if(mode==2)velocity=C3DVector(2,0,0);if(mode==3){velocity.Z=-1;pos.Z=-1;}if(mode==4){velocity.Y=-1;pos.Y=-1;}if(mode==5){velocity.X=6;pos.X=6;}Equal(p.Velocity,velocity);Equal(p.Position,pos);Equal(p.Acceleration,C3DVector());assert(keep&&u.LifeTimer==1&&p.AngleUpdates==1&&p.OrientationUpdates==1&&p.Lifetimer==(mode==1?0:1));}
// Parameter fallback and wind enabled/disabled.
for(int mode=0;mode<4;++mode){Reset();CPSCUpdateNormal u;CParticleSystem s;CParticle p;CParticleUpdateParameters<unsigned long,unsigned long>a;bool keep=false;s.Particles.push_back(&p);u.UseAccelerationParam=true;u.ParticleAcceleration=C3DVector(2,0,0);a.Found=(mode&1)!=0;a.Accel=C3DVector(4,0,0);u.WindFactor=2;Environment.Wind=C3DVector(0,2,0);Environment.WindActive=(mode&2)!=0;u.Update(s,a,keep);Equal(p.Velocity,C3DVector(a.Found?2.0f:1.0f,Environment.WindActive?2.0f:0.0f,0));assert(a.Reads==1);}
// Lifetime early exit, normalised time and alpha, zero-duration system.
for(int mode=0;mode<3;++mode){Reset();CPSCUpdateNormal u;CParticleSystem s;CParticle p;CParticleUpdateParameters<unsigned long,unsigned long>a;bool keep=true;s.Particles.push_back(&p);u.UseSystemLifeSecs=true;u.SystemAlphaFadeEnable=true;u.SystemAlphaFadeMinimum=0.5f;u.LifeTimer=mode==0?8:2;if(mode==2)u.SystemLifeSecs=0;u.Update(s,a,keep);if(mode!=1){assert(!keep&&p.AngleUpdates==0&&s.Time==-1&&u.LifeTimer==(mode==0?8:2));}else assert(keep&&s.TimeEnabled&&s.Time==0.25f&&s.Alpha==0.75f&&u.LifeTimer==3);}
// Erase returns the next iterator and leaves caller's keep-alive flag unchanged.
{Reset();CPSCUpdateNormal u;CParticleSystem s;CParticle p,q;CParticleUpdateParameters<unsigned long,unsigned long>a;bool keep=false;u.UseParticleLifeSecs=true;p.Lifetimer=4;s.Particles.push_back(&p);s.Particles.push_back(&q);u.Update(s,a,keep);assert(s.Deletes==1&&s.Particles.size()==1&&keep&&p.AngleUpdates==0&&q.Lifetimer==1);}
for(int mode=0;mode<8;++mode){Reset();CPSCUpdateNormal u;CParticleSystem s;CParticle p;CParticleUpdateParameters<unsigned long,unsigned long>a;bool keep=false;s.Particles.push_back(&p);p.Velocity=C3DVector(2,4,6);u.ParticleCollideWithGround=true;u.ParticleCreateDecal=true;u.ParticleCreateDecalEmitter=true;u.DecalEmitterName=CCharString("decal");Environment.Collision=true;if(mode==1)Environment.Landscape=false;if(mode==2)u.ParticleDieOnCollision=true;if(mode==3){s.FailDecal=true;Manager.Fail=true;}if(mode==4)Manager.Decals=false;if(mode==5)Database.Handle=0;if(mode==6)u.DecalEmitterName=CCharString();if(mode==7)s.Local=true;u.Update(s,a,keep);assert(Environment.CollisionQueries==1);Equal(Environment.LastPos,mode==7?C3DVector(10,0,0):C3DVector());if(mode==2){assert(s.Deletes==1&&!keep&&p.Lifetimer==0);}else{assert(keep);Equal(p.Position,mode==7?C3DVector(-2,4,2):C3DVector(8,4,2));Equal(p.Velocity,mode==1?C3DVector(-1,-2,-3):C3DVector(1,2,-3));}if(mode!=3&&mode!=4){assert(s.Decal.Active&&s.Decal.Landscape==Environment.Landscape&&s.Decal.MaxLife==-1&&s.Decal.Lifetimer==0&&RandomCalls==2&&s.Decal.Index==1&&s.Decal.Time==2);}else assert(!s.Decal.Active&&RandomCalls==0);assert(Manager.Creates==((mode==5||mode==6)?0:1));assert(Manager.Created.Updates==((mode==3||mode==5||mode==6)?0:1));}
// Noise allocation, reuse and failed allocation preserve update behavior.
for(int mode=0;mode<3;++mode){Reset();CPSCUpdateNormal u;CParticleSystem s;CParticle p;CParticleUpdateParameters<unsigned long,unsigned long>a;bool keep=false;s.Particles.push_back(&p);u.RandomisePosEnable=true;p.Random=mode==1;p.RandomAllocationFails=mode==2;p.OldOffset=C3DVector(1,2,3);u.Update(s,a,keep);assert(p.RandomCreates==(mode==1?0:1));if(mode!=2){Equal(p.Velocity,C3DVector(4,5,6));Equal(p.OldOffset,C3DVector(5,7,9));assert(Manager.Noise.Calls==3);}else{Equal(p.Velocity,C3DVector());assert(Manager.Noise.Calls==0);}}
// Direction/game orientation plus empty, live, dead decal-emitter traversal.
for(int mode=0;mode<4;++mode){Reset();CPSCUpdateNormal u;CParticleSystem s;CParticle p;CParticleEmitter live,dead;dead.Dead=true;CParticleUpdateParameters<unsigned long,unsigned long>a;bool keep=false;s.Particles.push_back(&p);u.SetOrientationFromDirection=true;if(mode==0)p.Velocity=C3DVector(1,0,0);if(mode==1){u.SetOrientationFromGame=true;a.OrientFound=true;a.Orient=C3DVector(0,1,0);}if(mode==2){u.StayWithEmitter=true;s.EmitterVelocity=C3DVector(0,0,1);}u.DecalEmitterList.push_back(CIntelligentPointer<CParticleEmitter>());u.DecalEmitterList.push_back(CIntelligentPointer<CParticleEmitter>(&live));u.DecalEmitterList.push_back(CIntelligentPointer<CParticleEmitter>(&dead));u.Update(s,a,keep);assert(p.OrientationUpdates==0&&p.OrientationSets==(mode==3?0:1));if(mode!=3)assert(p.Orientation.Mode==3&&p.Change.Mode==1);assert(u.DecalEmitterList.size()==1&&live.Updates==1&&dead.Updates==0);}
puts("00b0dc20_TEST PASS");return 0;}
