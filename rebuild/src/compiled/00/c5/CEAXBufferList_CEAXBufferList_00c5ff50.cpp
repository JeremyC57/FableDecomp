// CEAXBufferList::CEAXBufferList() -- GameCODA 2.10 EAX listener buffer-list node: only the vtable store (0x012B9338)
// Leaf: ??0CEAXBufferList@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5FF50, 9 bytes: `mov eax,ecx ; mov dword ptr [eax],0x012B9338 ; ret`.
// Name (owner decision 2026-11-03 D3): GameCODA's CEAXBufferList (Codex pass 12 CEAXListener.h: lpNext +4, lpEAXBuffer +8; pass 11
//   CEaxSourceNode); CEAXListener::AddSource 0x00C609A0 allocates the 0xc-byte node and calls this constructor. Earlier label:
//   CVtable_012b9338 (tiny-body shape).
#include "engine/CEAXListener.h"

CEAXBufferList::CEAXBufferList()
{
}
