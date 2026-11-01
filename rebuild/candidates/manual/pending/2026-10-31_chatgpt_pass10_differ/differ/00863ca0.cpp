// 00863ca0 CThingBodyReorienter::FillBodyReorientationPossibleIntersecting
// Leaf: ?FillBodyReorientationPossibleIntersecting@CThingBodyReorienter@@AAEXXZ
// Flags:
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CClass CClassFields
#include "engine/CClass.h"
#undef CClass
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
class CCharString;class C3DVector;class C2DVector;class CThing;class CThingPhysical;class CThingCreatureBase;class CThingAICreature;class CThingPlayerCreature;class CThingBuilding;class CTCBase;class CMaterialDef;class CEnvironment;class CEngineGraphic;class CInventoryItem;class CThingDef;class CThingProperties;class CInterpolationInfo;class CEnginePrimitivePrepareInfo;class CPhysicsObject;class CTCParamsBase;class CThingLevelScript;class CThingGameState;class CPhysicsObjectDamage;class CCollisionInfo;class CThingDrawInfo;class CThingDebugDrawInfo;class CThingPhysicalProperties;class CSoundPair;class CAnimationEntry;
template<class T>class CDefPointer;template<class T>class CArray;namespace std{template<class T>class allocator;template<class T,class A>class vector;}
class C3DVector;
class CCharString;
class CCreatureAbilityDef;
class CEditEngineDrawInfo;
class CEngineGraphic;
class CEnginePrimitivePrepareInfo;
class CHitParameters;
class CInterpolationInfo;
class CMaterialDef;
class CMemoryDataInputStream;
class CPersistContext;
class CSoundPair;
class CThing;
class CThingBaseDef;
class CThingCreatureBase;
class CThingDrawInformation;
class CThingInfo;
class CThingPhysical;
class CThingPhysicalDebugInfo;
enum ECollideType{};
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
// Layout: native CClass17-byte flattened header, rounded20 when used as a polymorphic base.
class CClass:public CBaseObject{public:virtual void Save(CCharString&);private:unsigned char _pad_class[sizeof(CClassFields)-sizeof(CBaseObject)];};
class CThing:public CClass{public:class CSerialiseInfo;
public:virtual bool SerialiseGameState(CPersistContext&);
public:virtual bool ConstructFromLevelScript(const char*);
public:virtual bool ConstructAlreadyExistingThingFromLevelScript(const char*);
public:virtual bool ConstructFromGameStateScript(const char*);
public:virtual void CopyGameState(const CThing&);
public:virtual bool OnCreate();
public:virtual void InitialActivate();
public:virtual void OnInitialActivate();
protected:virtual void OnDie();
protected:virtual void OnKill();
protected:virtual bool PreOnKill();
protected:virtual void OnSetInLimbo(bool);
protected:virtual void CreateFromDef(const CThingBaseDef&);
public:virtual void ConstructFromBinarySave(CMemoryDataInputStream&,const CThing::CSerialiseInfo&);
public:virtual bool IsToBePlacedInMap() const;
public:virtual bool ValidateThing(CCharString&) const;
public:virtual bool WorldUpdate();
public:virtual void WorldUpdateBulletTime();
public:virtual bool RenderUpdate(const CInterpolationInfo*);
public:virtual bool IsEntitySerialisable() const;
public:virtual unsigned long GetChecksum() const;
public:virtual void GetInfo(CThingInfo*) const;
public:virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const;
public:virtual void DrawUpdate(const CInterpolationInfo*);
public:virtual bool DrawIsDrawable() const;
public:virtual void SetAsDrawable(bool);
public:virtual bool DrawIsEditDrawable() const;
public:virtual bool DrawIsInDrawList() const;
public:virtual unsigned long DrawGetPhysicsMesh() const;
public:virtual bool DrawIsShowingFacingDirection() const;
public:virtual bool DrawIsDisplayingInformation() const;
public:virtual bool DrawGetThingDisplayInformation(CThingDrawInformation*) const;
public:virtual bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo&) const;
public:virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public:virtual void SetThingStandingOn(CThing*);
public:virtual CThing* GetThingStandingOn();
public:virtual const CThing* GetThingStandingOn() const;
public:virtual long GetFloorMaterial() const;
public:virtual void OnCollision(ECollideType,const C3DVector&,const C3DVector&,CThing*);
public:virtual void OnNonKillingLevelUnload();
public:virtual void OnLevelUnload();
public:virtual bool IsFrameUpdated() const;
public:virtual void SetPositionUsingBrain();
public:template<class T>bool GetTC(T**);CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);const C3DVector&GetPos()const;bool IsDead()const{return (reinterpret_cast<const unsigned char*>(this)[0x91]&1)!=0;}unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingFields*>(this)->ThingTypeFlags;}static unsigned long __fastcall StatGetTypeFlags(){return 0;}
private:unsigned char _pad_members[sizeof(CThingFields)-sizeof(CClass)];
};

#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
class CTCPhysicsBase:public CTCPhysicsBaseFields{protected:using CTCPhysicsBaseFields::Radius;public:float GetRadius()const{return Radius;}};
// Layout: PDB class keys and public geometry fields; native long fields are four bytes.
class C2DCoordI{public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}C2DCoordI(const C2DCoordI&p):X(p.X),Y(p.Y){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI(long a,long b,long c,long d):TLX(a),TLY(b),BRX(c),BRY(d){}C2DBoxI(const C2DCoordI&center,long radius){TLY=center.Y-radius;BRY=center.Y+radius;TLX=center.X-radius;BRX=center.X+radius;}C2DBoxI(const C2DBoxI&b):TLX(b.TLX),TLY(b.TLY),BRX(b.BRX),BRY(b.BRY){}};
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(T*p){SetPItem(static_cast<CBaseObject*>(p));}CIntelligentPointer(const CIntelligentPointer&o){SetPItem(o.GetPItem());}};
extern "C" void __cdecl free(void*);
inline void*operator new(unsigned int,void*p){return p;}
namespace std{
struct __false_type{};
template<class T>class allocator{public:allocator()throw(){}allocator(const allocator&)throw(){}template<class U>allocator(const allocator<U>&)throw(){}~allocator()throw(){}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)free(_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){}T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}bool empty()const{return this->_M_start==this->_M_finish;}void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);
__forceinline void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){if(this->_M_finish)new(this->_M_finish)T(x);++this->_M_finish;}else {__false_type tag;_M_insert_overflow(this->_M_finish,x,tag,1,true);}}};
template<class A,class R>struct unary_function{};
}
class CThingFilter_HasPhysicsMesh:public std::unary_function<CThing,bool>{public:bool operator()(const CThing&)const;};
#define CThingSearchTools CThingSearchToolsFields
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
class CThingSearchTools:public CThingSearchToolsFields{public:template<class T,class V,class F>void GetAllThingsInRoughArea(const C2DBoxI&,const F&,V&)const;};
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
class CWorld:public CWorldFields{public:CThingSearchTools&GetThingSearchTools(){return *PThingSearchTools_ptr;}};
namespace boost{template<class T>class scoped_ptr{private:T*ptr;public:T&operator*()const{return *ptr;}};}
// Layout: original protected scoped world pointer at24; singleton symbol proven in main camera rows.
class CMainGameComponent{protected:static CMainGameComponent*PMainGameComponent;unsigned char unread[0x24];boost::scoped_ptr<CWorld>PWorld;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}CWorld&GetWorld(){return *PWorld;}};
// Layout: raw PDB private reorientation fields; native STLport vector12 puts Body reference at24.
class CThingBodyReorienter{private:bool BodyReorientationDone,ReorientingBody;long ReorientationCount;bool StillCheckingForNewBodyReorientationAngle,HaveFinalReorientationAngle;float FinalReorientationAngleXY;enum ERotationDirection{};ERotationDirection RotationDirection;bool HasPossibleIntersectingThings;std::vector<CIntelligentPointer<CThing> >PossibleIntersectingThings;CThing&Body;void FillBodyReorientationPossibleIntersecting();};
void CThingBodyReorienter::FillBodyReorientationPossibleIntersecting(){
 HasPossibleIntersectingThings=true;StillCheckingForNewBodyReorientationAngle=true;
 float size=reinterpret_cast<CThingFields&>(Body).PhysicsTC->GetRadius()*2.0f;if(size<1.0f)size=1.0f;
 const C3DVector&position=Body.GetPos();C2DCoordI center((long)position.X,(long)position.Y);long half=(long)size;
 C2DBoxI box(center,half);std::vector<CThing*>things;CThingFilter_HasPhysicsMesh filter;
 CMainGameComponent::Get()->GetWorld().GetThingSearchTools().GetAllThingsInRoughArea<CThing>(box,filter,things);
 for(CThing**it=things.begin();it!=things.end();++it)PossibleIntersectingThings.push_back(CIntelligentPointer<CThing>(*it));
 if(PossibleIntersectingThings.empty()){BodyReorientationDone=true;StillCheckingForNewBodyReorientationAngle=false;HasPossibleIntersectingThings=false;}
}
