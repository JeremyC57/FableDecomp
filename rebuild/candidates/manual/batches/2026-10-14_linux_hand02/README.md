# Batch `2026-10-14_linux_hand02` — hand-authored C3DMesh2 / C3DMeshFile*Chunk functions: 40 bodies (37 landed)

Authored by a delegated hand-authoring pass (compiler in the loop, Wine lane) over the first chunk
of class-labelled open functions of 17–96 bytes (`C3DMesh2 .. C3DMeshFilePartitionChunk`): chunk
constructors (`C3DMeshFileChunk(const char* chunkType)` @ 0x00AA8360 with the four-letter chunk
ids), `WriteToFile` through `CDataOutputStream::WriteULong/Long/Float/Bool`, `GetChunkInfo`
(`"Name: " + GetName()` through the `__fastcall` CCharString `operator+` with the hidden result in
ecx), `CreateNewInstance` factories (`return RegisterNewChunk(new T)`), an aligned 4x4 transpose,
mesh buffer lock/upload helpers and two block copies to/from globals.

The Ego_r PDB headers put `C3DMeshFileChunk` at 0x34 bytes where retail has 0x2c, so every derived
member sits 8 bytes lower in retail; the sources model the retail layout locally with the PDB
member names and say so, and only the two headers that agree are `#include`d. Three bodies differ
only in instruction scheduling (two matrix-command record constructors, the `CThingPlayerCreature`
constructor's `add esp,4` placement) and are kept in the lane scratch with their best source.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
