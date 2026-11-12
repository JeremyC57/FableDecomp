# OUTLINE 0x00BB5040 CEngineClothRenderer::CEngineClothRenderer()  -- ??0CEngineClothRenderer@@QAE@XZ

## Frame
- 0x00BB5040 `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,18h ; push ebx ; push ebp ; push esi ; push edi ; mov edi,ecx`.
  Saved order ebx, ebp, esi, edi. No EH (no fs:0, no state stores), no /GS. Plain `ret` at 0x00BB6317; `mov eax,edi` (return this).
- Epilogue 0x00BB6310: `pop edi ; pop esi ; pop ebp ; pop ebx ; mov esp,ebp ; pop ebp ; ret`. No cold blocks after it; the
  function ends at the ret. No jump tables.
- Registers: edi=this, ebx=0 (`xor ebx,ebx` 0x00BB505A), ebp=shader resource (0x00BB5119), esi=&current CArray / scratch.
- Frame map (F = esp after the four pushes, no pending pushes; the and-esp makes F+0x28 8-aligned):
  - F+0x10 : 4 bytes, unused (alignment pad below the temp)
  - F+0x14 / F+0x18 : two 4-byte slots that ALTERNATE per statement between `CCharString name` temporary (ctor 0x0099EBF0 /
    dtor 0x0099EAE0) and the cached CArray begin() pointer (`CVertexShader* p = arr.begin()` copy, read after the call). The compiler
    alternates them -- not a source difference.
  - F+0x1C : `const CVertexShader&` result of GetVertexShader (eax spilled; array statements only -- member statements keep it in esi/ebp)
  - F+0x20 : by-value `CVertexShader` temporary (8 bytes: +0x20 vptr, +0x24 Data); constructed by the callee (hidden result ptr),
    destroyed inline: `mov [F+0x20],0129B3D0h ; if(Data) Release ; mov [F+0x20],0129A030h`. This is the only 8-byte local and sits
    at aligned-8: the aligned frame is tied to it (see DOSSIER sec. 1 for hypotheses).

## Pseudo-C++ in retail order
```cpp
CEngineClothRenderer::CEngineClothRenderer()
    // 0x00BB504F call CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (via inline CEnginePrimitiveRenderUtil() {})
    // 0x00BB5054 mov [edi],012A56ECh  (CEngineClothRenderer vtable; RenderUtil's vptr store is dead-eliminated)
    // 0x00BB505C..0x00BB5095 CArray (STLport vector) default ctors: three zero words each at +4,+10,+1C,+28,+34,+48
    //   store order: 04 08 0C 10 14 18 1C 20 [lea esi,[edi+4]] 24 28 2C 30 34 38 3C 48 4C [lea ecx,[edi+54h]] 50
    // 0x00BB5098..0x00BB50C0 CVertexShader() 0x00A62FB0 x6 on +54 +5C +64 +6C +74 +7C
    // 0x00BB50CB CIndexBuffer() 0x00AC1EC0 on +8C
    : IndexBufferSize(-1)                      // 0x00BB50DC (scheduled between the pool's pushes and its call)
    , PositionBufferPool(0, 0x1000)           // 0x00BB50D0 push 1000h ; push ebx ; lea ecx,[edi+0A8h] ; call 0x00A5C570 @0x00BB50E6
    , NormalBufferPool(0, 0x1000)             // 0x00BB50EB ... call 0x00A5C570 @0x00BB50F7 (ecx = +0xCC)
{
    PositionBuffer = 0;                        // 0x00BB50FC +0xC4  (ebx)
    PositionBufferSize = 0;                    // 0x00BB5102 +0xC8
    NormalBuffer = 0;                          // 0x00BB5108 +0xE8
    NormalBufferSize = 0;                      // 0x00BB510E +0xEC
    // (these four are AFTER both pool ctor calls -> body statements, or init-list entries VC sank; body is the safe reading)

    const CShaderResource* shaders = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)0);
                                               // 0x00BB5114 mov eax,[01436E98h] ; mov ebp,[eax+29C0h]   (one load, kept in ebp)

    TriVertexShaders.resize(6);                // 0x00BB511F push 6 ; mov ecx,esi(+4) ; call 0x00B6CBD0 @0x00BB5123
    TriVertexShaders[0] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG");          // 0x00BB5128
    TriVertexShaders[1] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG"); // 0x00BB51B0
    TriVertexShaders[2] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG"); // 0x00BB5238
    TriVertexShaders[3] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG"); // 0x00BB52C0
    TriVertexShaders[4] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG"); // 0x00BB5348
    TriVertexShaders[5] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG"); // 0x00BB53D0
    TriVertexShaders2D = shaders->GetVertexShader("VSHADER_2D_STATIC_DIRLIGHT");             // 0x00BB544E (Data +0x68)
    TriBackfaceVertexShaders2D = shaders->GetVertexShader("VSHADER_2D_STATIC_DIRLIGHT_BACKFACE"); // 0x00BB54C4 (Data +0x70)

    TriVertexShadersDepthBufferShadow.resize(6);  // 0x00BB553A (+0x10), call @0x00BB5541
    ... [0..5] = "VSHADER_STATIC_DIRLIGHT_FOG_SHADOW", "_2POINTLIGHTS_FOG_SHADOW" x2, "_4POINTLIGHTS_FOG_SHADOW" x2, "_5POINTLIGHTS_FOG_SHADOW"
    TriVertexShadersSpotDepthBufferShadow.resize(6);  // 0x00BB586C (+0x1C), call @0x00BB5873
    ... [0..5] = "VSHADER_STATIC_DIRLIGHT_FOG_SPOT_SHADOW", "_2POINTLIGHTS_FOG_SPOT_SHADOW" x2, "_4POINTLIGHTS_..." x2, "_5POINTLIGHTS_..."

    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {
                                               // 0x00BB5B9E mov ecx,[01436E1Ch] ; call 0x009BEE00 ; cmp eax,578h ; jb 0x00BB6218
        TriVertexShadersColourBufferShadow.resize(6);      // 0x00BB5BB4 (+0x28)
        ... [0..5] "VSHADER_STATIC_DIRLIGHT_FOG_COLBUFF_SHADOW", "_2POINTLIGHTS_FOG_COLBUFF_SHADOW" x2, "_4POINTLIGHTS_..." x2, "_5POINTLIGHTS_..."
        TriVertexShadersSpotColourBufferShadow.resize(6);  // 0x00BB5EE6 (+0x34)
        ... [0..5] "VSHADER_STATIC_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW", "_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW" x2, "_4POINTLIGHTS_..." x2, "_5POINTLIGHTS_..."
    }
    TriVertexShaderOutline = shaders->GetVertexShader("VSHADER_STATIC_OUTLINE");                 // 0x00BB6218 (branch target; Data +0x58)
    TriVertexShaderOutlineSmoothFill = shaders->GetVertexShader("VSHADER_STATIC_OUTLINE_SMOOTH_FILL"); // 0x00BB628F (Data +0x60)

    EnableCloth = true;                        // 0x00BB6301 mov byte [edi+0A4h],1
    ClothDebug = false;                        // 0x00BB6308 mov byte [edi+88h],bl
}                                              // 0x00BB630E mov eax,edi ; ret
```
Only branch in the function body besides the inline refcount tests: `jb 0x00BB6218` (skip both colour-buffer arrays). The
comparison is unsigned (`jb`) -> GetPixelShaderVersion returns unsigned long; write `>= 1400` (or `< 1400` skip).

## One statement, expanded (retail 0x00BB5128..0x00BB51AF, array element i; 34 such statements)
```
push -1 ; push "literal" ; lea ecx,[name] ; call CCharString::CCharString(const char*, long = -1) 0x0099EBF0
mov ecx,[esi]   (begin of the CArray; for i>0 `mov eax,[esi]`)   -- LHS address taken BEFORE the call (begin()+i form, FABLE_RETAIL_CARRAY)
lea edx,[name] ; push ; lea eax,[temp] ; push (hidden result) ; mov [beginslot],ecx ; mov ecx,ebp ; call GetVertexShader 0x00A5D5F0
mov [F+1C],eax ; ecx = begin[i].Data ; if (ecx) { if (--ecx->ReferenceCount == 0) ecx->vfunc0(1); }      // operator= release
eax = result->Data ; begin[i].Data = eax ; ++eax->ReferenceCount                                         // operator= share
temp dtor: [temp]=0129B3D0h ; if (temp.Data) { --temp.Data->ReferenceCount (via reg, stored back) ; if 0 -> vfunc0(1) } ; [temp]=0129A030h
lea ecx,[name] ; call ~CCharString 0x0099EAE0
```
- Last element of each array (i=5): begin is held in esi (`mov esi,[esi]`) instead of a stack slot (esi no longer needed).
- Member statements (2D, backface, outline): `mov ecx,[edi+Data]` right after the call, result in esi (ebp for the last one).
- The Release in the temp dtor is the header's `Data->Release()` (`mov r,[eax+10h] ; dec r ; mov [eax+10h],r`) while operator= uses
  `dec dword ptr [ecx+10h]` -- exactly the two bodies engine/CVertexShader.h (FABLE_RETAIL_CVERTEXSHADER) already spells; the same
  expansion matched in 0x00BC14E0.
- The full per-statement address list (push -1 / ctor / GetVertexShader / ~CCharString of all 34) is in $W/units.txt.

## Suggested includes (author to verify)
```
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#include "engine/CEnginePrimitiveRenderUtil.h"  "engine/CArray.h"  "engine/CVertexShader.h"  "engine/CIndexBuffer.h"
#include "engine/CSimpleMemoryPool.h"  "engine/CEngineResourceManager.h"  "engine/CShaderResource.h"
class CDisplayManager { public: unsigned long GetPixelShaderVersion() const; };   // 0x009BEE00 unless the header gains it
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; extern CDisplayManager* GPDisplayManager; }
class CEngineClothRenderer : public CEnginePrimitiveRenderUtil { ... members of DOSSIER sec. 4 ...; virtual void FrameStart(); };
```
Class needs a virtual dtor (vtable slot 0 = ??_G 0x00BB6710) and FrameStart (slot 1); no other virtuals. Flags: none (no /EHsc).
