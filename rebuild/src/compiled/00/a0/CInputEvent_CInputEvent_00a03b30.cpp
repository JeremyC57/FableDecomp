// CInputEvent::~CInputEvent() -- the empty out-of-line destructor
// Leaf: ??1CInputEvent@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A03B30, 1 byte (`ret`), thiscall. Ego_r.pdb 0x00B6E5F0 / FableWin.pdb 0x0317B590
//   ??1CInputEvent@@QAE@XZ (public); Ego_r orders it right before ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ 0x00B6E600 as retail
//   orders 0x00A03B30 before CInputEvent::GetType 0x00A03B40; 45 landed callers (0x006286F0 on its event local, the CInputEventPackage
//   rows) bind this spelling here.
// NF139 (2026-11-20, q4299): moved off the C00a03b30::M placeholder onto engine/CInputEvent.h, which declares the destructor out of line.
#include "engine/CInputEvent.h"

CInputEvent::~CInputEvent()
{
}
