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