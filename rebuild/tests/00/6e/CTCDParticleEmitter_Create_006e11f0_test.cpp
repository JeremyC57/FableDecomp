// 006e11f0 CTCDParticleEmitter::Create
// Flags: /O2 /Oy /W3
// Leaf: Create
#include <stdlib.h>
#include <string.h>
#include "engine/CTCPhysicsBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CParticleEmitter.h"
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace X_006e11f0 {
struct C3DVector {float X,Y,Z;__forceinline C3DVector(){}__forceinline C3DVector(const C3DVector&a):X(a.X),Y(a.Y),Z(a.Z){}};struct CRightHandedSet {C3DVector Up,Forward;__forceinline CRightHandedSet(){}__forceinline CRightHandedSet(const CRightHandedSet&a):Up(a.Up),Forward(a.Forward){}};extern CRightHandedSet DefaultAxes;
struct CBaseClass {virtual ~CBaseClass();};
struct CBaseClassNonCopyable:CBaseClass {__declspec(noinline) CBaseClassNonCopyable();__declspec(noinline) virtual ~CBaseClassNonCopyable();};
struct CTCParamsPhysicsBase:CBaseClassNonCopyable {C3DVector position;CRightHandedSet axes;__declspec(noinline) CTCParamsPhysicsBase(const C3DVector& p,const CRightHandedSet& a):position(p),axes(a){}};
// Method-only virtual dispatch view of actual flattened CTCPhysicsBase, not an original class reconstruction.
struct CTCPhysicsBaseVirtualSlots {virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual void ConstructFromParams(const CTCParamsPhysicsBase&)=0;};
#pragma pack(push,1)
template<class Char>struct CBasicString{Char*PData;unsigned StringLength,DataLength;bool UseFastExtend;};
struct CCharStringData{CBasicString<char>Data;int NoRefs;};
#pragma pack(pop)
extern int StringInstances;
struct CCharString {CCharStringData*Data;__declspec(noinline) CCharString(const char*p,int n=-1):Data(0){++StringInstances;if(!p||!*p)return;if(n<0)n=(int)strlen(p);Data=new CCharStringData;Data->Data.PData=new char[n+1];memcpy(Data->Data.PData,p,n);Data->Data.PData[n]=0;Data->Data.StringLength=n;Data->Data.DataLength=n+1;Data->Data.UseFastExtend=true;Data->NoRefs=1;}__declspec(noinline) CCharString(const CCharString&p):Data(0){++StringInstances;if(this!=&p&&p.Data){Data=p.Data;++Data->NoRefs;}}__declspec(noinline) ~CCharString(){if(Data&&--Data->NoRefs<=0){if(Data->Data.PData){::operator delete[](Data->Data.PData);Data->Data.PData=0;}Data->Data.StringLength=0;Data->Data.DataLength&=0x80000000u;delete Data;}Data=0;--StringInstances;}const char*Text()const{return Data?Data->Data.PData:"";}};
template<class K,class V>struct CParticleUpdateParamDesc:CBaseClassNonCopyable{void*Callback;V Index;};
template<class K,class V>struct CVectorMap {struct Entry{K Key;V Value;};Entry*first,*last,*end;unsigned char flags[4];CVectorMap():first(0),last(0),end(0){flags[1]=0;}~CVectorMap(){for(Entry*p=first;p!=last;++p)p->~Entry();if(first)free(first);}};
template<class K,class V>struct CParticleUpdateParameters:CBaseClassNonCopyable {CVectorMap<K,C3DVector>VectorParams;CVectorMap<K,CParticleUpdateParamDesc<K,V> >UpdateParams;};
struct CParticleCreationParameters {unsigned char flags[4];int start,end;float fraction;CRightHandedSet axes;CParticleUpdateParameters<unsigned,unsigned> update;__declspec(noinline) CParticleCreationParameters(int a,int b,float f,const CRightHandedSet& X):start(a),end(b),fraction(f),axes(X){flags[0]=1;flags[1]=flags[2]=flags[3]=0;}};
struct CCPPointerInfo {int RefCount;void (__fastcall*DeleteFunc)(void*);void*Data;};
void __fastcall DeleteRequest(void*data){delete (CParticleCreationParameters*)data;}
template<class T>struct CCountedPointer {T*data;CCPPointerInfo*info;__declspec(noinline) void Reset(T*p){if(info){if(!--info->RefCount){info->DeleteFunc(info->Data);delete info;}}info=0;data=p;if(p){info=new CCPPointerInfo;if(info){info->RefCount=1;info->DeleteFunc=DeleteRequest;info->Data=p;}}}};
struct CTCDParticleEmitter {char pad[16];int zero;char gap[36];int frame;CCountedPointer<CParticleCreationParameters> pointer;unsigned char flags;void SetPEmitter(::CParticleEmitter*);};
struct CThing {char pad[40];unsigned flags;char gap[48];CTCDParticleEmitter* extra;::CTCPhysicsBase* physics;bool DoActivationSequence();void Kill(bool);};
struct CWorldMapVirtualSlots {
virtual void v0()=0;
virtual void v1()=0;
virtual void v2()=0;
virtual void v3()=0;
virtual void v4()=0;
virtual void v5()=0;
virtual void v6()=0;
virtual void v7()=0;
virtual void v8()=0;
virtual void v9()=0;
virtual void v10()=0;
virtual void v11()=0;
virtual void v12()=0;
virtual void v13()=0;
virtual void v14()=0;
virtual void v15()=0;
virtual int GetMapNumberAt(const C3DVector&)const=0;};struct CThingManager {char pad[40];::CWorldMap*WorldMap;};extern CThingManager* g_pThingManager;
struct CWorldMapAdapter: ::CWorldMap {bool IsLoadingAnyMap()const;};struct CWorldVirtualSlots {virtual void v0()=0;
virtual void v1()=0;
virtual void v2()=0;
virtual void v3()=0;
virtual void v4()=0;
virtual void v5()=0;
virtual void v6()=0;
virtual void v7()=0;
virtual void v8()=0;
virtual void v9()=0;
virtual void v10()=0;
virtual void v11()=0;
virtual void v12()=0;
virtual const ::CWorldMap& PeekWorldMap()const=0;};struct CMainGameComponent {char pad[36];::CWorld*PWorld;};extern CMainGameComponent* g_pMainGameComponent;
struct CDefinitionManagerAdapter: ::CDefinitionManager {static CDefinitionManagerAdapter* __fastcall Get();int GetDefGlobalIndexFromName(const CCharString&);};CThing* __fastcall Create(int,int,int,bool);namespace NParticleEngine{struct CParticleEmitterManager {static CParticleEmitterManager*__fastcall Get();::CParticleEmitter*CreateInterpolatedEmitter(int,int,float,const C3DVector&,const CRightHandedSet&,bool);};}extern int Frame;extern const float Scale;
// Layout: Matrix6a06e0 is vptr+position12+Axes24=40. Request6e2640 is flags4+IDs8+float4+Axes24+Update36=76.
// Ptr6e2710 holds data+0/control+4; CCPPointerInfo refs+0/callback+4/object+8.
// Layout: thing flags+0x28, extra+0x5c; extra flags+0x44,time+0x38,pointer+0x3c.
CThing* __fastcall Create(int a,int b,float c,const C3DVector& position,bool persistent){if(!persistent&&((const CWorldMapAdapter&)((const CWorldVirtualSlots*)g_pMainGameComponent->PWorld)->PeekWorldMap()).IsLoadingAnyMap())persistent=true;CThing*object=0;int id=((const CWorldMapVirtualSlots*)g_pThingManager->WorldMap)->GetMapNumberAt(position);if(id>0){int def;{CCharString name("PARTICLE_EMITTER_NORMAL");def=CDefinitionManagerAdapter::Get()->GetDefGlobalIndexFromName(name);}object=Create(def,4,id,true);CRightHandedSet axes=DefaultAxes;CTCPhysicsBaseVirtualSlots*physics=(CTCPhysicsBaseVirtualSlots*)object->physics;physics->ConstructFromParams(CTCParamsPhysicsBase(position,axes));CTCDParticleEmitter*extra=0;if(object->flags&0x800)extra=object->extra;extra->zero=0;extra->flags=(extra->flags&0xf9)|((persistent&1)<<1);object->DoActivationSequence();NParticleEngine::CParticleEmitterManager*effects=NParticleEngine::CParticleEmitterManager::Get();::CParticleEmitter*effect=effects->CreateInterpolatedEmitter(a,b,c,position,axes,persistent);if(effect)extra->SetPEmitter(effect);else if(!persistent){extra->flags|=1;extra->frame=(int)(Frame*Scale);extra->pointer.Reset(new CParticleCreationParameters(a,b,c,axes));}else{object->Kill(false);object=0;}}return object;}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace X_006e11f0 {
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CRightHandedSet DefaultAxes;struct InitialiseAxes{InitialiseAxes(){float*p=(float*)&DefaultAxes;for(int i=0;i<6;++i)p[i]=float(i+1);}}initialiseAxes;int Frame=8;const float Scale=0.5f;
static int id=7,mode=0,paused=0,created=0,deleted=0,activated=0,attached=0,setCalls=0,strings=0;
static bool expectedPersistent=true;static C3DVector expectedPosition;struct InitialisePosition{InitialisePosition(){expectedPosition.X=2;expectedPosition.Y=4;expectedPosition.Z=8;}}initialisePosition;static ::CParticleEmitter token;
CMainGameComponent* g_pMainGameComponent;CThingManager* g_pThingManager;
const char Empty[]="";int StringInstances=0;

static void CheckAxes(const CRightHandedSet& a){for(int i=0;i<6;++i)assert(((const float*)&a)[i]==((const float*)&DefaultAxes)[i]);}
static void CheckPosition(const C3DVector& p){assert(p.X==2&&p.Y==4&&p.Z==8);}
struct TestPhysics:CTCPhysicsBaseVirtualSlots {void v0(){}void v1(){}void v2(){}void ConstructFromParams(const CTCParamsPhysicsBase& m){++setCalls;CheckPosition(m.position);CheckAxes(m.axes);}};static TestPhysics physics;static ::CTCPhysicsBase physicsHeader;
CThing* __fastcall Create(int def,int kind,int definition,bool enabled){assert(def==31&&kind==4&&definition==7&&enabled);CThing* o=new CThing;memset(o,0,sizeof(*o));CTCDParticleEmitter* X=new CTCDParticleEmitter;memset(X,0,sizeof(*X));X->flags=0xa7;X->zero=99;o->flags=0x800;o->extra=X;physicsHeader.__vftable=*(void**)&physics;o->physics=&physicsHeader;++created;return o;}
static void Cleanup(CThing* p){if(p){p->extra->pointer.Reset(0);delete p->extra;delete p;}}
bool CThing::DoActivationSequence(){++activated;assert(extra->zero==0);assert((extra->flags&6)==(expectedPersistent?2:0));return true;}
void CThing::Kill(bool flag){assert(!flag);++deleted;Cleanup(this);}
void CTCDParticleEmitter::SetPEmitter(::CParticleEmitter* p){assert(p==&token);++attached;}
bool CWorldMapAdapter::IsLoadingAnyMap()const{return paused!=0;}
static CDefinitionManagerAdapter database;CDefinitionManagerAdapter* __fastcall CDefinitionManagerAdapter::Get(){return &database;}int CDefinitionManagerAdapter::GetDefGlobalIndexFromName(const CCharString& s){assert(strcmp(s.Text(),"PARTICLE_EMITTER_NORMAL")==0);return 31;}
static NParticleEngine::CParticleEmitterManager effects;NParticleEngine::CParticleEmitterManager*__fastcall NParticleEngine::CParticleEmitterManager::Get(){return &effects;}
::CParticleEmitter* NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter(int a,int b,float f,const C3DVector& p,const CRightHandedSet& axes,bool persistent){assert(a==11&&b==22&&f==0.375f&&persistent==expectedPersistent);CheckPosition(p);CheckAxes(axes);assert(StringInstances==0);return mode==0?&token:0;}
struct TestDefs:CWorldMapVirtualSlots {void v0(){}void v1(){}void v2(){}void v3(){}void v4(){}void v5(){}void v6(){}void v7(){}void v8(){}void v9(){}void v10(){}void v11(){}void v12(){}void v13(){}void v14(){}void v15(){}int GetMapNumberAt(const C3DVector& p)const{CheckPosition(p);return id;}};
static CWorldMapAdapter map;struct TestWorld:CWorldVirtualSlots {void v0(){}void v1(){}void v2(){}void v3(){}void v4(){}void v5(){}void v6(){}void v7(){}void v8(){}void v9(){}void v10(){}void v11(){}void v12(){}const ::CWorldMap& PeekWorldMap()const{return map;}};
}

static void*stringWatch;static int stringDeletes;void __cdecl operator delete[](void*p){if(p==stringWatch)++stringDeletes;free(p);}
namespace X_006e11f0{static void StringOwnershipProbe(){{CCharString value("owned");CCharString*copies[140];for(int i=0;i<140;++i)copies[i]=new CCharString(value);assert(value.Data->NoRefs==141);for(int j=0;j<140;++j)delete copies[j];assert(value.Data->NoRefs==1);value.Data->Data.UseFastExtend=false;stringWatch=value.Data->Data.PData;}assert(stringDeletes==1&&!StringInstances);{CCharString value("zero");value.Data->NoRefs=0;stringWatch=value.Data->Data.PData;}assert(stringDeletes==2&&!StringInstances);}}
int main(){X_006e11f0::StringOwnershipProbe();using namespace X_006e11f0;assert(sizeof(CTCParamsPhysicsBase)==40&&sizeof(CParticleUpdateParameters<unsigned,unsigned>)==36&&sizeof(CParticleCreationParameters)==76&&sizeof(CCountedPointer<CParticleCreationParameters>)==8&&sizeof(X_006e11f0::CCPPointerInfo)==12);
TestDefs mapMethods;::CWorldMap mapHeader={0};mapHeader.__vftable=*(void**)&mapMethods;X_006e11f0::CThingManager g;memset(&g,0,sizeof(g));g.WorldMap=&mapHeader;g_pThingManager=&g;TestWorld PWorld;X_006e11f0::CMainGameComponent r;memset(&r,0,sizeof(r));::CWorld worldHeader={0};worldHeader.__vftable=*(void**)&PWorld;r.PWorld=&worldHeader;g_pMainGameComponent=&r;
id=0;assert(Create(11,22,0.375f,expectedPosition,true)==0&&created==0);id=7;
CThing* o=Create(11,22,0.375f,expectedPosition,true);assert(o&&attached==1&&setCalls==1&&o->extra->pointer.data==0);Cleanup(o);
mode=1;assert(Create(11,22,0.375f,expectedPosition,true)==0&&deleted==1);
expectedPersistent=false;o=Create(11,22,0.375f,expectedPosition,false);assert(o&&o->extra->frame==4&&(o->extra->flags&7)==1);CParticleCreationParameters* q=o->extra->pointer.data;assert(q&&o->extra->pointer.info->RefCount==1&&o->extra->pointer.info->Data==q);assert(q->start==11&&q->end==22&&q->fraction==0.375f&&q->flags[0]==1&&q->flags[1]==0&&q->flags[2]==0&&q->flags[3]==0);CheckAxes(q->axes);assert(q->update.VectorParams.first==0&&q->update.UpdateParams.first==0&&q->update.VectorParams.flags[1]==0&&q->update.UpdateParams.flags[1]==0);
// A second pointer shares the control; cleanup must release only its own reference.
CCountedPointer<CParticleCreationParameters> share=o->extra->pointer;++share.info->RefCount;Cleanup(o);assert(share.info->RefCount==1&&share.data->fraction==0.375f);share.Reset(0);assert(share.data==0&&share.info==0);
paused=1;expectedPersistent=true;assert(Create(11,22,0.375f,expectedPosition,false)==0&&deleted==2);
assert(created==4&&activated==4&&setCalls==4&&StringInstances==0);puts("006e11f0_TEST PASS");return 0;}