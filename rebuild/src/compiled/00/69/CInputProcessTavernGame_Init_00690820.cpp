// CInputProcessTavernGame::Init() -- nothing to set up: the default (empty) Init
// Leaf: ?Init@CInputProcessTavernGame@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690820, 1 byte: `ret`; reached only through vftable slot 2.
// Identity: Ego_r.pdb / FableWin.pdb ?Init@CInputProcessTavernGame@@UAEXXZ; retail vftable 0x01237CE8 slot 2 (RTTI .?AVCInputProcessTavernGame@@).
// NF477 (r467d / r467b, 2026-11-26): was the stand-in ?CInputProcessTavernGame_Init@@YIXXZ (no virtual on the shared header); respelt onto engine/CInputProcessTavernGame.h
//   FABLE_RETAIL_INPUTPROCESS_VTABLE (the family's retail virtual list, declaration only), so the vftables 0x00488D10 emits
//   name this row at the slot. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessTavernGame.h"

void CInputProcessTavernGame::Init()
{
}
