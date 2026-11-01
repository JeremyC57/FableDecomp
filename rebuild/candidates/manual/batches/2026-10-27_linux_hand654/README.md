# Chunk 654 -- 3 of 10 landed (2026-11-01, increased-attempts + retail-fidelity brief; third concurrent agent)

Cut 2026-11-01 from the fixable bucket (residues 8-9 bytes). Landed via `coordinator/land_chunk.sh 654` (vland --land 3/3
RELOCATION_MATCH + PASS, multi-check 0): 0x00A6BCD0 `std::vector<CIVCountedPointer<CManagedTextureData> >::_M_fill_insert` (shared
CIVCountedPointeeBase header + real inline operator=; 2 probes), 0x0065A8EE `CTCVillage::IsTimeToGoToSchool` (PDB `GetTC<T>(T**)` form,
block-scoped out locals; 30 probes), 0x00670D50 `CThingCreatureDef::Validate` (implicit "DEFAULT" -> CCharString temporary order; real
members PrimaryMeleeWeaponDef +0xd8 / Graphic +0x100; 13 probes).
Parked in `pending/2026-10-27_linux_hand654/differ/` (STATUS.tsv has residue + probe counts): 007b8b10 GetEatingPoint 408v417 (ebp/ebx
tie), 00935e40 PutOutFire 476v468 (identical sibling 0x009363D0: land both together), 0078f8f0 CancelForCutscene 506v505 (caster spill),
00d38d20 RunVillagerComments 529v526 (merged exit order; its 25.0f is a squared distance of 5 -- 0x00CBE583 is GetSquaredDistanceBetweenThings),
00bc7660 Render2DPrimitive 695v692 (frame-data pointer register; FableWin inline GetTexture cut 86->39 lines), 0095ef80 IsRunnable
1536v1528 (frame 0x38 v 0x30 slot sharing), 00533d30 CComponent::GetSize 1723v1750 (branch layout; frame now 0x94 = retail).
13 NAME_FIXES queued (c654). Labels: ghidra_out/labels_chunk654_2026-10-27.tsv (12). Review at landing: 0x00670D50 same-file GetSubDef
stand-in body (the real template once CDefClassBase.h carries it); `__forceinline` on C3DVector operator- / GetLength and GetTC in
0x0065A8EE (not PDB-attested; the re-author pass with 0x00B677D0 and the PerformAction family); the globals 0x013BAC30/34/48 keep bound
placeholder spellings (no PDB name). Lessons in GOTCHAS (1 line) and LESSONS.md.
