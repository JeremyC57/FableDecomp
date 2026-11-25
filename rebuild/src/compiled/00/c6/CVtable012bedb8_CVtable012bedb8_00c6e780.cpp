// CSalRenderDisable::~CSalRenderDisable() -- empty virtual destructor of the GameCODA plain-gain renderer: own vtable 0x012BEDB8, then the
//   base CSalRender destructor (tail jump to 0x00C6E630)
// Leaf: ??1CSalRenderDisable@@UAE@XZ
// Flags: /Ob1 (R-NF444-1, the render TUs)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E780, 11 bytes: `mov dword ptr [ecx],012BEDB8h ; jmp 0x00C6E630`. Callers: the deleting destructor
//   0x00C6ECB0 and the SIMD classes' implicit destructors 0x00C55540 / 0x00C555D0 (`jmp 0x00C6E780`).
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderDisable.obj CodeView class / method record + CODAApi.lib Release/SalRenderDisable.obj body): virtual ~CSalRenderDisable(), 2 instructions; (2) retail 0x00C6E780 is the same 2 instructions
//   (vptr store + jump to the base destructor), storing the vtable the constructor 0x00C6E760 stores. Was ??1CVtable_012bedb8@@UAE@XZ.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderDisable.h"   // CSalRenderDisable (vtable 0x012BEDB8) : CSalRender (destructor 0x00C6E630)

CSalRenderDisable::~CSalRenderDisable()
{
}
