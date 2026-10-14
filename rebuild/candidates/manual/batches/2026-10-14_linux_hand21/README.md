# Batch `2026-10-14_linux_hand21` — delegated hand chunk 21 (CComponent .. CConsoleCommandBase): 34 bodies (34 landed)

Twenty-first delegated hand chunk of the Wine lane (`$S/w22/chunk21.tsv`, 40 small-module bodies
of 17–96 bytes authored by a background agent against the live VC7.1 compiler, re-proved through
the lander's own parity and behaviour checks): `NUISystem::CNavButton(long)`, `CTCRockTrollShield`,
`CTCCarriedActionUseTrophy`, `CAnimFilterOfGroup(const CCharString&)`, `CModeSitFloor`,
`CConsoleCommandBase(const CCharString&, const long&)`, the `CConsole` command-line and history
accessors (retail layout: maps 12 bytes + list sentinel, FrontCommandLine +0x1c, history +0x4c),
`CCompressorZlib` front ends (`DecompressBlock` → slot 7), `CVertexBufferBase()`,
`~CChunkedFileSuperChunk`, the RTTI-less tree `GetItemByIndex` (slot 9 of 0x012B2F0C) and two
def-class destructors reachable only from their deleting thunks.

Label corrections and retail-vs-PDB layout facts are in the batch sources. Parked (scheduling or
DDK-only shapes): 0x00436D30, 0x00A6A330, 0x00B2F760, 0x00B2C400, 0x00625160 / 0x0067C520 (the
implicit-destructor `??_G` family F73: the DDK keeps `??1` out of line).

All graded `RELOCATION_MATCH` / `MATCH` + `PASS` on 2026-10-14.
