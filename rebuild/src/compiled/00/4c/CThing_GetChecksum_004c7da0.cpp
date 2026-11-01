// 004c7da0 CThing::GetChecksum
// Leaf: ?GetChecksum@CThing@@UBEKXZ
// Flags:
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
class CCharStringData;
class CCharString {protected:CCharStringData* PStringData;public:CCharString();CCharString(const CCharString&);~CCharString();};
#ifdef _MSC_VER
#define FABLE_STLP_CDECL __cdecl
#else
#define FABLE_STLP_CDECL
#endif
extern "C" int FABLE_STLP_CDECL puts(const char*);
extern "C" void FABLE_STLP_CDECL abort(void);
namespace std {
inline void FABLE_STLP_CDECL __stl_throw_out_of_range(const char*msg){puts(msg);abort();}
template<class T>class allocator {};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class A,class B>struct pair {typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool> {bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Select1st:public unary_function<T,typename T::first_type> {const typename T::first_type&operator()(const T&x)const{return x.first;}};
template<class T>struct _Identity:public unary_function<T,T> {const T&operator()(const T&x)const{return x;}};
struct _Rb_tree_node_base {bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base {T _M_value_field;};
template<class T,class A>struct _Rb_tree_base {protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};
template<class K,class V,class Select,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A> {protected:unsigned int _M_node_count;Compare _M_key_compare;};
template<class K,class Compare=less<K>,class A=allocator<K> >class set {private:_Rb_tree<K,K,_Identity<K>,Compare,A>_M_t;};
template<class T,class A>class _Vector_base {protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:typedef unsigned int size_type;typedef T*iterator;typedef const T*const_iterator;iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator begin()const{return this->_M_start;}const_iterator end()const{return this->_M_finish;}size_type size()const{return size_type(this->_M_finish-this->_M_start);}T&operator[](size_type n){return *(begin()+n);}const T&operator[](size_type n)const{return *(begin()+n);}};
struct _List_node_base {_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base {T _M_data;};
struct _List_iterator_base {_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}void _M_incr(){_M_node=_M_node->_M_next;}bool operator!=(const _List_iterator_base&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits {typedef T value_type;typedef T*pointer;typedef T&reference;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base {typedef _List_iterator<T,_Nonconst_traits<T> >iterator;_List_iterator(_List_node<T>*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&x):_List_iterator_base(x._M_node){}typename Traits::reference operator*()const{return static_cast<_List_node<T>*>(this->_M_node)->_M_data;}_List_iterator&operator++(){this->_M_incr();return *this;}};
template<class T,class A>class _List_base {public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A> {public:typedef _List_iterator<T,_Nonconst_traits<T> >iterator;iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}};
template<unsigned int N>struct _Base_bitset {unsigned long _M_w[N];};
template<unsigned int N>class bitset:public _Base_bitset<(N+31)/32> {public:bool test(unsigned int pos)const{if(pos>=N)__stl_throw_out_of_range("bitset");return (this->_M_w[pos/32]&(1UL<<(pos%32)))!=0;}};
}
template<class T>class CArray:public std::vector<T> {};
template<class K,class V>class CKeyPairCompareLess {};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {public:CompareType Compare;bool Dirty;};
#ifdef _MSC_VER
typedef unsigned __int64 ThingIdentifier;
#else
typedef unsigned int ThingIdentifier __attribute__((mode(DI),aligned(8)));
#endif
class CClass:public CBaseObject {public:std::_List_iterator<CClass*,std::_Nonconst_traits<CClass*> >ClassListPos,TypeListPos;unsigned char Type;virtual void Save(CCharString&);};
// Layout: shared CClass flattens its vptr; preserve the typed inheritance and Type at +0x10.
FABLE_STATIC_ASSERT(offsetof(CClass,Type)==offsetof(CClassLayout,Type));
class CThingBaseDef;class CTCBase;class CTCDriverBase;class CTCPhysicsBase;
namespace NGraphicAppearance {class CTCGraphicAppearance;}
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
enum ETCInterfaceType {};
class CPersistContext;class CMemoryDataInputStream;class CInterpolationInfo;
class CThingInfo;class CEditEngineDrawInfo;class CEnginePrimitivePrepareInfo;
class CThingDrawInformation;class CThingPhysicalDebugInfo;enum ECollideType {};
class C3DVector{public:float X,Y,Z;};
class CThingManager;
class CThing:public CClass {
private:static CThingManager*PCurrentThingManager;
public:
 class CSerialiseInfo;
 virtual void Save(CCharString&);
 virtual bool SerialiseGameState(CPersistContext&);
 virtual bool ConstructFromLevelScript(const char*);
 virtual bool ConstructAlreadyExistingThingFromLevelScript(const char*);
 virtual bool ConstructFromGameStateScript(const char*);
 virtual void CopyGameState(const CThing&);
 virtual bool OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();
protected:
 virtual void OnDie();virtual void OnKill();virtual bool PreOnKill();virtual void OnSetInLimbo(bool);
 virtual void CreateFromDef(const CThingBaseDef&);
public:
 virtual void ConstructFromBinarySave(CMemoryDataInputStream&,const CSerialiseInfo&);
 virtual bool IsToBePlacedInMap()const;virtual bool ValidateThing(CCharString&)const;
 virtual bool WorldUpdate();virtual void WorldUpdateBulletTime();virtual bool RenderUpdate(const CInterpolationInfo*);
 virtual bool IsEntitySerialisable()const;virtual unsigned long GetChecksum()const;bool IsLocal()const;
 virtual void GetInfo(CThingInfo*)const;virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*)const;
 virtual void DrawUpdate(const CInterpolationInfo*);virtual bool DrawIsDrawable()const;virtual void SetAsDrawable(bool);
 virtual bool DrawIsEditDrawable()const;virtual bool DrawIsInDrawList()const;virtual unsigned long DrawGetPhysicsMesh()const;
 virtual bool DrawIsShowingFacingDirection()const;virtual bool DrawIsDisplayingInformation()const;
 virtual bool DrawGetThingDisplayInformation(CThingDrawInformation*)const;virtual bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo&)const;
 virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
 virtual void SetThingStandingOn(CThing*);virtual const CThing*GetThingStandingOn()const;virtual CThing*GetThingStandingOn();
 virtual long GetFloorMaterial()const;virtual void OnCollision(ECollideType,const C3DVector&,const C3DVector&,CThing*);
 virtual void OnNonKillingLevelUnload();virtual void OnLevelUnload();virtual bool IsFrameUpdated()const;
 virtual void SetPositionUsingBrain();
protected:virtual~CThing();
public:
 ThingIdentifier UniqueID;std::bitset<274>UsingComponents;CVectorMap<ETCInterfaceType,CTCBase*>Components;std::list<CTCBase*>UpdateComponentsList;CCharString SerialisationSectionName;
 CTCDriverBase*PTCDriver;CTCPhysicsBase*PhysicsTC;NGraphicAppearance::CTCGraphicAppearance*PTCGraphicAppearance;long WorldFrameCreatedAt;unsigned long ThingTypeFlags;CDefPointer<const CThingBaseDef>PBaseDef;CCharString ScriptName,ScriptData;
 CThing*RenderListPrev,*RenderListNext,*UpdateListPrev,*UpdateListNext;unsigned short DefGlobalIndex;short CurrentMapNumber;signed char PlayerNumber;
 unsigned char Dead:1,CreatedByOwnerThingCreation:1,IncludeInThingScans:1,Activated:1,GloballyLoaded:1,InLimbo:1,KillLocked:1,CreatedBySerialisation:1;
 unsigned char DeletedOnOwnerLevelOutOfScope:1,Drawable:1,Dying:1,ForceFrameUpdate:1,LevelPersistent:1,GameStatePersisted:1,PersistOnUnloadWhenInLimbo:1,PausedDuringCutscenes:1;
 unsigned char NotPausedDuringCutscenes:1,DeathPersistent:1,KillOnLevelUnload:1,ActivatedScript:1,LevelIsUnloading:1,IsAffectedByBulletTimeOverride:1,AddingTCsPermitted:1;
 unsigned char InitialisationStage:3;
};
// Layout: the imported CThing header flattens the vptr, so retain the original polymorphic declaration here.
#define CThing CThingLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
FABLE_STATIC_ASSERT(sizeof(CThing)==sizeof(CThingLayout));
FABLE_STATIC_ASSERT(offsetof(CThing,PhysicsTC)==offsetof(CThingLayout,PhysicsTC));
FABLE_STATIC_ASSERT(offsetof(CThing,DefGlobalIndex)==offsetof(CThingLayout,DefGlobalIndex));
FABLE_STATIC_ASSERT(offsetof(CThing,PlayerNumber)==offsetof(CThingLayout,PlayerNumber));

#include "engine/CBaseClassNonCopyable.h"
class CTCParamsBase;
namespace NTCMessages{class CMessageBase;}
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
class CTCBase:public CBaseClassNonCopyable{
protected:virtual void OnSerialise(CPersistContext&,bool);
public:
 virtual~CTCBase();virtual void OnRemove();virtual void ConstructFromParams(const CTCParamsBase&);
 virtual void OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();virtual void RenderUpdate(float);
 virtual bool HasFrameUpdate()const;virtual void FrameUpdate();virtual void BulletTimeFrameUpdate();
 virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);virtual void Copy(const CTCBase&);
 virtual void OnDie();virtual void OnKill();virtual void OnLevelUnload();virtual void OnSetInLimbo(bool);
 virtual bool Validate(CCharString&)const;virtual bool IsFreeForDeletion()const;virtual bool IsSerialiseCreatable()const;
 virtual bool IsSerialisableForHeroState()const;virtual ETCInterfaceType GetInterfaceType()const=0;
 virtual CCharString GetClassTypeName()const=0;virtual bool IsComponentSerialisable()const;
 virtual bool IsDriver()const;virtual bool IsCopyable()const;virtual void GlobalUpdate(bool);
 virtual void HandleMessage(const NTCMessages::CMessageBase&);
protected:CThing&ParentThing;
private:bool ToBeRemoved,IsInGlobalUpdateTCs;
};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseFields));
class CTCDriverBase:public CTCBase{public:
 virtual bool IsDriver()const;virtual bool IsDriverType(long)const=0;virtual void CreateAdditionalTCs();
};


class CPhysicsDef;
class CTCPhysicsBase:public CTCBase{public:
 virtual~CTCPhysicsBase();
 C3DVector Position,OldPosition;long OldPositionLastFrameSet;C3DVector Velocity;float Radius;CDefPointer<const CPhysicsDef>PhysicsDef;bool Moving:1,OldMoving:1;
 const C3DVector&GetPosition()const{return Position;}const C3DVector&GetVelocity()const{return Velocity;}
};
#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase)==sizeof(CTCPhysicsBaseFields));
class CClassTypeInfo{public:std::list<CClass*>TypeList;unsigned long NoAllocated;bool Used,Local;CClass*(__fastcall*AllocFunc)();CCharString Name;};
class CClass;class CClassTypeInit;
class CClassFactory:public CBaseClassNonCopyable {
protected:
 CClassFactory(unsigned char);
 virtual ~CClassFactory();
 virtual void AddNewClassType(const CClassTypeInit&);
 virtual void OnCreateClass(CClass&);
 virtual void OnDeleteClass(CClass&);
public:
 std::vector<CClassTypeInfo>ClassTypeInfo;
 unsigned long MaxClassTypeID,NoAllocatedClasses;
 std::list<CClass*>ClassList;
};

#define CClassFactory CClassFactoryFields
#include "engine/CClassFactory.h"
#undef CClassFactory
FABLE_STATIC_ASSERT(sizeof(CClassFactory)==sizeof(CClassFactoryFields));
class CThingManager:public CClassFactory{public:virtual~CThingManager();bool IsTypeLocal(unsigned long type)const{return ClassTypeInfo[type].Local;}protected:virtual void OnCreateClass(CClass&);virtual void OnDeleteClass(CClass&);};
inline bool CThing::IsLocal()const{return PCurrentThingManager->IsTypeLocal(Type);}
unsigned long CThing::GetChecksum()const{
 if(IsLocal())return 0;
 unsigned long checksum=Type+PlayerNumber+DefGlobalIndex;
 if(PhysicsTC){float sum=PhysicsTC->GetPosition().Z;sum+=PhysicsTC->GetPosition().Y;sum+=PhysicsTC->GetPosition().X;
  sum+=PhysicsTC->GetVelocity().Z;sum+=PhysicsTC->GetVelocity().Y;sum+=PhysicsTC->GetVelocity().X;
  checksum=static_cast<unsigned long>(checksum+sum);
 }
 return checksum;
}