// CEnginePrimitiveManagerParticleLineGroup::~CEnginePrimitiveManagerParticleLineGroup -- release index/vertex buffers, counted shaders and shader array, then CBase; retail00bc5420,217bytes, tailjmp0099a300. PDB manager header has16byte CArray while retail has12bytes, shifting every member after array by-4. Flat shared buffer records lack lifecycle methods; local lifecycle models preserve their layouts. Calls00ac1ee0 Uninitialise,00a63200 Uninitialise,0099a300 base dtor,00bfea14 free.
// Leaf: ??1CEnginePrimitiveManagerParticleLineGroup@@UAE@XZ
// Naming wave 25 (c692 queue): CIndexBuffer::Uninitialise 0x00AC1EE0 / CVertexBufferWin32::Uninitialise 0x00A63200 (Ego_r.pdb) on the PDB
//   classes CIndexBuffer / CVertexBufferWin32 / CMovableResource (were the stand-ins CIndexBuffer_Retail::Release, CVertexBufferWin32_Retail,
//   CMovableResource_Retail; their vtables are the ones 0x00AC2010 / 0x00BB63A0 / 0x009D5110 bind), and the row's own class is the PDB
//   CEnginePrimitiveManagerParticleLineGroup (was ..._Retail); the engine layout headers are included under a layout alias.
#include <stddef.h>
#ifdef _MSC_VER
#define CEnginePrimitiveManagerParticleLineGroup CEnginePrimitiveManagerParticleLineGroupLayout
#define CIndexBuffer CIndexBufferLayout
#define CVertexBufferWin32 CVertexBufferWin32Layout
#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
#undef CEnginePrimitiveManagerParticleLineGroup
#undef CIndexBuffer
#undef CVertexBufferWin32
#include "engine/CVertexShader.h"
#else
#define __declspec(x)
#define __forceinline inline
#define __cdecl
#endif
#include <stdlib.h>
class CBaseClassNonCopyable { public: protected: virtual ~CBaseClassNonCopyable(); public: };   // CBaseClassNonCopyable (Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ; was stand-in CBase_Retail, naming wave 15)
class CMovableResource { public: virtual ~CMovableResource(){} };
class CIndexBuffer : public CMovableResource { public: void* PD3DIndexBuffer;unsigned long ByteLength;unsigned long PrimitiveType;void* PAllocatedBuffer; void Uninitialise(); __forceinline ~CIndexBuffer(){Uninitialise();} };
struct CVertexCompressionParams_Retail {float PosScaleX,PosScaleY,PosScaleZ,PosScaleW;float PosBiasX,PosBiasY,PosBiasZ,PosBiasW;};
class CVertexBufferWin32 : public CBaseClassNonCopyable { public: void* PVertexBuffer;unsigned long Length;long Type;unsigned char Stride;CVertexCompressionParams_Retail CompressionParams; void Uninitialise(); __forceinline ~CVertexBufferWin32(){Uninitialise();} };
struct CShaderBase_Retail { virtual ~CShaderBase_Retail(){} };
struct CPixelShaderData_Retail { virtual ~CPixelShaderData_Retail(); void* PixelShader; long ReferenceCount; };
struct CVertexShaderData_Retail { virtual ~CVertexShaderData_Retail(); void* VertexDeclaration;void* VertexShader;const void* FormatDecl;long ReferenceCount; };
struct CPixelShader_Retail : CShaderBase_Retail { CPixelShaderData_Retail* Data; __forceinline ~CPixelShader_Retail(){CPixelShaderData_Retail* p=Data;if(p && --p->ReferenceCount==0)delete p;} };
struct CVertexShader_Retail : CShaderBase_Retail { CVertexShaderData_Retail* Data; __forceinline ~CVertexShader_Retail(){CVertexShaderData_Retail* p=Data;if(p && --p->ReferenceCount==0)delete p;} };
struct CShaderArray_Retail { CVertexShader_Retail* Start;CVertexShader_Retail* Finish;CVertexShader_Retail* End; __forceinline ~CShaderArray_Retail(){CVertexShader_Retail* last=Finish; for(CVertexShader_Retail* p=Start;p!=last;++p)p->~CVertexShader_Retail();if(Start)free(Start);} };
class __declspec(novtable) CEnginePrimitiveManagerParticleLineGroup : public CBaseClassNonCopyable { public:
 unsigned long Type; bool Enabled;
 CShaderArray_Retail VertexShadersLinePointLights;
 CVertexShader_Retail VertexShaderDisplacement;
 CPixelShader_Retail PixelShader,PixelShaderStipple,PixelShaderDisplacement;
 CVertexBufferWin32 VertexBuffer;
 CIndexBuffer IndexBuffer;
 unsigned char ConstantBlock[0x1e0];
 ~CEnginePrimitiveManagerParticleLineGroup();
};
CEnginePrimitiveManagerParticleLineGroup::~CEnginePrimitiveManagerParticleLineGroup(){}

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIndexBuffer)==sizeof(CIndexBufferLayout));
FABLE_STATIC_ASSERT(sizeof(CVertexBufferWin32)==sizeof(CVertexBufferWin32Layout));
FABLE_STATIC_ASSERT(sizeof(CVertexShader_Retail)==sizeof(CVertexShader));
#endif