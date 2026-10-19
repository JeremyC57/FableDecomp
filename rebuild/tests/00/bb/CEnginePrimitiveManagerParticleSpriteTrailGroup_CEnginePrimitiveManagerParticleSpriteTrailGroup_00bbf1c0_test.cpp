// CEnginePrimitiveManagerParticleSpriteTrailGroup::~CEnginePrimitiveManagerParticleSpriteTrailGroup -- drop trail manager, counted shaders, array and base; retail00bbf1c0 217bytes. RTTI012a5d94 slot0 deleting00bbf1a0 calls this body. Shared donor header CArray16 vs retail12 shifts fields after array by-4; lifecycle native view uses actual retail offsets. Shader headers are flat data records; native views supply genuine virtual destruction.
// Leaf: ??1CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView
#ifdef _MSC_VER
#include "engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h"
#include "engine/CVertexShader.h"
#else
#define __declspec(x)
#define __forceinline inline
#define __fastcall
#endif
#include <stdlib.h>
struct CBaseNativeView { virtual ~CBaseNativeView(); }; //0099a300
struct CShaderBaseNativeView { virtual ~CShaderBaseNativeView(){} };
struct CPixelShaderDataNativeView { virtual ~CPixelShaderDataNativeView(); void* PixelShader;long ReferenceCount; };
struct CVertexShaderData { virtual ~CVertexShaderData();void* VertexDeclaration;void* VertexShader;const void* FormatDecl;long ReferenceCount; };
struct CPixelShaderNativeView:CShaderBaseNativeView { CPixelShaderDataNativeView* Data; __forceinline virtual ~CPixelShaderNativeView(){CPixelShaderDataNativeView* p=Data;if(p && --p->ReferenceCount==0)delete p;} };
struct CVertexShaderNativeView:CShaderBaseNativeView { CVertexShaderData* Data; __forceinline virtual ~CVertexShaderNativeView(){CVertexShaderData* p=Data;if(p && --p->ReferenceCount==0)delete p;} };
struct CShaderArrayNativeView { CVertexShaderNativeView* Start;CVertexShaderNativeView* Finish;CVertexShaderNativeView* End; __forceinline ~CShaderArrayNativeView(){CVertexShaderNativeView* last=Finish;for(CVertexShaderNativeView* p=Start;p!=last;++p)p->~CVertexShaderNativeView();if(Start)free(Start);} };
struct CTrailManagerCountInfo {long ReferenceCount;void (__fastcall* Destroy)(void*);void* Object;};
struct CTrailManagerPointer { void* Data;CTrailManagerCountInfo* Info;__forceinline ~CTrailManagerPointer(){if(Info){--Info->ReferenceCount;if(Info->ReferenceCount==0){Info->Destroy(Info->Object);delete Info;}}Data=0;Info=0;} };
struct __declspec(novtable) CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView:CBaseNativeView {
 unsigned long Type;bool Enabled;
 CShaderArrayNativeView VertexShadersSpriteTrailPointLights;
 CVertexShaderNativeView VertexShaderDisplacement;
 CPixelShaderNativeView PixelShader,PixelShaderStipple,PixelShaderDisplacement;
 CTrailManagerPointer PTrailManager;
 virtual ~CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView();
};
CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView::~CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView(){}

#include <cstdio>
#include <cstdlib>
#include <new>
static int failures=0,events[32],eventCount=0,trailDestroyed=0;
static void check(bool b){if(!b)++failures;}
static void event(int n){events[eventCount++]=n;}
CBaseNativeView::~CBaseNativeView(){event(9);}
CPixelShaderDataNativeView::~CPixelShaderDataNativeView(){check(ReferenceCount==0);event((int)(size_t)PixelShader);}
CVertexShaderData::~CVertexShaderData(){check(ReferenceCount==0);event((int)(size_t)VertexShader);}
static void __fastcall DestroyTrail(void* object){check(*static_cast<int*>(object)==123);delete static_cast<int*>(object);++trailDestroyed;event(1);}
static CPixelShaderDataNativeView* pixel(int id,long refs){CPixelShaderDataNativeView* p=new CPixelShaderDataNativeView;p->PixelShader=(void*)(size_t)id;p->ReferenceCount=refs;return p;}
static CVertexShaderData* vertex(int id,long refs){CVertexShaderData* p=new CVertexShaderData;p->VertexShader=(void*)(size_t)id;p->ReferenceCount=refs;return p;}
static void run(int mode)
{
 eventCount=0;trailDestroyed=0;
 void* memory=std::malloc(sizeof(CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView));
 CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView* m=new(memory) CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView;
 m->VertexShadersSpriteTrailPointLights.Start=0;m->VertexShadersSpriteTrailPointLights.Finish=0;m->VertexShadersSpriteTrailPointLights.End=0;
 m->PixelShader.Data=0;m->PixelShaderStipple.Data=0;m->PixelShaderDisplacement.Data=0;m->VertexShaderDisplacement.Data=0;m->PTrailManager.Data=0;m->PTrailManager.Info=0;
 CPixelShaderDataNativeView* shared=0;CVertexShaderData* retained=0;CTrailManagerCountInfo* info=0;
 if(mode){
  info=new CTrailManagerCountInfo;info->ReferenceCount=mode;info->Object=new int(123);info->Destroy=DestroyTrail;m->PTrailManager.Info=info;m->PTrailManager.Data=info->Object;
  m->PixelShaderDisplacement.Data=pixel(4,1);m->PixelShaderStipple.Data=pixel(5,1);shared=pixel(6,2);m->PixelShader.Data=shared;m->VertexShaderDisplacement.Data=vertex(7,1);
  CVertexShaderNativeView* a=(CVertexShaderNativeView*)std::malloc(sizeof(CVertexShaderNativeView)*3);
  for(int i=0;i<3;++i)new(a+i) CVertexShaderNativeView;
  a[0].Data=vertex(8,1);a[1].Data=0;retained=vertex(10,2);a[2].Data=retained;
  m->VertexShadersSpriteTrailPointLights.Start=a;m->VertexShadersSpriteTrailPointLights.Finish=a+3;m->VertexShadersSpriteTrailPointLights.End=a+3;
 }
 m->CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView::~CEnginePrimitiveManagerParticleSpriteTrailGroupNativeView();std::free(memory);
 if(mode){int expected[]={1,4,5,7,8,9};int first=mode==1?0:1;check(eventCount==6-first);for(int j=first;j<6 && j-first<eventCount;++j)check(events[j-first]==expected[j]);check(shared->ReferenceCount==1 && retained->ReferenceCount==1);check(trailDestroyed==(mode==1?1:0));if(mode==2){check(info->ReferenceCount==1);DestroyTrail(info->Object);delete info;}shared->ReferenceCount=0;delete shared;retained->ReferenceCount=0;delete retained;}
 else check(eventCount==1 && events[0]==9);
}
int main(){run(0);run(1);run(2);if(failures){std::printf("00bbf1c0_TEST FAIL\n");return 1;}std::printf("00bbf1c0_TEST PASS\n");return 0;}