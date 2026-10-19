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