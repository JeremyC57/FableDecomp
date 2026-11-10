// Self-contained C++03 behaviour test. Exact selected target and inline render helpers follow.
// Engine services are controlled fixtures; this checks queueing, distance, lifetime and exit paths.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#ifndef _MSC_VER
#define __fastcall
#endif
static int allocations,base_constructions,queue_calls,fail_allocation;
static void require(bool b){if(!b){puts("00bca6b0_TEST FAIL");exit(1);}}
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}
 const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
 float GetSquaredMagnitude()const{return (X*X+Z*Z+Y*Y);}float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
};
template<class T>class CArray:public std::vector<T>{public:T& operator[](unsigned int i){return *(this->begin()+i);}const T& operator[](unsigned int i)const{return *(this->begin()+i);}};
class CBaseClass {public:virtual ~CBaseClass(){}};
class CBaseClassNonCopyable:public CBaseClass {protected:CBaseClassNonCopyable(){++base_constructions;}virtual ~CBaseClassNonCopyable(){}private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);};
enum ERendered{RENDERED_NOTHING=0,RENDERED_BOUNDING_VOLUME=1,RENDERED_OBJECT=2};
enum EEngineRenderLayer{RENDER_LAYER_NULL=0,RENDER_LAYER_EFFECTS=2097152};
class CEngineScene{};class CEngineSubPrimitiveChain{};
class CEngineCamera {public:C3DVector Position;const C3DVector& PeekCameraPos()const{return Position;}};
class CEnginePrimitiveRenderUtil:public CBaseClassNonCopyable {public:static CEngineCamera& __fastcall GetCurrentCamera();static float __fastcall CalcDistanceFromCamera(const C3DVector&);};
class CEngineSubPrimitiveBase;
class CEngineInternalPrimitiveBase {public:virtual ~CEngineInternalPrimitiveBase(){}};
class CEngineInternalPrimitiveSpriteTrail:public CEngineInternalPrimitiveBase {public:class CSpriteTrailEntry{public:C3DVector Position;float Width;};CArray<CSpriteTrailEntry> Trail;};
class CEnginePrimitiveManager:public CEnginePrimitiveRenderUtil {public:CEnginePrimitiveManager():Enable(false){}bool GetEnable()const{return Enable;}bool Enable;protected:void AddSubPrimitive(CEngineSubPrimitiveBase*,float,unsigned char,unsigned long);};
class CEnginePrimitiveManagerSpriteTrails:public CEnginePrimitiveManager {public:virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);};
class CEngineSubPrimitiveBase:public CBaseClassNonCopyable {public:
 CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent):Parent(parent),NextSubPrimitive(0){}
 virtual ~CEngineSubPrimitiveBase(){}
#ifdef _MSC_VER
 static void* __fastcall operator new(unsigned int n)throw()
#else
 static void* operator new(size_t n)throw()
#endif
 {++allocations;if(fail_allocation)return 0;void* p=malloc(n);require(p!=0);memset(p,0xcd,n);return p;}
 static void __fastcall operator delete(void* p){free(p);}
 unsigned long RenderFlags;unsigned char Alpha;CEngineInternalPrimitiveBase* Parent;CEngineSubPrimitiveBase* NextSubPrimitive;CEnginePrimitiveManager* Manager;
};
class CEngineSubPrimitiveSpriteTrail:public CEngineSubPrimitiveBase {public:CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveSpriteTrail* parent):CEngineSubPrimitiveBase(parent){}virtual ~CEngineSubPrimitiveSpriteTrail(){}};
static CEngineSubPrimitiveBase* queued;static CEnginePrimitiveManager* queued_manager;static float queued_distance;static unsigned long queued_flags;static unsigned char queued_alpha;
class CEngineSubPrimitiveRenderer {public:void AddSubPrimitive(CEngineSubPrimitiveBase* p,float d,unsigned char a,unsigned long f,CEnginePrimitiveManager* m){++queue_calls;queued=p;queued_distance=d;queued_alpha=a;queued_flags=f;queued_manager=m;}};
namespace NEngineInternal {CEngineCamera* GPCamera;CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;}
inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera(){return *NEngineInternal::GPCamera;}
inline float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& world_pos){return (world_pos-GetCurrentCamera().PeekCameraPos()).GetMagnitude();}
inline void CEnginePrimitiveManager::AddSubPrimitive(CEngineSubPrimitiveBase* subprim,float distance,unsigned char primitive_alpha,unsigned long render_flags){NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(subprim,distance,primitive_alpha,render_flags,this);}
ERendered CEnginePrimitiveManagerSpriteTrails::RenderPrimitive(CEngineInternalPrimitiveBase& primitive_base,EEngineRenderLayer layer,CEngineSubPrimitiveChain& subprim_chain)
{
 if(!GetEnable())return RENDERED_NOTHING;
 CEngineInternalPrimitiveSpriteTrail& primitive=(CEngineInternalPrimitiveSpriteTrail&)primitive_base;
 if(!primitive.Trail.empty()) {
  C3DVector world_pos=primitive.Trail[0].Position;
  float distance=CalcDistanceFromCamera(world_pos);
  AddSubPrimitive(new CEngineSubPrimitiveSpriteTrail(&primitive),distance,0xff,1);
 }
 return RENDERED_OBJECT;
}

int main(){
 CEnginePrimitiveManagerSpriteTrails manager;CEngineSubPrimitiveRenderer renderer;CEngineCamera camera;CEngineSubPrimitiveChain chain;
 NEngineInternal::GPCamera=&camera;NEngineInternal::GPSubPrimitiveRenderer=&renderer;
 const float delta[6][3]={{0,0,0},{3,4,0},{-3,0,4},{0,3,-4},{0,0,-5},{8,15,0}};
 const float distances[6]={0,5,5,5,5,17};unsigned int cases=0;
 for(int enabled=0;enabled!=2;++enabled)for(int length=0;length!=4;++length)for(int failure=0;failure!=2;++failure)for(int point=0;point!=6;++point)for(int origin=0;origin!=4;++origin)for(int layer=0;layer!=2;++layer){
  camera.Position=C3DVector((float)(origin*4),(float)(origin*-8),(float)(origin*16));
  CEngineInternalPrimitiveSpriteTrail primitive;primitive.Trail.resize(length);
  for(int k=0;k!=length;++k){primitive.Trail[k].Position=C3DVector(camera.Position.X+delta[point][0]+k*100,camera.Position.Y+delta[point][1]+k*200,camera.Position.Z+delta[point][2]+k*300);primitive.Trail[k].Width=(float)(k+1);}
  manager.Enable=enabled!=0;fail_allocation=failure;allocations=base_constructions=queue_calls=0;queued=0;queued_manager=0;queued_distance=-1;queued_flags=0;queued_alpha=0;
  ERendered result=manager.RenderPrimitive(primitive,layer?RENDER_LAYER_EFFECTS:RENDER_LAYER_NULL,chain);
  require(result==(enabled?RENDERED_OBJECT:RENDERED_NOTHING));bool should_queue=enabled&&length;
  require(allocations==(should_queue?1:0));require(queue_calls==(should_queue?1:0));require(base_constructions==(should_queue&&!failure?1:0));
  if(should_queue){require(queued_manager==&manager&&queued_distance==distances[point]&&queued_flags==1&&queued_alpha==255);require((queued!=0)==!failure);if(queued){require(queued->Parent==&primitive&&queued->NextSubPrimitive==0);delete queued;}}
  require(primitive.Trail.size()==(unsigned int)length);for(int k=0;k!=length;++k)require(primitive.Trail[k].Width==(float)(k+1));++cases;
 }
 require(cases==768);puts("00bca6b0_TEST PASS");return 0;
}