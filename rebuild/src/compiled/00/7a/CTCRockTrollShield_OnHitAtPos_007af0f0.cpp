// 007af0f0 CTCRockTrollShield::OnHitAtPos
// Leaf: ?OnHitAtPos@CTCRockTrollShield@@QAEXPAVCThing@@ABVC3DVector@@1JAA_N@Z
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseFields
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBaseFields
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CSummonerDef CSummonerDefFields
#include "engine/CSummonerDef.h"
#undef CSummonerDef
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
extern unsigned long GFastOneOverSqrtTable[128];
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)
__forceinline float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-r*r*value)*r*0.5f;}
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
__forceinline const C3DVector&operator*=(float scale){X*=scale;Y*=scale;Z*=scale;return *this;}
__forceinline C3DVector&Normalise(){float factor=GFFastOneOverSqrt(X*X+Y*Y+Z*Z);X*=factor;Y*=factor;Z*=factor;return *this;}
__forceinline const float Dot(const C3DVector&b)const{float r=Y*b.Y;r+=Z*b.Z;r+=X*b.X;return r;}
};class CRightHandedSet{protected:C3DVector Up,Forward;};
class CCRC{public:static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);};
template<class T>class CBasicString{protected:T*PData;unsigned long StringLength;public:const T*PeekData()const{return PData;}unsigned long GetLength()const{return StringLength;}};
class CCharStringData{public:CBasicString<char>Data;};
class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();const char*GetAsCharArray()const{return PStringData?PStringData->Data.PeekData():"";}long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}unsigned long GetCRC()const{return CCRC::Calc(0,GetAsCharArray(),GetLength());}};

class CDefString{private:long TablePos;public:bool operator==(const char*)const;};
class CBaseObjectPointer;
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*Get()const{return reinterpret_cast<T*>(GetPItem());}T*operator->()const{return Get();}};
enum ETCInterfaceType{};class C2DVector;class CTCParamsBase;class CEnginePrimitivePrepareInfo;class CPersistContext;namespace NTCMessages{class CMessageBase;}
// Layout: shared CTCBase prefix; native table omits donor editor-only property transfer pair.
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;
protected:virtual void OnSerialise(CPersistContext&,bool);
public:virtual void OnRemove();
public:virtual void ConstructFromParams(const CTCParamsBase&);
public:virtual void OnCreate();
public:virtual void InitialActivate();
public:virtual void OnInitialActivate();
public:virtual void RenderUpdate(float);
public:virtual bool HasFrameUpdate()const;
public:virtual void FrameUpdate();
public:virtual void BulletTimeFrameUpdate();
public:virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public:virtual void Copy(const CTCBase&);
public:virtual void OnDie();
public:virtual void OnKill();
public:virtual void OnLevelUnload();
public:virtual void OnSetInLimbo(bool);
public:virtual bool Validate(CCharString&)const;
public:virtual bool IsFreeForDeletion()const;
public:virtual bool IsSerialiseCreatable()const;
public:virtual bool IsSerialisableForHeroState()const;
public:virtual ETCInterfaceType GetInterfaceType()const;
public:virtual CCharString GetClassTypeName()const;
public:virtual bool IsComponentSerialisable()const;
public:virtual bool IsDriver()const;
public:virtual bool IsCopyable()const;
public:virtual void GlobalUpdate(bool);
public:virtual void HandleMessage(const NTCMessages::CMessageBase&);
};
// Layout: shared physics base64, actual virtual prefix and unread member storage after12.
class CTCPhysicsBase:public CTCBase{unsigned char unreadPhysics[0x40-0xc];
protected:virtual C2DVector GetOffsetVector()const;
protected:virtual C3DVector GetOffsetVector3D()const;
public:virtual void SetPosition(const C3DVector&);
public:virtual void ResetPosition(const C3DVector&);
public:virtual void ResetRHSet(const CRightHandedSet&);
public:virtual void SetPositionToNavigateTo(const C3DVector&);
public:virtual void SetForcedToMovePos(const C3DVector&,bool);
public:virtual void ResetMovementState();
public:virtual void EnablePhysics(bool);
public:virtual bool IsPhysicsEnabled()const;
public:virtual void EnableGravity(bool);
public:virtual bool IsGravityEnabled()const;
public:virtual void EnableNavigator(bool);
public:virtual bool IsNavigationEnabled()const;
public:virtual bool HasNavigationLayers()const;
public:virtual void SetInteractionFlags(long);
public:virtual void ReturnToDefaultInteractionFlags();
public:virtual void OnDroppedFromUpdateList();
public:virtual void OnAddedToUpdateList();
public:virtual bool IsAlwaysFacingMovementDirection()const;
public:virtual bool IsAffectedByGravity()const;
public:virtual bool IsAffectedByAir()const;
public:virtual bool IsAffectedByWind()const;
public:virtual bool IsAngledWithGround()const;
public:virtual bool IsImmoveable()const;
public:virtual bool IsOnAnySurface()const;
public:virtual bool IsOnAnyStationarySurface()const;
public:virtual bool IsOnMeshSurface()const;
public:virtual bool IsOnGroundSurface()const;
public:virtual bool CanCollideWithThings()const;
public:virtual bool CanCollideWithThingsOneWay()const;
public:virtual bool CanCollideWithLandscape()const;
public:virtual bool CanInteractWithThings()const;
public:virtual bool IsExcludingCreatures()const;
public:virtual bool IsPhysicsMeshImpassable()const;
public:virtual float GetFacingAngleXY()const;
public:virtual float GetFacingAngleYZ()const;
public:virtual float GetFacingAngleXZ()const;
public:virtual void SetFacingAngleXY(float);
public:virtual void SetFacingAngleYZ(float);
public:virtual void SetFacingAngleXZ(float);
public:virtual C3DVector GetFacingVector()const;
public:virtual C3DVector GetFacingVector(float)const;
public:virtual const CRightHandedSet& GetRHSet()const;
public:virtual CRightHandedSet GetRHSet(float)const;
};


#define CThingBaseDef CThingBaseDefFields
#include "engine/_quarantine/CThingBaseDef.h"
#undef CThingBaseDef
#define CSpecialEffectsDef CSpecialEffectsDefFields
#include "engine/_quarantine/CSpecialEffectsDef.h"
#undef CSpecialEffectsDef
// Layout: supplied packed parent39 and supplied native MeshHeight5c; other original definition bytes are unread.
class CThingBaseDef:public CParentDefClassBase{unsigned char unreadThingBase[0x5c-0x39];public:float MeshHeight;};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;};
template<class A,class R>struct unary_function{};template<class A,class B,class R>struct binary_function{};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&x)const{return x;}};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}};
template<class T>class _Rb_global{public:static _Rb_tree_node_base*__cdecl _M_increment(_Rb_tree_node_base*);};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Rb_tree_node<T>*p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<T,_Nonconst_traits<T> >&x){_M_node=x._M_node;}typename Traits::reference operator*()const{return static_cast<_Rb_tree_node<T>*>(_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global<bool>::_M_increment(_M_node);return *this;}};
template<class V,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;};
template<class K,class V,class Select,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned int _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;const_iterator find(const K&)const;const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}const_iterator end()const{return const_iterator(this->_M_header._M_data);}};
template<class Pair>struct _Select1st:unary_function<Pair,typename Pair::first_type>{const typename Pair::first_type&operator()(const Pair&x)const{return x.first;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{private:typedef _Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,C,A>tree_type;tree_type _M_t;public:typedef typename tree_type::const_iterator const_iterator;const_iterator find(const K&k)const{return _M_t.find(k);}const_iterator end()const{return _M_t.end();}};
}
class CSpecialEffectsStringMap{public:typedef std::map<unsigned long,long> TStringCRCMap;public:TStringCRCMap ContainmentMap;};
// Layout: shared original subdef prefix25 and native SpecialEffectsMap at28.
class CSpecialEffectsDef:public CSubDefClassBase{public:CSpecialEffectsStringMap SpecialEffectsMap;bool GetEffectFromString(const CCharString&,long&)const;static CCharString __fastcall StatGetTypeName(){return CCharString("CSpecialEffectsDef");}};
__declspec(noinline)bool CSpecialEffectsDef::GetEffectFromString(const CCharString&name,long&out)const{unsigned long crc=name.GetCRC();CSpecialEffectsStringMap::TStringCRCMap::const_iterator it=SpecialEffectsMap.ContainmentMap.find(crc);if(it==SpecialEffectsMap.ContainmentMap.end())return false;out=(*it).second;return true;}
class CDefinitionManager:public CDefinitionManagerFields{friend class CDefClassBase;protected:CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long);};
template<class T>__declspec(noinline)bool CDefClassBase::GetSubDef(CDefPointer<const T>*out)const{const CSubDefInfo*info=GetPSubDefInfo(T::StatGetTypeName());if(!info)return false;CDefPointer<const T>value(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));out->SetDynamicCast(value);return true;}
class CThing:public CThingFields{public:const C3DVector&GetPos()const;const CThingBaseDef&PeekBaseDef()const{return *reinterpret_cast<const CThingBaseDef*>(PBaseDef_Object);}
 bool HasTC(ETCInterfaceType kind)const{if(reinterpret_cast<const unsigned long*>(UsingComponents)[kind>>5]&(1UL<<(kind&31)))return true;return false;}
};
class CTCDParticleEmitter{public:static CThing*__fastcall Create(long,const C3DVector&,bool);};
// Layout: original CTCBase12 then private health12/rechargeCounter16, both raw TPI and native receiver fields.
class CTCRockTrollShield:public CTCBase{private:long health,rechargeCounter;public:void OnHitAtPos(CThing*,const C3DVector&,const C3DVector&,long,bool&);};
void CTCRockTrollShield::OnHitAtPos(CThing*attacker,const C3DVector&pos,const C3DVector&direction,long damage,bool&blocked){
 if(health==0)return;
 if(!attacker||!attacker->HasTC(static_cast<ETCInterfaceType>(119))){blocked=true;return;}
 const C3DVector&parentPos=ParentThing.GetPos();C3DVector hitPos(pos);hitPos.X-=parentPos.X;hitPos.Y-=parentPos.Y;hitPos.Z-=parentPos.Z;float hitHeightRatio=hitPos.Z/ParentThing.PeekBaseDef().MeshHeight;
 if(hitHeightRatio>0.5f){
  C3DVector toward(direction.X,direction.Y,0.0f);toward.Normalise();toward*=-1.0f;
  if(toward.Dot(ParentThing.PhysicsTC->GetFacingVector())>cos(1.0471976115464443)){
   health+=damage;CDefPointer<const CSpecialEffectsDef>effects;long effect=-1;
   bool found=ParentThing.PeekBaseDef().GetSubDef(&effects)&&effects->GetEffectFromString("GET_HIT_BY_ARROW",effect);
   if(found)CTCDParticleEmitter::Create(effect,pos,false);
  }
 }
 if(health<=0){health=5;rechargeCounter=0;}else blocked=true;
}