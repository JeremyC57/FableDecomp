// CSalRenderDisable::CSalRenderDisable() -- constructor of the GameCODA plain-gain ("HRTF disabled") renderer: the CSalRender base (0x00C6E6A0),
//   own vtable 0x012BEDB8, then the current / new parameter pointers aimed at the two ParamsDisable records it owns
// Leaf: ??0CSalRenderDisable@@QAE@XZ
// Flags: /Ob1 (R-NF444-1, the render TUs)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E760, 30 bytes (+ int3 padding). Callers: 0x00C550B3 / 0x00C55113 (the SIMD constructors).
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderDisable.obj CodeView class / method record + CODAApi.lib Release/SalRenderDisable.obj body): public CSalRenderDisable() of LF_CLASS CSalRenderDisable : public CSalRender, 11 instructions;
//   (2) retail 0x00C6E760 is 11 instructions with the same shape (base call, vptr, +0x30 = &+0x28, +0x34 = &+0x2C), and the SDK Release
//   SalSw3DVoice.obj ??0CSalRenderDisableSSE2 / MMX COMDATs call ??0CSalRenderDisable where retail 0x00C550B0 / 0x00C55110 call 0x00C6E760.
//   Was ??0CVtable_012bedb8@@QAE@XZ (descriptive, vtable-named).
// Shape: ParamsDisable has no constructor, so only the two pointer members are initialised (init-list order); VC7.1 schedules the vtable
//   store between the two pointer stores, as retail.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderDisable.h"   // CSalRenderDisable (vtable 0x012BEDB8, 0x38 bytes) : CSalRender (constructor 0x00C6E6A0)

CSalRenderDisable::CSalRenderDisable()
    : m_pParamsCurrent(&m_paramsA), m_pParamsNew(&m_paramsB)
{
}
