// CSalRenderPan::~CSalRenderPan() -- empty virtual destructor of the GameCODA volume panner: own vtable 0x012BED90, then the base CSalRender
//   destructor (tail jump to 0x00C6E630)
// Leaf: ??1CSalRenderPan@@UAE@XZ
// Flags: /Ob1 (R-NF444-1, the render TUs)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6DA30, 11 bytes: `mov dword ptr [ecx],012BED90h ; jmp 0x00C6E630`. Callers: the deleting destructor
//   0x00C6E1D0 and the SIMD classes' implicit destructors 0x00C55510 / 0x00C555A0 (`jmp 0x00C6DA30`).
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderPan.obj CodeView class / method record + CODAApi.lib Release/SalRenderPan.obj body): virtual ~CSalRenderPan(), 2 instructions; (2) retail 0x00C6DA30 is the same 2 instructions (vptr
//   store + jump to the base destructor), storing the vtable the constructor 0x00C6D9D0 stores. Was ??1CVtable_012bed90@@UAE@XZ.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPan.h"   // CSalRenderPan (vtable 0x012BED90) : CSalRender (destructor 0x00C6E630)

CSalRenderPan::~CSalRenderPan()
{
}
