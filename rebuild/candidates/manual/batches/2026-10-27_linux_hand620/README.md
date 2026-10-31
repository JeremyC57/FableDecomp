# Chunk 620 -- 0 of 10 landed (2026-10-31)

Cut from the parked-row pool. No row byte-matched; all ten compile to the retail length and are parked in
`pending/2026-10-27_linux_hand620/differ/` with register-colouring / frame-slot / store-order residues (009fb750 ebx-ebp swap,
00489d40 three C3DVector slots, 00a17f80 and 00a177a0 scheduling twins, 005726a0 and 0058f649 `Method = 1` store before the
vtable load, 0085c470 eax/ecx/edx in the last block, 0047e9ae the push ecx / fsubp swap shared with 0x0047E651 and 0x0047F30F,
0057df85 three CCharString slots, 00622a00 zero kept in ebx). Every row has more than four grades across passes. The agent
instance was cut off at 14:52 UTC and resumed from its saved probes. 3 NAME_FIXES (RecheckAvailabilities 0x005726A0,
NCameraModes::CMoveOnCylinder::Update 0x0085C470, GetSingleInstanceCollisionSet 0x00A6CBB0 -- the latter is a landed misspelling)
+ 2 review rows queued (prefix c620). Labels: ghidra_out/labels_chunk620_2026-10-27.tsv.
