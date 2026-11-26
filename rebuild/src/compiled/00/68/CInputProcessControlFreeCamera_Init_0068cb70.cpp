// CInputProcessControlFreeCamera::Init() -- set the free-camera once-flag when the process is added
// Leaf: ?Init@CInputProcessControlFreeCamera@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068CB70; CInputProcessControlFreeCamera vftable slot 2 (RTTI; NF477 audit: one `.rdata` dword, no call).
// Identity: Ego_r.pdb + FableWin.pdb ?Init@CInputProcessControlFreeCamera@@UAEXXZ (public virtual, the CAInputProcess slot 2 override).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the stand-in ?Init@CInputProcessControlFreeCamera_Methods@@QAEXXZ; respelt onto
//   engine/CInputProcessControlFreeCamera.h FABLE_RETAIL_INPUTPROCESS_VTABLE (resolves T3 nf319 / nf323 "base class model lacks the virtual").
//   No body change; the flag is still the row's file-static (its retail global is not named here).
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessControlFreeCamera.h"
static char g_f=0;

void CInputProcessControlFreeCamera::Init(){ if(!g_f) g_f=1; }
