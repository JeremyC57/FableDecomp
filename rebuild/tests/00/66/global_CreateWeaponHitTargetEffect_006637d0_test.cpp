// 006637d0 CreateWeaponHitTargetEffect
// Flags: /O2 /Oy /W3 
// Leaf: CreateWeaponHitTargetEffect
#include <math.h>
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCPhysicsBase.h"
#pragma intrinsic(sqrt)
namespace X_006637d0 {
struct C3DVector{float X,Y,Z;C3DVector(){} __forceinline C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){} C3DVector(float a,float b,float c):X(a),Y(b),Z(c){} __forceinline C3DVector& operator=(const C3DVector&r){X=r.X;Y=r.Y;Z=r.Z;return *this;} __forceinline C3DVector&operator-=(const C3DVector&r){X-=r.X;Y-=r.Y;Z-=r.Z;return *this;} C3DVector& Normalise(); __forceinline float DotProduct(const C3DVector&other)const{return Z*other.Z+Y*other.Y+X*other.X;} __forceinline C3DVector operator*(float k)const{return C3DVector(X*k,Y*k,Z*k);} __forceinline C3DVector operator-(const C3DVector&r)const{return C3DVector(X-r.X,Y-r.Y,Z-r.Z);} };struct CRightHandedSet{C3DVector Up,Forward;__forceinline const C3DVector&GetUp()const{return Up;}};struct CCharStringData;struct CCharString{CCharStringData*p;CCharString(const char*,long);~CCharString();};
struct CTCPhysicsBaseVirtualSlots{virtual void V0()=0;virtual void V1()=0;virtual void V2()=0;virtual void V3()=0;virtual void V4()=0;virtual void V5()=0;virtual void V6()=0;virtual void V7()=0;virtual void V8()=0;virtual void V9()=0;virtual void V10()=0;virtual void V11()=0;virtual void V12()=0;virtual void V13()=0;virtual void V14()=0;virtual void V15()=0;virtual void V16()=0;virtual void V17()=0;virtual void V18()=0;virtual void V19()=0;virtual void V20()=0;virtual void V21()=0;virtual void V22()=0;virtual void V23()=0;virtual void V24()=0;virtual void V25()=0;virtual void V26()=0;virtual void V27()=0;virtual void V28()=0;virtual void V29()=0;virtual void V30()=0;virtual void V31()=0;virtual void V32()=0;virtual void V33()=0;virtual void V34()=0;virtual void V35()=0;virtual void V36()=0;virtual void V37()=0;virtual void V38()=0;virtual void V39()=0;virtual void V40()=0;virtual void V41()=0;virtual void V42()=0;virtual void V43()=0;virtual void V44()=0;virtual void V45()=0;virtual void V46()=0;virtual void V47()=0;virtual void V48()=0;virtual void V49()=0;virtual void V50()=0;virtual void V51()=0;virtual void V52()=0;virtual void V53()=0;virtual void V54()=0;virtual void V55()=0;virtual void V56()=0;virtual void V57()=0;virtual void V58()=0;virtual void V59()=0;virtual void V60()=0;virtual void V61()=0;virtual void V62()=0;virtual void V63()=0;virtual void V64()=0;virtual void V65()=0;virtual void V66()=0;virtual void V67()=0;virtual void V68()=0;virtual void V69()=0;virtual void V70()=0;virtual void V71()=0;virtual const CRightHandedSet&GetRHSet()const=0;};

// Layout: CDefPointeeBase prefix: vptr+0, reference count+4, whole donor base size28; CSpecialEffectsDef uses this base and is quarantined.
struct CDefPointeeBase{virtual void A()=0;virtual void DeleteThis()=0;int refs;char RemainingBase[20];__forceinline void AddRef(){++refs;} __forceinline void Release(){if(--refs==0)DeleteThis();}};
struct CSpecialEffectsDef: CDefPointeeBase{bool GetEffectFromString(const CCharString&,long&)const;};
// Layout: struct_layouts_egor CDefPointer has one CDefPointeeBase* Object; conversion uses its zero-offset definition base.
template<class T>struct CDefPointer{CDefPointeeBase*Object;__forceinline CDefPointer():Object(0){} __forceinline explicit CDefPointer(CDefPointeeBase*p):Object(p){if(Object)Object->AddRef();} __forceinline CDefPointer(const CDefPointer&r):Object(r.Object){if(Object)Object->AddRef();} __forceinline ~CDefPointer(){if(Object)Object->Release();} __forceinline T*operator->()const{return reinterpret_cast<T*>(Object);} __forceinline CDefPointer&operator=(const CDefPointer&r){if(Object!=r.Object){if(Object)Object->Release();Object=r.Object;if(Object)Object->AddRef();}return *this;}};
struct CSubDefInfo{long DefIndex,OriginalParentDefIndex;};
struct CDefClassBaseVirtualSlots{virtual void V0()=0;virtual void V1()=0;virtual void V2()=0;virtual void V3()=0;virtual void V4()=0;virtual void V5()=0;virtual void V6()=0;virtual void V7()=0;virtual void V8()=0;virtual void V9()=0;virtual void V10()=0;virtual void V11()=0;virtual void V12()=0;virtual void V13()=0;virtual const CSubDefInfo*GetPSubDefInfo(const CCharString&)const=0;};
struct CDefinitionManagerMethods: ::CDefinitionManager{CDefPointer<::CDefClassBase>GetPDefFromGlobalIndex(long);};
struct CDefClassBaseMethods: ::CDefClassBase {bool GetSubDef(CDefPointer<const CSpecialEffectsDef>*)const;};
// Fidelity: full noinline GetSubDef semantics, from 669580: string-key lookup, manager acquisition, conversion and balanced ownership. Out-object address never escapes.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBaseMethods::GetSubDef(CDefPointer<const CSpecialEffectsDef>*out)const{
 const CSubDefInfo*info;
 {CCharString name("CSpecialEffectsDef",-1);info=((const CDefClassBaseVirtualSlots*)this)->GetPSubDefInfo(name);}
 if(!info)return false;
 CDefPointer<::CDefClassBase> base=static_cast<CDefinitionManagerMethods*>(PDefManager)->GetPDefFromGlobalIndex(info->DefIndex);
 CDefPointer<const CSpecialEffectsDef> typed(base.Object);
 *out=typed;
 return true;
}
#pragma auto_inline(on)
struct CThing;struct CTCDParticleEmitter{static CThing* __fastcall Create(long,const C3DVector&,bool);void SetEmitterPosition(const C3DVector&);void SetEmitterOrientation(const CRightHandedSet&);};struct CThing;

// Layout: active CDefClassBase and CTCPhysicsBase headers own storage. Methods/virtual-slots adapters add no fields; CThing and CSpecialEffectsDef headers are quarantined.
// Layout: exact oracle physics+60 resource+70 dead+91; effect flags+28 component+5c.
// Layout: physics vtable0125d2c4 slot72 is006b1290, returning the static RH set0137f9b4 (Up={0,0,1},Forward={0,1,0}); PDB identifies GetRHSet() const and GetUp() const.
struct CThing{char pad[0x20];unsigned UsingComponents[9];char mapAndLists[24];CTCDParticleEmitter*component;::CTCPhysicsBase*physics;char gap[12];::CDefClassBase*resource;char gap2[0x1d];unsigned char dead;const C3DVector&GetPosition()const;};

void __fastcall CreateWeaponHitTargetEffect(const CThing*thing,const C3DVector&position,const C3DVector&direction,bool flourish,bool knockout,bool counter){if(!thing||(thing->dead&1))return;CDefPointer<const CSpecialEffectsDef> def;long id=-1;const char*key=flourish?"LATERAL_SLASH_FLOURISH":knockout?"LATERAL_SLASH_KNOCKOUT":counter?"LATERAL_SLASH_COUNTER_BLOCK":"LATERAL_SLASH_NORMAL";{CCharString name(key,-1);bool acquired=static_cast<CDefClassBaseMethods*>(thing->resource)->GetSubDef(&def);if(acquired&&def->GetEffectFromString(name,id)){CThing*effect=CTCDParticleEmitter::Create(id,thing->GetPosition(),false);if(effect){CTCDParticleEmitter*c=0;if(effect->UsingComponents[2]&0x800)c=effect->component;c->SetEmitterPosition(position);C3DVector up;up=((CTCPhysicsBaseVirtualSlots*)thing->physics)->GetRHSet().GetUp();up.Normalise();C3DVector forward=direction;float dot=forward.DotProduct(up);forward-=up*dot;forward.Normalise();CRightHandedSet b;b.Up=up;b.Forward=forward;c->SetEmitterOrientation(b);}}}}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <float.h>
void* __cdecl operator new[](unsigned int size);
void __cdecl operator delete[](void* p);
namespace X_006637d0 {
__declspec(noinline) C3DVector& C3DVector::Normalise(){float factor=(float)(1.0/sqrt(Z*Z+Y*Y+X*X));X*=factor;Y*=factor;Z*=factor;return *this;}
static bool available=true,found=true,emitterAvailable=true;
static int oriented,stringsLive,lookups,created,destroyed;
static C3DVector power;
static CRightHandedSet result;
static CSpecialEffectsDef*definition;
static CThing effect;
static const char*expectedKey="LATERAL_SLASH_NORMAL";
#pragma pack(push,1)
struct CCharStringData{char*text;unsigned length,capacity;unsigned char owned;int refs;};
#pragma pack(pop)
CCharString::CCharString(const char*s,long):p(0){if(s&&*s){p=new CCharStringData;p->length=(unsigned)strlen(s);p->capacity=p->length+1;p->text=new char[p->capacity];memcpy(p->text,s,p->capacity);p->owned=1;p->refs=1;++stringsLive;}}
CCharString::~CCharString(){if(p&&--p->refs<=0){delete[]p->text;delete p;--stringsLive;}}
CDefPointer<::CDefClassBase>CDefinitionManagerMethods::GetPDefFromGlobalIndex(long id){assert(id==77);return CDefPointer<::CDefClassBase>(definition);}
bool CSpecialEffectsDef::GetEffectFromString(const CCharString&name,long&id)const{assert(name.p&&strcmp(name.p->text,expectedKey)==0);++lookups;if(found)id=1;return found;}
CThing* __fastcall CTCDParticleEmitter::Create(long id,const C3DVector&position,bool stopped){assert(id==1&&!stopped&&position.X==4&&position.Y==8&&position.Z==16);++created;return emitterAvailable?&effect:0;}
void CTCDParticleEmitter::SetEmitterPosition(const C3DVector&v){power=v;}
void CTCDParticleEmitter::SetEmitterOrientation(const CRightHandedSet&b){result=b;++oriented;}
const C3DVector&CThing::GetPosition()const{static C3DVector v(4,8,16);return v;}
struct D:CSpecialEffectsDef{void A(){}void DeleteThis(){++destroyed;delete this;}};
struct FakeDef:CDefClassBaseVirtualSlots{void V0(){}void V1(){}void V2(){}void V3(){}void V4(){}void V5(){}void V6(){}void V7(){}void V8(){}void V9(){}void V10(){}void V11(){}void V12(){}void V13(){} const CSubDefInfo*GetPSubDefInfo(const CCharString&name)const{assert(name.p&&strcmp(name.p->text,"CSpecialEffectsDef")==0);static CSubDefInfo info={77,88};return available?&info:0;}};
struct FakePhysics:CTCPhysicsBaseVirtualSlots{void V0(){}void V1(){}void V2(){}void V3(){}void V4(){}void V5(){}void V6(){}void V7(){}void V8(){}void V9(){}void V10(){}void V11(){}void V12(){}void V13(){}void V14(){}void V15(){}void V16(){}void V17(){}void V18(){}void V19(){}void V20(){}void V21(){}void V22(){}void V23(){}void V24(){}void V25(){}void V26(){}void V27(){}void V28(){}void V29(){}void V30(){}void V31(){}void V32(){}void V33(){}void V34(){}void V35(){}void V36(){}void V37(){}void V38(){}void V39(){}void V40(){}void V41(){}void V42(){}void V43(){}void V44(){}void V45(){}void V46(){}void V47(){}void V48(){}void V49(){}void V50(){}void V51(){}void V52(){}void V53(){}void V54(){}void V55(){}void V56(){}void V57(){}void V58(){}void V59(){}void V60(){}void V61(){}void V62(){}void V63(){}void V64(){}void V65(){}void V66(){}void V67(){}void V68(){}void V69(){}void V70(){}void V71(){} const CRightHandedSet&GetRHSet()const{static CRightHandedSet v;v.Up=C3DVector(0,0,1);v.Forward=C3DVector(0,1,0);return v;}};
}
int main(){using namespace X_006637d0;
 CThing t;D d;CDefClassBaseMethods r;CDefinitionManagerMethods manager;FakeDef defDispatch;FakePhysics physics;CTCDParticleEmitter comp;
 memset(&t,0,sizeof(t));memset(&effect,0,sizeof(effect));memset(&r,0,sizeof(r));memcpy(&r,&defDispatch,4);r.PDefManager=&manager;
 ::CTCPhysicsBase physicsStorage;memset(&physicsStorage,0,sizeof(physicsStorage));memcpy(&physicsStorage,&physics,4);t.physics=&physicsStorage;t.resource=&r;d.refs=1;definition=&d;effect.UsingComponents[2]=0x800;effect.component=&comp;
 C3DVector dir(1,0,1),position(2,2,3);
 CreateWeaponHitTargetEffect(0,position,dir,false,false,false);assert(stringsLive==0&&lookups==0);
 t.dead=1;CreateWeaponHitTargetEffect(&t,position,dir,false,false,false);assert(stringsLive==0&&lookups==0);t.dead=0;
 available=false;CreateWeaponHitTargetEffect(&t,position,dir,false,false,false);assert(stringsLive==0&&lookups==0&&d.refs==1);available=true;
 found=false;CreateWeaponHitTargetEffect(&t,position,dir,false,false,false);assert(stringsLive==0&&lookups==1&&created==0&&d.refs==1);found=true;
 emitterAvailable=false;CreateWeaponHitTargetEffect(&t,position,dir,false,false,false);assert(stringsLive==0&&created==1&&oriented==0&&d.refs==1);emitterAvailable=true;
 const char*keys[]={"LATERAL_SLASH_NORMAL","LATERAL_SLASH_COUNTER_BLOCK","LATERAL_SLASH_KNOCKOUT","LATERAL_SLASH_FLOURISH"};
 for(int mode=0;mode<4;++mode){expectedKey=keys[mode];CreateWeaponHitTargetEffect(&t,position,dir,mode==3,mode==2,mode==1);assert(oriented==mode+1&&stringsLive==0&&d.refs==1);assert(power.X==2&&power.Y==2&&power.Z==3);assert(result.Up.X==0&&result.Up.Y==0&&result.Up.Z==1&&result.Forward.X==1&&result.Forward.Y==0&&result.Forward.Z==0);}
 expectedKey=keys[3];CreateWeaponHitTargetEffect(&t,position,dir,true,true,true);assert(oriented==5&&d.refs==1&&stringsLive==0);
 D second;second.refs=1;{CDefPointer<const CSpecialEffectsDef>a(&d),b(&second);assert(d.refs==2&&second.refs==2);a=b;assert(d.refs==1&&second.refs==3);a=a;assert(second.refs==3);}assert(d.refs==1&&second.refs==1);
 D*finalDef=new D;finalDef->refs=0;definition=finalDef;expectedKey=keys[0];CreateWeaponHitTargetEffect(&t,position,dir,false,false,false);assert(destroyed==1&&stringsLive==0);definition=&d;
 {CDefPointer<const CSpecialEffectsDef>out(&second);available=false;assert(!r.GetSubDef(&out)&&out.Object==&second);available=true;assert(r.GetSubDef(&out)&&out.Object==&d&&second.refs==1&&d.refs==2);}assert(d.refs==1);
 C3DVector axis(0,0,2);assert(&axis.Normalise()==&axis&&axis.X==0&&axis.Y==0&&axis.Z==1);C3DVector zero(0,0,0);zero.Normalise();assert(_isnan(zero.X)&&_isnan(zero.Y)&&_isnan(zero.Z));
 puts("006637d0_TEST PASS");return 0;
}