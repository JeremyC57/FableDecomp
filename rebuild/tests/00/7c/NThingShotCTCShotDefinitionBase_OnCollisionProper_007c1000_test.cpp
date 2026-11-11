// Controlled dependency fixture; no engine layout claims. Production caller is pasted unchanged below.
#include <cassert>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <utility>
#define FABLE_FASTCALL
class CThing;
class CThingGameObject;
class CThingNoise;
class CWeaponDef;
class CThingMultiArrowShotDef;
enum ETCInterfaceType { TEST_INTERFACE_NONE=0 };
enum ECollideType { COLLIDE_TYPE_NONE=0,COLLIDE_TYPE_THING=1,COLLIDE_TYPE_MAP=3,COLLIDE_TYPE_WATER=4 };
enum ECutscenePauseBehaviour { CUTSCENE_BEHAVIOUR_NOT_PAUSED=0 };
class C3DVector { public:
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& b)const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);}
 C3DVector operator-(const C3DVector& b)const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}
 C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}
 C3DVector operator/(float f)const{return *this*(1.0f/f);}
 C3DVector& operator*=(float f){X*=f;Y*=f;Z*=f;return *this;}
 float Dot(const C3DVector& b)const{return X*b.X+(Y*b.Y+Z*b.Z);}
 C3DVector Cross(const C3DVector& b)const{return C3DVector(Y*b.Z-Z*b.Y,Z*b.X-X*b.Z,X*b.Y-Y*b.X);}
 C3DVector CrossWithUp()const{return C3DVector(Y,-X,0);}
 bool IsNonZero()const{float squared=Y*Y;squared+=X*X;squared+=Z*Z;return squared>0.0f;}
 C3DVector& AccurateNormalise(){float m=(float)std::sqrt(X*X+Y*Y+Z*Z);if(m!=0)*this*=1.0f/m;return *this;}
};
static bool same(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
class CRightHandedSet { public:C3DVector Up,Forward;CRightHandedSet(const C3DVector&u,const C3DVector&f):Up(u),Forward(f){}};
class CCharString {public:std::string Value;CCharString(const char*s=""):Value(s){}bool operator==(const CCharString&x)const{return Value==x.Value;}};
template<class T>class CIntelligentPointer {T*p;public:CIntelligentPointer(T*x=0):p(x){}template<class U>CIntelligentPointer(const CIntelligentPointer<U>&x):p(x.Get()){}T*Get()const{return p;}};
template<class T>class CDefPointer {T*p;public:CDefPointer(T*x=0):p(x){}T*operator->()const{return p;}bool IsNull()const{return p==0;}void Set(T*x){p=x;}};
class CWeaponDef { public:CCharString ProjectileWeaponHelperNameTip,ProjectileWeaponHelperNameBase;CWeaponDef():ProjectileWeaponHelperNameTip("tip"),ProjectileWeaponHelperNameBase("base") {}};
class CThingMultiArrowShotDef { public:long MultiArrowHitEffect;float MultiArrowHitEffectPercentage;CThingMultiArrowShotDef():MultiArrowHitEffect(0),MultiArrowHitEffectPercentage(0) {}};
class CThingBaseDef { public:const CWeaponDef* Weapon;CThingBaseDef():Weapon(0){}bool GetSubDef(CDefPointer<const CWeaponDef>*x)const{x->Set(Weapon);return Weapon!=0;}};
class CThingShotDef { public:
 bool PreventSlidingOnCollision,BounceOnGround,AllowDeflections,RotateOnDeflection,DoesExplosionDamageCaster,UseEndTimeLineOnContinousEffect;
 float Bounciness;long PrimaryEffect,GroundExplosionDef,ExplosionDef,LeavesObjectBehind;const CThingMultiArrowShotDef*Arrow;
 CThingShotDef():PreventSlidingOnCollision(false),BounceOnGround(false),AllowDeflections(false),RotateOnDeflection(false),DoesExplosionDamageCaster(false),UseEndTimeLineOnContinousEffect(false),Bounciness(1),PrimaryEffect(0),GroundExplosionDef(-1),ExplosionDef(-1),LeavesObjectBehind(0),Arrow(0){}
 bool GetSubDef(CDefPointer<const CThingMultiArrowShotDef>*x)const{x->Set(Arrow);return Arrow!=0;}
};
namespace NHitLocations {class CHitLocation { public:};}
class CHitParameters { public:bool Blocked,Negated,Particle;CHitParameters():Blocked(false),Negated(false),Particle(false){}void SetHitNegated(bool x){Negated=x;}void SetCreateParticleEffectOnHit(bool x){Particle=x;}bool IsBlocked()const{return Blocked;}bool IsHitNegated()const{return Negated;}};
namespace NEntityEvents {class CEventShotStruck { public:C3DVector Position;CEventShotStruck(C3DVector&v):Position(v){}};}
class CTCEntityEvents { public:int Count;C3DVector Last;CTCEntityEvents():Count(0){}void AddEvent(NEntityEvents::CEventShotStruck*e){++Count;Last=e->Position;delete e;}};
class CTCPhysicsBase { public:
 C3DVector Velocity,Position,LastPosition,Facing,ResetUp,ResetForward;bool Moving,Gravity;int VelocityCalls,PositionCalls,RHCalls,DefaultCalls,MovingCalls,FaceXZCalls,FaceYZCalls;unsigned Flags;float XZ,YZ;
 CTCPhysicsBase():Facing(1,0,0),Moving(false),Gravity(false),VelocityCalls(0),PositionCalls(0),RHCalls(0),DefaultCalls(0),MovingCalls(0),FaceXZCalls(0),FaceYZCalls(0),Flags(0),XZ(1),YZ(1){}
 void SetVelocity(const C3DVector&v){Velocity=v;++VelocityCalls;SetAsMoving(true);}void SetAsMoving(bool v){Moving=v;++MovingCalls;}
 const C3DVector&GetPosition()const{return Position;}const C3DVector&GetLastPosition()const{return LastPosition;}bool IsMoving()const{return Moving;}
 void ResetPosition(const C3DVector&v){Position=v;++PositionCalls;}void ResetRHSet(const CRightHandedSet&v){ResetUp=v.Up;ResetForward=v.Forward;++RHCalls;}
 void EnableGravity(bool v){Gravity=v;}C3DVector GetFacingVector()const{return Facing;}void SetFacingAngleXZ(float v){XZ=v;++FaceXZCalls;}void SetFacingAngleYZ(float v){YZ=v;++FaceYZCalls;}
 void SetInteractionFlags(unsigned v){Flags=v;}void ReturnToDefaultInteractionFlags(){++DefaultCalls;}
};
class CTCWeaponTrail { public:std::vector<std::pair<C3DVector,C3DVector> > Points;bool Success;int GetCalls,SetCalls;C3DVector Last;CTCWeaponTrail():Success(false),GetCalls(0),SetCalls(0){}bool GetLastProjectileWeaponTrailPoints(std::vector<std::pair<C3DVector,C3DVector> >&p){++GetCalls;p=Points;return Success;}void SetLastProjectileWeaponTrailPoint(const C3DVector&p){Last=p;++SetCalls;}};
namespace NGraphicAppearance {class CTCGraphicAppearance { public:C3DVector Tip,Base;bool Success;int Calls;std::vector<int> Indices;CTCGraphicAppearance():Tip(6,0,0),Base(0,0,0),Success(true),Calls(0){}bool GetHelperPoint(const CCharString&name,int i,int,C3DVector&v,float){++Calls;Indices.push_back(i);v=(i==1||name.Value=="base")?Base:Tip;return Success;}};}
class CTCRotate { public:C3DVector Axis;float Rate,Limit;bool Rotating;int Calls;CTCRotate():Rate(0),Limit(0),Rotating(false),Calls(0){}void SetRotationInfo(const C3DVector&a,float r,float l){Axis=a;Rate=r;Limit=l;++Calls;}void SetRotating(bool x){Rotating=x;}};
class CTCObjectAugmentations { public:long Effect;int Stops;CTCObjectAugmentations():Effect(0),Stops(0){}long GetOnHitExplodingEffect(){return Effect;}void StopTrailEffect(){++Stops;}};
class CTCDeadShotInThingTracker { public:const CThing*Stuck;CTCDeadShotInThingTracker():Stuck(0){}void SetThingStuckIn(const CThing&t){Stuck=&t;}};
class CTCHeroExperience { public:long Total;int Calls;CTCHeroExperience():Total(0),Calls(0){}void AddExperience(long n){Total+=n;++Calls;}};
class CTCDExperienceOrb { public:long Points;int Collected;CThing*Collector;CTCDExperienceOrb():Points(16),Collected(0),Collector(0){}long GetExperiencePoints()const{return Points;}void OnCollected(CThing&t){++Collected;Collector=&t;}};
static std::vector<long> particleIds;
static std::vector<C3DVector> particlePositions;
class CTCDParticleEmitter { public:int Ends;CTCDParticleEmitter():Ends(0){}void EnterTimelineEnd(){++Ends;}static void Create(long n,const C3DVector&p,bool){particleIds.push_back(n);particlePositions.push_back(p);}};
static CThing* explosionResult;
static std::vector<long> explosionIds;
class CTCDExplosion { public:const CThing*Immune;CTCDExplosion():Immune(0){}void SetThingAsImmune(const CThing&t){Immune=&t;}static CThing*Create(long n,const C3DVector&){explosionIds.push_back(n);return explosionResult;}};
class CWorldMap { public:bool InMap;float Ground;mutable int MapCalls,GroundCalls;CWorldMap():InMap(false),Ground(2),MapCalls(0),GroundCalls(0){}int GetMapNumberAt(const C3DVector&)const{++MapCalls;return InMap?1:0;}float GetGroundSizeZAt(const C3DVector&)const{++GroundCalls;return Ground;}};
class CWorld { public:unsigned long Seed;CWorld():Seed(1){}unsigned long*GetWorldSeed(){return &Seed;}};
static CWorldMap testWorldMap;
static CWorld testWorld;
class CMainGameComponent { public:static float GetConstantFPS(){return 32.0f;}};
class CGameDefinitionManager { public:int Calls;CGameDefinitionManager():Calls(0){}static CGameDefinitionManager&Get(){static CGameDefinitionManager m;return m;}long GetDefGlobalIndexFromName(const CCharString&n){assert(n.Value=="SIMPLE_NOISE");++Calls;return 7;}};
class CThing {public:
 bool Dead;unsigned long TypeFlags;CTCPhysicsBase Physics;C3DVector Pos;CThingBaseDef Def;NGraphicAppearance::CTCGraphicAppearance Appearance;void*Components[256];CTCDExperienceOrb*Orb;CTCDExplosion*Explosion;CTCDParticleEmitter*Emitter;CTCRotate Rotate;CTCDeadShotInThingTracker Tracker;std::vector<int>Removed;int Kills,PauseCalls,AddedRotate,AddedTracker;bool Pause;
 CThing():Dead(false),TypeFlags(0),Orb(0),Explosion(0),Emitter(0),Kills(0),PauseCalls(0),AddedRotate(0),AddedTracker(0),Pause(false){for(int i=0;i<256;++i)Components[i]=0;}
 bool IsDead()const{return Dead;}unsigned long GetTypeFlags()const{return TypeFlags;}CTCPhysicsBase*GetPhysicsTC(){return &Physics;}const C3DVector&GetPos()const{return Pos;}const CThingBaseDef&PeekBaseDef()const{return Def;}NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return &Appearance;}
 template<class T>bool GetTC(ETCInterfaceType t,T**p){*p=static_cast<T*>(Components[(int)t]);return *p!=0;}
 bool HasTC(ETCInterfaceType t)const{return Components[(int)t]!=0;}void RemoveTC(ETCInterfaceType t){Removed.push_back((int)t);Components[(int)t]=0;}
 bool GetTCDriver(CTCDExperienceOrb**p){*p=Orb;return *p!=0;}bool GetTCDriver(CTCDExplosion**p){*p=Explosion;return *p!=0;}bool GetTCDriver(CTCDParticleEmitter**p){*p=Emitter;return *p!=0;}
 void*AddTC(const CCharString&s,bool,int){if(s.Value=="CTCRotate"){++AddedRotate;return &Rotate;}assert(s.Value=="CTCDeadShotInThingTracker");++AddedTracker;Components[0x76]=&Tracker;return &Tracker;}
 void SetCutscenePauseBehaviour(ECutscenePauseBehaviour){Pause=true;++PauseCalls;}void Kill(bool){++Kills;Dead=true;}
};
class CThingGameObject:public CThing {public:int HitCalls,ParameterCalls;bool BlockHit,NegateHit,ReceivedNegated,ReceivedParticle;CThingGameObject():HitCalls(0),ParameterCalls(0),BlockHit(false),NegateHit(false),ReceivedNegated(false),ReceivedParticle(false){}void GetShotHitParameters(NHitLocations::CHitLocation*,CThing&,const C3DVector&,CHitParameters&)const{++const_cast<CThingGameObject*>(this)->ParameterCalls;}void OnHit(CHitParameters&p){++HitCalls;ReceivedNegated=p.Negated;ReceivedParticle=p.Particle;if(BlockHit)p.Blocked=true;if(NegateHit)p.Negated=true;}};
static CThingNoise*noiseResult;
static int noiseCalls;
class CThingNoise:public CThing {public:const CThing*Creator;CThingNoise():Creator(0){}static CThingNoise*Create(long n,const C3DVector&,float duration){assert(n==7&&duration==-1.0f);++noiseCalls;return noiseResult;}void SetCreator(const CThing&t){Creator=&t;}};
static bool IS_THING_ALIVE(const CThing*t){return t!=0&&!t->IsDead();}
template<class T>static bool IS_THING_ALIVE(const CIntelligentPointer<T>&p){return IS_THING_ALIVE(p.Get());}
static float randomValue;static int randomCalls;
float FABLE_FASTCALL GFFloatRandom(float maximum,unsigned long*){assert(maximum==1.0f);++randomCalls;return randomValue;}
namespace NThingShot {class CTCShotDefinitionBase {public:
 ECollideType BestCollisionType;CDefPointer<const CThingShotDef>PShotDef;CThing*ParentThing;CIntelligentPointer<CThing>PBestCollisionThing,PShooter,PEffect;C3DVector BestCollisionPos,BestCollisionNormal;NHitLocations::CHitLocation*PBestCollisionHitLocation;std::vector<CIntelligentPointer<const CThing> >HitThingsList;bool MultiArrowShotType,Negated,Particle;CDefPointer<const int>PShotAbilityDef;
 CTCShotDefinitionBase():BestCollisionType(COLLIDE_TYPE_NONE),ParentThing(0),PBestCollisionHitLocation(0),MultiArrowShotType(false),Negated(false),Particle(false){}
 bool IsNegatedHit()const{return Negated;}bool IsCreateParticleEffectOnHit()const{return Particle;}CWorld&GetWorld(){return testWorld;}CWorldMap&GetWorldMap(){return testWorldMap;}const CWorldMap&PeekWorldMap()const{return testWorldMap;}
 void OnCollisionProper(bool&);
};}

void NThingShot::CTCShotDefinitionBase::OnCollisionProper(bool& killed)
{
    if (BestCollisionType == 0)
        return;
    if (PShotDef->PreventSlidingOnCollision) {
        CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
        physics->SetVelocity(C3DVector(0.0f, 0.0f, 0.0f));
    }
    CThing* pthing = PBestCollisionThing.Get();
    ECollideType type = BestCollisionType;
    const C3DVector& collide_pos = BestCollisionPos;
    const C3DVector& collision_normal = BestCollisionNormal;
    NHitLocations::CHitLocation* hitLocation = PBestCollisionHitLocation;
    bool deflected = false;
    CTCEntityEvents* events;
    if (ParentThing->GetTC((ETCInterfaceType)0x42, &events))
        events->AddEvent(new NEntityEvents::CEventShotStruck(BestCollisionPos));
    if (IS_THING_ALIVE(pthing) && (pthing->GetTypeFlags() & 4) && hitLocation) {
        CDefPointer<const CWeaponDef> weaponDef;
        if (!ParentThing->PeekBaseDef().GetSubDef(&weaponDef)) {
            deflected = true;
        } else {
            CThingGameObject* phit_go = static_cast<CThingGameObject*>(pthing);
            CHitParameters params;
            phit_go->GetShotHitParameters(hitLocation, *ParentThing, collide_pos, params);
            params.SetHitNegated(IsNegatedHit());
            params.SetCreateParticleEffectOnHit(IsCreateParticleEffectOnHit());
            phit_go->OnHit(params);
            if (params.IsBlocked() || params.IsHitNegated())
                deflected = true;
        }
    }
    if (IS_THING_ALIVE(PBestCollisionThing)) {
        HitThingsList.push_back(CIntelligentPointer<const CThing>(PBestCollisionThing));
    }
    if (ParentThing->HasTC((ETCInterfaceType)0x2a))
        ParentThing->RemoveTC((ETCInterfaceType)0x2a);
    if (((type == 3 && PShotDef->BounceOnGround) || deflected) && PShotDef->AllowDeflections) {
        ParentThing->SetCutscenePauseBehaviour(CUTSCENE_BEHAVIOUR_NOT_PAUSED);
        CTCWeaponTrail* trail;
        ParentThing->GetTC((ETCInterfaceType)0x4c, &trail);
        std::vector<std::pair<C3DVector, C3DVector> > points;
        C3DVector direction;
        if (trail->GetLastProjectileWeaponTrailPoints(points) && points.size() >= 2) {
            direction = (points.end() - 1)->first - (points.end() - 2)->first;
            trail->SetLastProjectileWeaponTrailPoint(collide_pos);
        } else {
            CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
            direction = physics->GetPosition() - physics->GetLastPosition();
        }
        direction *= PShotDef->Bounciness * 0.5f;
        C3DVector deflect_line = collision_normal * (float)fabs(collision_normal.Dot(direction));
        C3DVector along_line = direction + deflect_line;
        C3DVector outward_line = deflect_line + along_line;
        CThing* thing = ParentThing;
        CThing* shooter = PShooter.Get();
        thing->GetPhysicsTC()->SetVelocity(C3DVector(0.0f, 0.0f, 0.0f));
        C3DVector up(0.0f, 0.0f, 1.0f);
        C3DVector forward = outward_line;
        C3DVector side = outward_line.CrossWithUp();
        if (side.IsNonZero()) {
            up = side.Cross(outward_line);
            up.AccurateNormalise();
        } else {
            forward.Z = 0.0f;
            if (forward.IsNonZero())
                forward.AccurateNormalise();
        }
        if (!(ParentThing->GetTypeFlags() & 8)) {
            thing->GetPhysicsTC()->ResetRHSet(CRightHandedSet(up,forward));
        }
        CDefPointer<const CWeaponDef> weaponDef;
        C3DVector offset;
        if (thing->PeekBaseDef().GetSubDef(&weaponDef)) {
            C3DVector tip;
            if (!thing->GetTCGraphicAppearance()->GetHelperPoint(weaponDef->ProjectileWeaponHelperNameTip, 0, 0, tip, 1.0f))
                tip = thing->GetPos();
            offset = tip - thing->GetPos();
        } else {
            offset = C3DVector(0.0f, 0.0f, 0.0f);
        }
        const C3DVector& position = collide_pos;
        C3DVector new_pos = position - offset;
        thing->GetPhysicsTC()->ResetPosition(new_pos);
        thing->GetPhysicsTC()->SetVelocity(forward);
        thing->GetPhysicsTC()->EnableGravity(true);
        if (PShotDef->RotateOnDeflection) {
            CTCRotate* rotate = (CTCRotate*)thing->AddTC("CTCRotate", false, 0);
            rotate->SetRotationInfo(collision_normal, 1.0f / CMainGameComponent::GetConstantFPS(), 100.0f);
            rotate->SetRotating(true);
        }
        thing->RemoveTC((ETCInterfaceType)0x77);
        if (!(ParentThing->GetTypeFlags() & 8))
            ParentThing->RemoveTC((ETCInterfaceType)0x7f);
        killed = true;
        if (PShotDef->PrimaryEffect)
            CTCDParticleEmitter::Create(PShotDef->PrimaryEffect, position, false);
        if (MultiArrowShotType) {
            CDefPointer<const CThingMultiArrowShotDef> arrowDef;
            PShotDef->GetSubDef(&arrowDef);
            long effect = arrowDef->MultiArrowHitEffect;
            if (effect && GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) < arrowDef->MultiArrowHitEffectPercentage)
                CTCDParticleEmitter::Create(effect, position, false);
        }
        CTCObjectAugmentations* augmentations;
        if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations)) {
            long effect = augmentations->GetOnHitExplodingEffect();
            if (effect)
                CTCDParticleEmitter::Create(effect, position, false);
        }
        return;
    }
    if (PShotDef->GroundExplosionDef >= 0 && type == COLLIDE_TYPE_MAP) {
        CThing* blast = CTCDExplosion::Create(PShotDef->GroundExplosionDef, collide_pos);
        if (!PShotDef->DoesExplosionDamageCaster && blast) {
            CTCDExplosion* driver;
            blast->GetTCDriver(&driver);
            driver->SetThingAsImmune(*PShooter.Get());
        }
    } else if (PShotDef->ExplosionDef >= 0) {
        CThing* blast = CTCDExplosion::Create(PShotDef->ExplosionDef, collide_pos);
        if (!PShotDef->DoesExplosionDamageCaster && blast) {
            CTCDExplosion* driver;
            blast->GetTCDriver(&driver);
            driver->SetThingAsImmune(*PShooter.Get());
        }
    }
    if (!ParentThing->IsDead()) {
        if (PeekWorldMap().GetMapNumberAt(collide_pos)) {
            long noiseDef = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("SIMPLE_NOISE");
            CThingNoise* noise = CThingNoise::Create(noiseDef, collide_pos, -1.0f);
            if (IS_THING_ALIVE(PShooter))
                noise->SetCreator(*PShooter.Get());
        }
        const CThingShotDef* shotDef = PShotDef.operator->();
        bool finishShot = true;
        bool leaveObject = shotDef->LeavesObjectBehind != 0;
        bool stick_in = true;
        bool keepShot = false;
        if (type == 1) {
            if (IS_THING_ALIVE(pthing)) {
                unsigned long flags = pthing->GetTypeFlags();
                CTCHeroExperience* experience;
                if (pthing->HasTC((ETCInterfaceType)0x69) && IS_THING_ALIVE(PShooter) && PShooter.Get()->GetTC((ETCInterfaceType)0x68, &experience)) {
                    CTCDExperienceOrb* orb;
                    if (pthing->GetTCDriver(&orb)) {
                        experience->AddExperience(orb->GetExperiencePoints());
                        orb->OnCollected(*PShooter.Get());
                    }
                }
                if ((flags & 8) || pthing->HasTC((ETCInterfaceType)0x5a) || pthing->GetPhysicsTC()->IsMoving()) {
                    stick_in = false;
                    leaveObject = false;
                }
            } else {
                stick_in = false;
                leaveObject = false;
            }
            if (shotDef->PrimaryEffect)
                CTCDParticleEmitter::Create(shotDef->PrimaryEffect, collide_pos, false);
            if (MultiArrowShotType) {
                CDefPointer<const CThingMultiArrowShotDef> arrowDef;
                PShotDef->GetSubDef(&arrowDef);
                long effect = arrowDef->MultiArrowHitEffect;
                if (effect && GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) < arrowDef->MultiArrowHitEffectPercentage)
                    CTCDParticleEmitter::Create(effect, collide_pos, false);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations)) {
                long effect = augmentations->GetOnHitExplodingEffect();
                if (effect)
                    CTCDParticleEmitter::Create(effect, collide_pos, false);
            }
            if (!PShotAbilityDef.IsNull()) {
                keepShot = true;
                finishShot = false;
            }
            if (stick_in && IS_THING_ALIVE(pthing)) {
                ParentThing->AddTC("CTCDeadShotInThingTracker", false, 0);
                CTCDeadShotInThingTracker* tracker;
                ParentThing->GetTC((ETCInterfaceType)0x76, &tracker);
                tracker->SetThingStuckIn(*pthing);
            }
        } else if (type == 3) {
            if (shotDef->PrimaryEffect)
                CTCDParticleEmitter::Create(shotDef->PrimaryEffect, collide_pos, false);
            if (MultiArrowShotType) {
                CDefPointer<const CThingMultiArrowShotDef> arrowDef;
                PShotDef->GetSubDef(&arrowDef);
                long effect = arrowDef->MultiArrowHitEffect;
                if (effect && GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) < arrowDef->MultiArrowHitEffectPercentage)
                    CTCDParticleEmitter::Create(effect, collide_pos, false);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations)) {
                long effect = augmentations->GetOnHitExplodingEffect();
                if (effect)
                    CTCDParticleEmitter::Create(effect, collide_pos, false);
            }
        } else if (type == 4) {
            leaveObject = false;
        }
        CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
        if (ParentThing->GetTypeFlags() & 8) {
            physics->SetFacingAngleXZ(0.0f);
            physics->SetFacingAngleYZ(0.0f);
            finishShot = false;
        }
        if (finishShot) {
            if (leaveObject) {
                if (IS_THING_ALIVE(PEffect)) {
                    CTCDParticleEmitter* emitter = 0;
                    PEffect.Get()->GetTCDriver(&emitter);
                    if (PShotDef->UseEndTimeLineOnContinousEffect)
                        emitter->EnterTimelineEnd();
                    else
                        PEffect.Get()->Kill(true);
                }
                CTCObjectAugmentations* augmentations;
                if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations))
                    augmentations->StopTrailEffect();
                CDefPointer<const CWeaponDef> weaponDef;
                if (stick_in && ParentThing->PeekBaseDef().GetSubDef(&weaponDef)) {
                    NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->GetTCGraphicAppearance();
                    CCharString tipName(weaponDef->ProjectileWeaponHelperNameTip);
                    CCharString baseName(weaponDef->ProjectileWeaponHelperNameBase);
                    C3DVector tip;
                    C3DVector base;
                    if (tipName == baseName) {
                        appearance->GetHelperPoint(tipName, 0, 0, tip, 1.0f);
                        appearance->GetHelperPoint(baseName, 1, 0, base, 1.0f);
                    } else {
                        appearance->GetHelperPoint(tipName, 0, 0, tip, 1.0f);
                        appearance->GetHelperPoint(baseName, 0, 0, base, 1.0f);
                    }
                    C3DVector position = ParentThing->GetPos();
                    C3DVector stick_in_pos = base + (tip - base) * 2.0f / 3.0f;
                    C3DVector line_offset = stick_in_pos - position;
                    C3DVector new_pos = collide_pos - line_offset;
                    physics->ResetPosition(new_pos);
                    appearance->GetHelperPoint(tipName, 0, 0, tip, 1.0f);
                    appearance->GetHelperPoint(baseName, 0, 0, base, 1.0f);
                } else {
                    C3DVector ground = collide_pos;
                    ground.Z = GetWorldMap().GetGroundSizeZAt(ground);
                    physics->ResetPosition(ground);
                    C3DVector facing = physics->GetFacingVector();
                    facing.Z = 0.0f;
                    if (facing.IsNonZero()) {
                        facing.AccurateNormalise();
                        physics->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), facing));
                    }
                }
                physics->SetAsMoving(false);
                physics->SetInteractionFlags(0x20);
                ParentThing->RemoveTC((ETCInterfaceType)0x77);
                if (!(ParentThing->GetTypeFlags() & 8))
                    ParentThing->RemoveTC((ETCInterfaceType)0x7f);
                killed = true;
            } else {
                if (IS_THING_ALIVE(PEffect)) {
                    CTCDParticleEmitter* emitter = 0;
                    PEffect.Get()->GetTCDriver(&emitter);
                    if (PShotDef->UseEndTimeLineOnContinousEffect)
                        emitter->EnterTimelineEnd();
                    else
                        PEffect.Get()->Kill(true);
                }
                CTCObjectAugmentations* augmentations;
                if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations))
                    augmentations->StopTrailEffect();
                ParentThing->Kill(true);
                killed = true;
            }
        } else if (!keepShot) {
            if (IS_THING_ALIVE(PEffect)) {
                CTCDParticleEmitter* emitter = 0;
                PEffect.Get()->GetTCDriver(&emitter);
                if (PShotDef->UseEndTimeLineOnContinousEffect)
                    emitter->EnterTimelineEnd();
                else
                    PEffect.Get()->Kill(true);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations))
                augmentations->StopTrailEffect();
            physics->SetAsMoving(false);
            physics->EnableGravity(true);
            physics->ReturnToDefaultInteractionFlags();
            ParentThing->RemoveTC((ETCInterfaceType)0x77);
            if (!(ParentThing->GetTypeFlags() & 8))
                ParentThing->RemoveTC((ETCInterfaceType)0x7f);
            killed = true;
        }
    }
    BestCollisionType = COLLIDE_TYPE_NONE;
    PBestCollisionThing = CIntelligentPointer<CThing>(0);
    PBestCollisionHitLocation = 0;
}

struct CollisionFixture {
 NThingShot::CTCShotDefinitionBase Shot;CThing Parent,Shooter,Effect,Blast;CThingGameObject Hit;CThingNoise Noise;CThingShotDef Def;CWeaponDef Weapon;CThingMultiArrowShotDef Arrow;NHitLocations::CHitLocation Location;CTCEntityEvents Events;CTCWeaponTrail Trail;CTCObjectAugmentations Augmentations;CTCHeroExperience Experience;CTCDExperienceOrb Orb;CTCDParticleEmitter Emitter;CTCDExplosion Explosion;int Ability;
 CollisionFixture():Ability(1){testWorldMap=CWorldMap();testWorld=CWorld();particleIds.clear();particlePositions.clear();explosionIds.clear();explosionResult=&Blast;noiseResult=&Noise;noiseCalls=0;randomCalls=0;randomValue=0.25f;CGameDefinitionManager::Get().Calls=0;Blast.Explosion=&Explosion;Effect.Emitter=&Emitter;Def.Arrow=&Arrow;Shot.PShotDef.Set(&Def);Shot.ParentThing=&Parent;Shot.PShooter=CIntelligentPointer<CThing>(&Shooter);Shot.BestCollisionType=COLLIDE_TYPE_MAP;Shot.BestCollisionPos=C3DVector(12,8,4);Shot.BestCollisionNormal=C3DVector(0,0,1);Parent.Components[0x4c]=&Trail;Parent.Physics.Position=C3DVector(2,0,0);}
 bool Run(){bool killed=false;Shot.OnCollisionProper(killed);return killed;}
 void HitThing(){Shot.BestCollisionType=COLLIDE_TYPE_THING;Shot.PBestCollisionThing=CIntelligentPointer<CThing>(&Hit);Shot.PBestCollisionHitLocation=&Location;Hit.TypeFlags=4;}
 void Bounce(){Def.AllowDeflections=true;Def.BounceOnGround=true;}
 bool Removed(int n)const{for(unsigned i=0;i<Parent.Removed.size();++i)if(Parent.Removed[i]==n)return true;return false;}
 void Cleared()const{assert(Shot.BestCollisionType==COLLIDE_TYPE_NONE&&Shot.PBestCollisionThing.Get()==0&&Shot.PBestCollisionHitLocation==0);}
 void UseEffect(){Shot.PEffect=CIntelligentPointer<CThing>(&Effect);Parent.Components[0xbf]=&Augmentations;}
};
int main(){int cases=0;
 {CollisionFixture f;f.Shot.BestCollisionType=COLLIDE_TYPE_NONE;f.Parent.Components[0x42]=&f.Events;bool killed=true;f.Shot.OnCollisionProper(killed);assert(killed&&f.Events.Count==0&&f.Parent.Physics.VelocityCalls==0&&f.Parent.Kills==0);++cases;}
 {CollisionFixture f;f.Parent.Dead=true;assert(!f.Run());f.Cleared();assert(noiseCalls==0&&f.Parent.Kills==0);++cases;}
 {CollisionFixture f;f.Parent.Dead=true;f.Def.PreventSlidingOnCollision=true;f.Parent.Components[0x42]=&f.Events;f.Parent.Physics.Velocity=C3DVector(4,2,1);assert(!f.Run());assert(f.Events.Count==1&&same(f.Events.Last,f.Shot.BestCollisionPos)&&same(f.Parent.Physics.Velocity,C3DVector())&&f.Parent.Physics.VelocityCalls==1);++cases;}
 {CollisionFixture f;f.HitThing();assert(f.Run());assert(f.Hit.HitCalls==0&&f.Shot.HitThingsList.size()==1&&f.Shot.HitThingsList[0].Get()==&f.Hit&&f.Parent.Kills==1);f.Cleared();++cases;}
 {CollisionFixture f;f.HitThing();f.Parent.Def.Weapon=&f.Weapon;f.Shot.Negated=false;f.Shot.Particle=true;assert(f.Run());assert(f.Hit.ParameterCalls==1&&f.Hit.HitCalls==1&&!f.Hit.ReceivedNegated&&f.Hit.ReceivedParticle&&f.Parent.AddedTracker==1&&f.Parent.Tracker.Stuck==&f.Hit);++cases;}
 {CollisionFixture f;f.HitThing();f.Parent.Def.Weapon=&f.Weapon;f.Hit.BlockHit=true;f.Def.AllowDeflections=true;assert(f.Run());assert(f.Hit.HitCalls==1&&f.Parent.Pause&&f.Parent.Kills==0&&f.Shot.BestCollisionType==COLLIDE_TYPE_THING&&f.Parent.Physics.Gravity);++cases;}
 {CollisionFixture f;f.HitThing();f.Parent.Def.Weapon=&f.Weapon;f.Shot.Negated=true;f.Def.AllowDeflections=true;assert(f.Run());assert(f.Hit.ReceivedNegated&&f.Parent.Pause);++cases;}
 {CollisionFixture f;f.Parent.Components[0x2a]=&f.Events;assert(f.Run());assert(f.Removed(0x2a));++cases;}
 {CollisionFixture f;f.HitThing();f.Hit.Dead=true;assert(f.Run());assert(f.Shot.HitThingsList.empty()&&f.Hit.HitCalls==0&&f.Parent.AddedTracker==0);++cases;}
 {CollisionFixture f;f.HitThing();f.Hit.TypeFlags=0;f.Parent.Def.Weapon=&f.Weapon;assert(f.Run());assert(f.Hit.HitCalls==0&&f.Shot.HitThingsList.size()==1);++cases;}
 {CollisionFixture f;f.HitThing();f.Shot.PBestCollisionHitLocation=0;f.Parent.Def.Weapon=&f.Weapon;assert(f.Run());assert(f.Hit.HitCalls==0);++cases;}
 {CollisionFixture f;f.Bounce();assert(f.Run());assert(same(f.Parent.Physics.Velocity,C3DVector(1,0,0))&&same(f.Parent.Physics.Position,f.Shot.BestCollisionPos)&&f.Parent.Physics.RHCalls==1&&same(f.Parent.Physics.ResetUp,C3DVector(0,0,1))&&f.Trail.SetCalls==0&&f.Removed(0x77)&&f.Removed(0x7f));++cases;}
 {CollisionFixture f;f.Bounce();f.Trail.Success=true;f.Trail.Points.push_back(std::make_pair(C3DVector(),C3DVector()));f.Trail.Points.push_back(std::make_pair(C3DVector(4,0,0),C3DVector()));assert(f.Run());assert(same(f.Parent.Physics.Velocity,C3DVector(2,0,0))&&f.Trail.SetCalls==1&&same(f.Trail.Last,f.Shot.BestCollisionPos));++cases;}
 {CollisionFixture f;f.Bounce();f.Trail.Success=true;f.Trail.Points.push_back(std::make_pair(C3DVector(10,0,0),C3DVector()));assert(f.Run());assert(same(f.Parent.Physics.Velocity,C3DVector(1,0,0))&&f.Trail.SetCalls==0);++cases;}
 {CollisionFixture f;f.Bounce();f.Parent.Physics.Position=C3DVector(0,0,-2);assert(f.Run());assert(same(f.Parent.Physics.Velocity,C3DVector())&&f.Parent.Physics.RHCalls==1);++cases;}
 {CollisionFixture f;f.Bounce();f.Parent.TypeFlags=8;assert(f.Run());assert(f.Parent.Physics.RHCalls==0&&!f.Removed(0x7f)&&f.Removed(0x77));++cases;}
 {CollisionFixture f;f.Bounce();f.Parent.Def.Weapon=&f.Weapon;f.Parent.Pos=C3DVector(2,0,0);f.Parent.Appearance.Tip=C3DVector(6,0,0);assert(f.Run());assert(same(f.Parent.Physics.Position,C3DVector(8,8,4))&&f.Parent.Appearance.Calls==1);++cases;}
 {CollisionFixture f;f.Bounce();f.Parent.Def.Weapon=&f.Weapon;f.Parent.Appearance.Success=false;f.Parent.Pos=C3DVector(2,0,0);assert(f.Run());assert(same(f.Parent.Physics.Position,f.Shot.BestCollisionPos));++cases;}
 {CollisionFixture f;f.Bounce();f.Def.RotateOnDeflection=true;assert(f.Run());assert(f.Parent.AddedRotate==1&&f.Parent.Rotate.Rotating&&f.Parent.Rotate.Calls==1&&f.Parent.Rotate.Rate==1.0f/32.0f&&f.Parent.Rotate.Limit==100.0f&&same(f.Parent.Rotate.Axis,f.Shot.BestCollisionNormal));++cases;}
 {CollisionFixture f;f.Bounce();f.Def.PrimaryEffect=10;f.Shot.MultiArrowShotType=true;f.Arrow.MultiArrowHitEffect=20;f.Arrow.MultiArrowHitEffectPercentage=0.5f;f.Parent.Components[0xbf]=&f.Augmentations;f.Augmentations.Effect=30;assert(f.Run());assert(particleIds.size()==3&&particleIds[0]==10&&particleIds[1]==20&&particleIds[2]==30&&randomCalls==1);for(unsigned i=0;i<particlePositions.size();++i)assert(same(particlePositions[i],f.Shot.BestCollisionPos));++cases;}
 {CollisionFixture f;f.Bounce();f.Shot.MultiArrowShotType=true;f.Arrow.MultiArrowHitEffect=20;f.Arrow.MultiArrowHitEffectPercentage=0.25f;assert(f.Run());assert(particleIds.empty()&&randomCalls==1);++cases;}
 {CollisionFixture f;f.Bounce();f.Shot.MultiArrowShotType=true;f.Arrow.MultiArrowHitEffect=0;assert(f.Run());assert(particleIds.empty()&&randomCalls==0);++cases;}
 {CollisionFixture f;f.Def.GroundExplosionDef=4;f.Def.ExplosionDef=5;assert(f.Run());assert(explosionIds.size()==1&&explosionIds[0]==4&&f.Explosion.Immune==&f.Shooter);++cases;}
 {CollisionFixture f;f.HitThing();f.Def.GroundExplosionDef=4;f.Def.ExplosionDef=5;assert(f.Run());assert(explosionIds.size()==1&&explosionIds[0]==5&&f.Explosion.Immune==&f.Shooter);++cases;}
 {CollisionFixture f;f.Def.ExplosionDef=5;f.Def.DoesExplosionDamageCaster=true;assert(f.Run());assert(explosionIds.size()==1&&f.Explosion.Immune==0);++cases;}
 {CollisionFixture f;f.Def.ExplosionDef=5;explosionResult=0;assert(f.Run());assert(explosionIds.size()==1);++cases;}
 {CollisionFixture f;testWorldMap.InMap=true;assert(f.Run());assert(noiseCalls==1&&f.Noise.Creator==&f.Shooter&&CGameDefinitionManager::Get().Calls==1);++cases;}
 {CollisionFixture f;testWorldMap.InMap=true;f.Shooter.Dead=true;assert(f.Run());assert(noiseCalls==1&&f.Noise.Creator==0);++cases;}
 {CollisionFixture f;f.HitThing();f.Hit.Components[0x69]=&f.Orb;f.Hit.Orb=&f.Orb;f.Shooter.Components[0x68]=&f.Experience;assert(f.Run());assert(f.Experience.Total==16&&f.Experience.Calls==1&&f.Orb.Collected==1&&f.Orb.Collector==&f.Shooter);++cases;}
 {CollisionFixture f;f.HitThing();f.Hit.Components[0x69]=&f.Orb;f.Shooter.Components[0x68]=&f.Experience;assert(f.Run());assert(f.Experience.Calls==0&&f.Orb.Collected==0);++cases;}
 {CollisionFixture f;f.HitThing();f.Def.LeavesObjectBehind=1;f.Hit.TypeFlags=8;assert(f.Run());assert(f.Parent.Kills==1&&f.Parent.AddedTracker==0);++cases;}
 {CollisionFixture f;f.HitThing();f.Def.LeavesObjectBehind=1;f.Hit.Components[0x5a]=&f.Events;assert(f.Run());assert(f.Parent.Kills==1&&f.Parent.AddedTracker==0);++cases;}
 {CollisionFixture f;f.HitThing();f.Def.LeavesObjectBehind=1;f.Hit.Physics.Moving=true;assert(f.Run());assert(f.Parent.Kills==1&&f.Parent.AddedTracker==0);++cases;}
 {CollisionFixture f;f.Shot.BestCollisionType=COLLIDE_TYPE_THING;f.Def.LeavesObjectBehind=1;assert(f.Run());assert(f.Parent.Kills==1);++cases;}
 {CollisionFixture f;f.HitThing();f.Shot.PShotAbilityDef.Set(&f.Ability);f.UseEffect();assert(!f.Run());f.Cleared();assert(f.Parent.Kills==0&&f.Effect.Kills==0&&f.Emitter.Ends==0&&f.Augmentations.Stops==0&&f.Parent.AddedTracker==1);++cases;}
 {CollisionFixture f;f.Def.LeavesObjectBehind=1;f.Def.PrimaryEffect=10;f.Shot.MultiArrowShotType=true;f.Arrow.MultiArrowHitEffect=20;f.Arrow.MultiArrowHitEffectPercentage=0.5f;f.Parent.Components[0xbf]=&f.Augmentations;f.Augmentations.Effect=30;assert(f.Run());assert(f.Parent.Kills==0&&particleIds.size()==3&&f.Parent.Physics.Flags==0x20&&!f.Parent.Physics.Moving&&testWorldMap.GroundCalls==1&&same(f.Parent.Physics.Position,C3DVector(12,8,2))&&f.Parent.Physics.RHCalls==1);f.Cleared();++cases;}
 {CollisionFixture f;f.Shot.BestCollisionType=COLLIDE_TYPE_WATER;f.Def.LeavesObjectBehind=1;assert(f.Run());assert(f.Parent.Kills==1&&particleIds.empty());++cases;}
 {CollisionFixture f;f.Def.LeavesObjectBehind=1;f.UseEffect();f.Def.UseEndTimeLineOnContinousEffect=true;assert(f.Run());assert(f.Emitter.Ends==1&&f.Effect.Kills==0&&f.Augmentations.Stops==1&&f.Parent.Kills==0);++cases;}
 {CollisionFixture f;f.UseEffect();assert(f.Run());assert(f.Effect.Kills==1&&f.Emitter.Ends==0&&f.Parent.Kills==1&&f.Augmentations.Stops==1);++cases;}
 {CollisionFixture f;f.HitThing();f.Def.LeavesObjectBehind=1;f.Parent.Def.Weapon=&f.Weapon;f.Parent.Pos=C3DVector(2,0,0);assert(f.Run());assert(same(f.Parent.Physics.Position,C3DVector(10,8,4))&&f.Parent.Appearance.Calls==4&&f.Parent.Appearance.Indices[1]==0&&f.Parent.AddedTracker==1&&f.Parent.Kills==0);++cases;}
 {CollisionFixture f;f.Def.LeavesObjectBehind=1;f.Parent.Def.Weapon=&f.Weapon;f.Weapon.ProjectileWeaponHelperNameBase=CCharString("tip");assert(f.Run());assert(f.Parent.Appearance.Calls==4&&f.Parent.Appearance.Indices[1]==1&&f.Parent.Appearance.Indices[3]==0);++cases;}
 {CollisionFixture f;f.Def.LeavesObjectBehind=1;f.Parent.Physics.Facing=C3DVector(0,0,4);assert(f.Run());assert(f.Parent.Physics.RHCalls==0);++cases;}
 {CollisionFixture f;f.Parent.TypeFlags=8;f.UseEffect();assert(f.Run());assert(f.Parent.Physics.FaceXZCalls==1&&f.Parent.Physics.FaceYZCalls==1&&f.Parent.Physics.XZ==0&&f.Parent.Physics.YZ==0&&f.Parent.Physics.DefaultCalls==1&&f.Parent.Physics.Gravity&&!f.Parent.Physics.Moving&&f.Parent.Kills==0&&f.Effect.Kills==1&&f.Augmentations.Stops==1&&f.Removed(0x77)&&!f.Removed(0x7f));++cases;}
 {CollisionFixture f;f.HitThing();f.Parent.TypeFlags=8;f.Shot.PShotAbilityDef.Set(&f.Ability);f.UseEffect();assert(!f.Run());assert(f.Parent.Physics.FaceXZCalls==1&&f.Parent.Physics.DefaultCalls==0&&f.Effect.Kills==0);++cases;}
 {CollisionFixture f;f.Def.LeavesObjectBehind=1;f.UseEffect();f.Effect.Dead=true;assert(f.Run());assert(f.Effect.Kills==0&&f.Emitter.Ends==0&&f.Augmentations.Stops==1);++cases;}
 {CollisionFixture f;f.HitThing();f.Def.PrimaryEffect=10;f.Shot.MultiArrowShotType=true;f.Arrow.MultiArrowHitEffect=20;f.Arrow.MultiArrowHitEffectPercentage=0.5f;f.Parent.Components[0xbf]=&f.Augmentations;f.Augmentations.Effect=30;assert(f.Run());assert(particleIds.size()==3&&particleIds[0]==10&&particleIds[1]==20&&particleIds[2]==30&&randomCalls==1);++cases;}
 {CollisionFixture f;f.Parent.TypeFlags=8;f.UseEffect();f.Def.UseEndTimeLineOnContinousEffect=true;assert(f.Run());assert(f.Emitter.Ends==1&&f.Effect.Kills==0&&f.Augmentations.Stops==1);++cases;}
 {CollisionFixture f;f.UseEffect();f.Def.UseEndTimeLineOnContinousEffect=true;assert(f.Run());assert(f.Emitter.Ends==1&&f.Parent.Kills==1&&f.Effect.Kills==0);++cases;}
 assert(cases==48);std::puts("007c1000_TEST PASS");return 0;
}