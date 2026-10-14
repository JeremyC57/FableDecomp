# Batch `2026-10-14_linux_hand00` — hand-authored C2DBoxF .. C3DAnimFileXSequenceChunk functions: 40 bodies (36 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the first chunk of
class-labelled open functions of 17–96 bytes: 2D box/vector/line maths (`Normalise`, `GetLowestY`,
`ToC2DCoordI`, `OnLeft`, `ValidateOrientationAndFlip` through an inline by-reference `Swap`),
Dinkumware `max`/`min` widened to double, the chunk constructors of the animation and mesh file
formats (`CChunkedFileSuperChunk(const char* chunkType)` @ 0x00AA8360 with "MVEC"/"ASEQ"/"AMSK"/
"PRIM" ids), `CreateNewInstance` factories through `InitNewInstance` (slot 10 @ 0x00AA8460), an
STLport `vector<CScriptThing>::push_back` and a script-module handle-registry factory.

Manifest labels corrected in the payload (several bodies were attributed to the wrong class or
method; the NOTES in the lane scratch list them). The Ego_r headers put `CChunkedFileSuperChunk`
at 0x34 bytes with 16-byte CArrays where retail has 0x2c with 12-byte CArrays, so the chunk rows
model the retail layout locally and say so. Four bodies differ only in instruction scheduling
(swapped X/Y loads, an x87 duplicate kept on the stack, a malloc cleanup placement, a `dec [esi]`
form of a counted release).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
