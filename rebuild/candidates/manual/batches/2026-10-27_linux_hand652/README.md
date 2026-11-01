# Chunk 652 -- 4 of 10 landed (2026-11-01, increased-attempts + retail-fidelity brief)

Cut 2026-11-01 from the fixable bucket (residues 4-5 bytes). Landed via `coordinator/land_chunk.sh 652` (vland --land 4/4
RELOCATION_MATCH + PASS, multi-check 0): 0x00845040 `CCreatureAction_GiveMoneyToThing::PerformAction` (landed shared model; inherited
`#pragma pack(1)` on C3DVector removed; 9 probes), 0x00761C00 `NShape::CShape::OnSerialise` (slot 2; real CPersistContext::Transfer<T>
members + the landed Transfer<float> body same-file; the anonymous-namespace CLOSED_STRING_NAME / LINE_STRING_NAME globals bound under our
TU hash `?A0xd1d0d6e9` -- the PDB lists only their demangled dynamic initializers, so the retail hash is unknown; 8 probes), 0x0072A090
`CTCActionUseScriptedHook::GlobalUpdate(bool)` (engine/CCountedPointer.h, FableWin inline CWorld::GetGameCamera / GetMessageEventManager,
SetPCameraPoint(CThing&), CTCSoundPlayer::PlayAttachedCriteria; 10 probes), 0x005FB140 `CTCInventoryMap::PanMap` (real STLport map; 8 probes).
At landing: 0x006BBF40 is CRegion::GetWorldMapOffset (0x006BBF70 GetNameGraphicOffset) -- manifest + landed caller 0x005FB8F0 respelled
(recheck RELOCATION_MATCH + PASS), old binding retired.
Parked in `pending/2026-10-27_linux_hand652/differ/` (STATUS.tsv has residue + probe counts; probe/ keeps the diagnostic sources):
009c4f20 CFileInstaller::ProcessAlways 490v490 -- RELOCATION_MATCH only with `volatile` on PCompletedRead (probe/009c4f20_diagV2.cpp);
OWNER DECISION: the PDB layout carries no qualifier and the purity rule bans `volatile`; the landed 0x009C42F0 already carries
`void* volatile` (purity debt). 006276f0 GetItemsListUI 1244v1260 -- RELOCATION_MATCH only with an invented same-file body for
ConstructItemCategoryList 0x00626E40 (probe/006276f0_p05.cpp): not landed, reconstruct 0x00626E40 (same TU, ~1.5 KB) first and cut the
pair together. 00e36750 DoCountDown 541v536 and 005fd5e0 ConstructRegionDescriptionQuests 975v982: block placement only. 007280c0
UpdateMorphSet 795v802 (leaf corrected to the private AAE; next: real CMorphSet / CMeshMorphSet). 00e96110 CM_QuestControl::Main
1360v1356 (~CTimer inlining sites; probe/00e96110_p11.cpp).
23 NAME_FIXES queued (c652; 0x004C7590 unresolved, bindings disagree). Labels: ghidra_out/labels_chunk652_2026-10-27.tsv (12).
Review: 34 landed PerformAction siblings + this row use `__forceinline` on HasTC / GetTC / PeekTC (no PDB attestation; same pass as 0x00B677D0).
