# Chunk 521 (agent521, 2026-10-28): 10 of 20 re-queued single functions landed (128-369 bytes)

Re-queue chunk of 20 rows of 128-369 bytes (0x00433DF0..0x004F9C07): STLport vector / heap / sort instantiations, player
manager, subtitle renderer, graphic appearance and def-persistence templates. One Opus 5.5 agent at high effort.

- **Landed:** 10 rows with `vland.py --land`, all RELOCATION_MATCH (0x00433DF0 needs /Gr), every test PASS under g++ and
  VC7.1; no RELOC_IDENTITY, CONFLICT or DUPLICATE. 0x0044A1A0 and 0x0048F939 use the shared CPlayer / CPlayerManager /
  CSubtitleRenderer headers.
- **Parked (10, pending/2026-10-27_linux_hand521/differ/):** nine `CPersistContext::TransferVectorOfSubComponents<T>`
  rows one byte short (254v255 / 244v245): the inline CDataInputStream::Read computes the new position in eax
  (`add eax,4; xor edx,edx`) where retail uses edx (`lea edx,[eax+4]; xor eax,eax`), a register tie tracking the element
  size that ~20 source forms and the QFE compiler did not move; 0x0049ED80 CWorld::ConsoleSetFreeCameraLookVector
  (310v310, x87 operand order in the cross product).
- **Identities:** all 20 row names proven (PDB, element strides, RTTI, callers); probable 0x004E8212 `_Rb_tree::clear` and
  0x004EC9D3 `_Rb_tree::insert_unique`.

Name fixes (NAME_FIXES.tsv, 41 entries, all deferred to the naming pass): the TransferVectorOfSubComponents template
spelling (bound as a non-template Transfer or invented `...Vector` structs), CWideString ctor / dtor / `operator=`
(returns const reference), CPlayer::AddMode / RemoveMode take EPlayerMode, 0x00449970 GetMainPlayer returns a reference,
0x00416359 is GetPPlayerInterface, the `pair<float,CCameraTag>` `__adjust_heap` at 0x004D0660.

Label corrections: every catalog label in the chunk was behavioural, propagated or wrong (all nine CPersistContext rows
carried `<CAbilityButtonGuiDef>`; 0x004BD040 was Appearance_FloatToCoord; 0x004D16A0 `_Sort_heap<CDebugCone*>`).
