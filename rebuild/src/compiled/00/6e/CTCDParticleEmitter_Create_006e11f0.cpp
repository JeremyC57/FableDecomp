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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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
struct CDefinitionManagerAdapter: ::CDefinitionManager {int GetDefGlobalIndexFromName(const CCharString&);};CThing* __fastcall Create(int,int,int,bool);namespace NParticleEngine{struct CParticleEmitterManager {static CParticleEmitterManager*__fastcall Get();::CParticleEmitter*CreateInterpolatedEmitter(int,int,float,const C3DVector&,const CRightHandedSet&,bool);};}extern int Frame;extern const float Scale;
} class CGameDefinitionManager : public X_006e11f0::CDefinitionManagerAdapter { public: static CGameDefinitionManager& __fastcall Get(); }; namespace X_006e11f0 {   // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ
// Layout: Matrix6a06e0 is vptr+position12+Axes24=40. Request6e2640 is flags4+IDs8+float4+Axes24+Update36=76.
// Ptr6e2710 holds data+0/control+4; CCPPointerInfo refs+0/callback+4/object+8.
// Layout: thing flags+0x28, extra+0x5c; extra flags+0x44,time+0x38,pointer+0x3c.
CThing* __fastcall Create(int a,int b,float c,const C3DVector& position,bool persistent){if(!persistent&&((const CWorldMapAdapter&)((const CWorldVirtualSlots*)g_pMainGameComponent->PWorld)->PeekWorldMap()).IsLoadingAnyMap())persistent=true;CThing*object=0;int id=((const CWorldMapVirtualSlots*)g_pThingManager->WorldMap)->GetMapNumberAt(position);if(id>0){int def;{CCharString name("PARTICLE_EMITTER_NORMAL");def=::CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);}object=Create(def,4,id,true);CRightHandedSet axes=DefaultAxes;CTCPhysicsBaseVirtualSlots*physics=(CTCPhysicsBaseVirtualSlots*)object->physics;physics->ConstructFromParams(CTCParamsPhysicsBase(position,axes));CTCDParticleEmitter*extra=0;if(object->flags&0x800)extra=object->extra;extra->zero=0;extra->flags=(extra->flags&0xf9)|((persistent&1)<<1);object->DoActivationSequence();NParticleEngine::CParticleEmitterManager*effects=NParticleEngine::CParticleEmitterManager::Get();::CParticleEmitter*effect=effects->CreateInterpolatedEmitter(a,b,c,position,axes,persistent);if(effect)extra->SetPEmitter(effect);else if(!persistent){extra->flags|=1;extra->frame=(int)(Frame*Scale);extra->pointer.Reset(new CParticleCreationParameters(a,b,c,axes));}else{object->Kill(false);object=0;}}return object;}
}