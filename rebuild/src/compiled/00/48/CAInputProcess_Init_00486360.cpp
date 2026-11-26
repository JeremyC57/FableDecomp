// CAInputProcess::Init() -- nothing to set up: the default (empty) Init
// Leaf: ?Init@CAInputProcess@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00486360, 1 byte: `ret`; reached only through vftable slot 2.
// Identity: FableWin.pdb ?Init@CAInputProcess@@UAEXXZ; CAInputProcess vftable 0x0129CA10 slot 2 (RTTI .?AVCAInputProcess@@), inherited by CInputProcessGameBase 0x0125BE98 and 42 family tables.
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CAInputProcess_Init@@YIXXZ (no virtual on the shared header); respelt onto engine/CAInputProcess.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CAInputProcess.h"

void CAInputProcess::Init()
{
}
