# Chunk 626 -- 3 of 10 landed + 2 extras (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 626` (vland --land 5/5 RELOCATION_MATCH + PASS, multi-check 0):
CTCTrapBase::UpdateTriggerThings 0x007881AD (catalog label CTCCombatBase_UpdateEngageable was wrong; a C2DCoordI temporary built
inside the call), MicroThread::SwitchToMicroThread 0x009D87F0 (the active-thread global as a file static whose address is never
taken; PDB unmangled helpers RestoreStack/PreserveStack as static inline), CCreatureActionBase::UpdateActionMovement 0x00692AA0
(virtual slots read from the retail vtable against the Ego_r order; bool IsMovementUsingPhysics() passed to SetForcedToMovePos),
plus the two chunk-624 extras with tests: std::__uninitialized_fill_n<CIntelligentPointer<CThingAICreature>*> 0x007AF940 (chunk 622
row) and CGraphicDataBank::UpdateAvailableMemory 0x009FD3A0 (retry1 row; Leaf corrected). Seven rows stay DIFFER in
`pending/2026-10-27_linux_hand626/differ/` (esi/edi swaps 008b9dd0 / 00954740, push scheduling 009fd700 / 00c2cbd0 -- the libpk
CEventScript ctor, not a quest script --, one lea 0095d4c0 with an ungraded idea in NOTES.md, vtable-load scheduling 008e8ff0,
CGameEvent zero immediates 00688260). 14 NAME_FIXES (CTCBase::PeekThingSearchTools 0x00686D90 / GetThingSearchTools 0x00686D80,
CThing::GetPos 0x004C73D0, C2DBoxF::ToC2DBoxI 0x00A55A30, the GetAllThingsInRoughArea<...CFilter_Bind2...> spelling 0x0095DB90,
NCreatureMode::GetStrafingAnimationName 0x008E8E50, GetCurrentMode 0x00633BE0, IsToggleZTarget 0x00445B80, AddTC 0x004C9D60, ...)
+ 3 review rows queued (prefix c626; the landed CGameEvent ctor 0x004159B0 uses a fake __fastcall model). Labels:
ghidra_out/labels_chunk626_2026-10-27.tsv.
