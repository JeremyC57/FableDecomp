// CEnginePrimitiveManagerParticleLineGroup::~CEnginePrimitiveManagerParticleLineGroup -- release index/vertex buffers, counted shaders and shader array, then CBase; retail00bc5420,217bytes, tailjmp0099a300. PDB manager header has16byte CArray while retail has12bytes, shifting every member after array by-4. Flat shared buffer records lack lifecycle methods; local lifecycle models preserve their layouts. Calls00ac1ee0 Release,00a63200 Uninitialise,0099a300 base dtor,00bfea14 free.
// Leaf: ??1CEnginePrimitiveManagerParticleLineGroup_Retail
#include <stddef.h>
#ifdef _MSC_VER
#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#else
#define __declspec(x)
#define __forceinline inline
#define __cdecl
#endif
#include <stdlib.h>
struct CBase_Retail { virtual ~CBase_Retail(); };
struct CMovableResource_Retail { virtual ~CMovableResource_Retail(){} };
struct CIndexBuffer_Retail : CMovableResource_Retail { void* PD3DIndexBuffer;unsigned long ByteLength;unsigned long PrimitiveType;void* PAllocatedBuffer; void Release(); __forceinline ~CIndexBuffer_Retail(){Release();} };
struct CVertexCompressionParams_Retail {float PosScaleX,PosScaleY,PosScaleZ,PosScaleW;float PosBiasX,PosBiasY,PosBiasZ,PosBiasW;};
struct CVertexBufferWin32_Retail : CBase_Retail { void* PVertexBuffer;unsigned long Length;long Type;unsigned char Stride;CVertexCompressionParams_Retail CompressionParams; void Uninitialise(); __forceinline ~CVertexBufferWin32_Retail(){Uninitialise();} };
struct CShaderBase_Retail { virtual ~CShaderBase_Retail(){} };
struct CPixelShaderData_Retail { virtual ~CPixelShaderData_Retail(); void* PixelShader; long ReferenceCount; };
struct CVertexShaderData_Retail { virtual ~CVertexShaderData_Retail(); void* VertexDeclaration;void* VertexShader;const void* FormatDecl;long ReferenceCount; };
struct CPixelShader_Retail : CShaderBase_Retail { CPixelShaderData_Retail* Data; __forceinline ~CPixelShader_Retail(){CPixelShaderData_Retail* p=Data;if(p && --p->ReferenceCount==0)delete p;} };
struct CVertexShader_Retail : CShaderBase_Retail { CVertexShaderData_Retail* Data; __forceinline ~CVertexShader_Retail(){CVertexShaderData_Retail* p=Data;if(p && --p->ReferenceCount==0)delete p;} };
struct CShaderArray_Retail { CVertexShader_Retail* Start;CVertexShader_Retail* Finish;CVertexShader_Retail* End; __forceinline ~CShaderArray_Retail(){CVertexShader_Retail* last=Finish; for(CVertexShader_Retail* p=Start;p!=last;++p)p->~CVertexShader_Retail();if(Start)free(Start);} };
struct __declspec(novtable) CEnginePrimitiveManagerParticleLineGroup_Retail : CBase_Retail {
 unsigned long Type; bool Enabled;
 CShaderArray_Retail VertexShadersLinePointLights;
 CVertexShader_Retail VertexShaderDisplacement;
 CPixelShader_Retail PixelShader,PixelShaderStipple,PixelShaderDisplacement;
 CVertexBufferWin32_Retail VertexBuffer;
 CIndexBuffer_Retail IndexBuffer;
 unsigned char ConstantBlock[0x1e0];
 ~CEnginePrimitiveManagerParticleLineGroup_Retail();
};
CEnginePrimitiveManagerParticleLineGroup_Retail::~CEnginePrimitiveManagerParticleLineGroup_Retail(){}

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIndexBuffer_Retail)==sizeof(CIndexBuffer));
FABLE_STATIC_ASSERT(sizeof(CVertexBufferWin32_Retail)==sizeof(CVertexBufferWin32));
FABLE_STATIC_ASSERT(sizeof(CVertexShader_Retail)==sizeof(CVertexShader));
#endif

#include <cstdio>
#include <cstdlib>
#include <new>
static int failures=0,events[32],eventCount=0;
static void* vertexAddress=0;
static void check(bool b){if(!b)++failures;}
static void event(int n){events[eventCount++]=n;}
CBase_Retail::~CBase_Retail(){event(this==vertexAddress?3:9);}
void CIndexBuffer_Retail::Release(){check(PD3DIndexBuffer==(void*)123);event(1);}
void CVertexBufferWin32_Retail::Uninitialise(){check(PVertexBuffer==(void*)456);event(2);}
CPixelShaderData_Retail::~CPixelShaderData_Retail(){check(ReferenceCount==0);event((int)(size_t)PixelShader);}
CVertexShaderData_Retail::~CVertexShaderData_Retail(){check(ReferenceCount==0);event((int)(size_t)VertexShader);}
static CPixelShaderData_Retail* pixel(int id,long refs){CPixelShaderData_Retail* p=new CPixelShaderData_Retail;p->PixelShader=(void*)(size_t)id;p->ReferenceCount=refs;return p;}
static CVertexShaderData_Retail* vertex(int id,long refs){CVertexShaderData_Retail* p=new CVertexShaderData_Retail;p->VertexShader=(void*)(size_t)id;p->ReferenceCount=refs;return p;}
static void run(bool populated)
{
 eventCount=0;
 void* memory=std::malloc(sizeof(CEnginePrimitiveManagerParticleLineGroup_Retail));
 CEnginePrimitiveManagerParticleLineGroup_Retail* m=new(memory) CEnginePrimitiveManagerParticleLineGroup_Retail;
 vertexAddress=&m->VertexBuffer;m->IndexBuffer.PD3DIndexBuffer=(void*)123;m->VertexBuffer.PVertexBuffer=(void*)456;
 m->VertexShadersLinePointLights.Start=0;m->VertexShadersLinePointLights.Finish=0;m->VertexShadersLinePointLights.End=0;
 m->PixelShader.Data=0;m->PixelShaderStipple.Data=0;m->PixelShaderDisplacement.Data=0;m->VertexShaderDisplacement.Data=0;
 CPixelShaderData_Retail* shared=0;CVertexShaderData_Retail* retained=0;
 if(populated){
  m->PixelShaderDisplacement.Data=pixel(4,1);m->PixelShaderStipple.Data=pixel(5,1);shared=pixel(6,2);m->PixelShader.Data=shared;
  m->VertexShaderDisplacement.Data=vertex(7,1);
  CVertexShader_Retail* a=(CVertexShader_Retail*)std::malloc(sizeof(CVertexShader_Retail)*3);
  for(int i=0;i<3;++i)new(a+i) CVertexShader_Retail;
  a[0].Data=vertex(8,1);a[1].Data=0;retained=vertex(10,2);a[2].Data=retained;
  m->VertexShadersLinePointLights.Start=a;m->VertexShadersLinePointLights.Finish=a+3;m->VertexShadersLinePointLights.End=a+3;
 }
 m->CEnginePrimitiveManagerParticleLineGroup_Retail::~CEnginePrimitiveManagerParticleLineGroup_Retail();std::free(memory);
 if(populated){int expected[]={1,2,3,4,5,7,8,9};check(eventCount==8);for(int j=0;j<8 && j<eventCount;++j)check(events[j]==expected[j]);check(shared->ReferenceCount==1 && retained->ReferenceCount==1);shared->ReferenceCount=0;delete shared;retained->ReferenceCount=0;delete retained;}
 else{check(eventCount==4);check(events[0]==1 && events[1]==2 && events[2]==3 && events[3]==9);}
}
int main(){run(false);run(true);if(failures){std::printf("00bc5420_TEST FAIL\n");return 1;}std::printf("00bc5420_TEST PASS\n");return 0;}