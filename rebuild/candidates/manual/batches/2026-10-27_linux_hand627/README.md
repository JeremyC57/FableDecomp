# Chunk 627 -- 4 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 627` (vland --land 4/4 RELOCATION_MATCH + PASS, multi-check 0):
CVsyncCallbackTimer::GetInfo 0x009F7050 (/Od row: `#pragma optimize("", off)`; at /Od VC7.1 lays locals out by name hash, so the
pointer local is named pRecord to get retail's frame), CWorld::ResetToFrontEnd 0x0049DCC0 (Init() called in each arm with a named
component local; ctors take CGame&), NGraphicAppearance::CTCGraphicAppearance::OnCreate 0x004BFC50 (vtable slot 4; GetInterface
into a local out pointer, inline GetBaseDef; CDefPointeeBase / CDefPointer / CDefinitionManager from the shared engine headers --
the CDefPointer header keeps the bytes because the dtor's dead `Object = 0` store is dropped), CTCShop::IsShopKeeperNearerThan
0x007723F0 (`return a && b && c` gives retail's callee-saved map; squared length of a C2DVector temporary for the x87 order).
Before landing the agent replaced its descriptive names: 0x013B8394 binds the established 0x0088F610 spelling, 0x00991620 the
landed body's compiled symbol (placeholder). Six rows stay DIFFER in `pending/2026-10-27_linux_hand627/differ/` (00994a80
CDataInputStream::SetPosition 142v141 register choice, 00978b90 / 00beeda0 STLport heap helpers by-value copy scheduling, 0085fbc0
NCreatureMode::IsAbleToBeEngagedInCombat return-false block placement, 00492ed1 temp slot, 00475dfa esi/edi). 12 NAME_FIXES + 4
review rows queued (prefix c627) incl. two OWNER DECISIONS: canonical spellings for the globals 0x013B83D0 (CGame object, three
landed spellings) and 0x013B8394 (sound manager, ~10 landed spellings). Labels: ghidra_out/labels_chunk627_2026-10-27.tsv.
