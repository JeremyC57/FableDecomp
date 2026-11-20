// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CNavConnectorNode>) the counted pointer's per-type deleter: `delete (T*)data`,
//   null-guarded, through the non-virtual ~CNavConnectorNode (0x00A780B0) and operator delete (0x00BFE9BC).
// Leaf: ?DeleteData@?$CCountedPointer@VCNavConnectorNode@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A82FA0, 23 bytes: `push esi; mov esi,ecx; test esi,esi; je; call ~CNavConnectorNode; push esi;
//   call operator delete; add esp,4; pop esi; ret` -- a static __fastcall member (data in ecx); only its address is used: it is stored as
//   the count block's DeleteFunc of the CNavConnectorNode the map-edge rows create (`mov [eax+4],0A82FA0h` at 0x00A794F4, 0x00A793D0 /
//   0x00A79680).
// Names (landing navedge, 2026-11-20; was the made-up extern "C" OnDie stand-in under the catalog label CTCMapwho::OnDie): Ego_r.pdb
//   ?DeleteData@?$CCountedPointer@VCNavConnectorNode@@@@SIXPAX@Z (0x00BC4940) + the body and its one use (the two map-edge rows bind this
//   spelling). Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on the shared engine/CNavConnectorNode.h; the
//   explicit instantiation emits the COMDAT (the landed 0x00BB6330 pattern).
#include "engine/CNavConnectorNode.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CNavConnectorNode>::DeleteData(void*);
