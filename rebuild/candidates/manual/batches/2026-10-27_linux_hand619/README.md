# Chunk 619 -- 1 of 10 landed (2026-10-31)

Cut from the parked-row pool (`$S/w30/pick.py` rank order). Landed via `coordinator/land_chunk.sh 619` (vland --land 1/1
RELOCATION_MATCH + PASS, multi-check 0): CPlane::GetIntersectionWithTriangle 0x00A42470 (750 bytes; lever: `const C3DVector& n =
Normal;` alias flips the x87 load order so the argument component loads first). Nine rows stay DIFFER in
`pending/2026-10-27_linux_hand619/differ/` (008c4fd0 723v722, 0066a1e0, 005542f0, 009ce5e0, 0047e651, 005d8978, 0055d180,
007b0360, 00a18b50 -- frame-slot rotation / register colouring / one scheduling swap residues, each with 4+ grades across the
sweeps). 6 NAME_FIXES + 4 label-review rows queued (prefix c619). The agent instance was cut off once at 14:52 UTC and resumed
from its saved probes; the match came from the first instance and was re-verified. Labels: ghidra_out/labels_chunk619_2026-10-27.tsv.
