// CInputProcessBoastUI::Init() -- nothing to set up: the default (empty) Init
// Leaf: ?Init@CInputProcessBoastUI@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690E60, 1 byte: `ret`; reached only through vftable slot 2.
// Identity: Ego_r.pdb / FableWin.pdb ?Init@CInputProcessBoastUI@@UAEXXZ; retail vftable 0x01237DB0 slot 2 (RTTI .?AVCInputProcessBoastUI@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CInputProcessBoastUI_Init@@YIXXZ (no virtual on the shared header); respelt onto engine/CInputProcessBoastUI.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessBoastUI.h"

void CInputProcessBoastUI::Init()
{
}
