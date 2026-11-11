# Chunk Q14 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Copies of `rebuild/include/engine/*.h` from repo HEAD. Every change sits behind a new `#ifdef FABLE_RETAIL_*` opt-in (or is a
new header); with no new define the preprocessed default path is byte-identical to HEAD.

| Header | Opt-in | What it adds | Used by |
|---|---|---|---|
| `engine/NCameraModes_CTools.h` | `FABLE_RETAIL_NCAMERAMODES_CTOOLS_POTENTIALFIELD` | includes C3DVector.h, forward-declares CThingGameObject; inline `PotentialField(pos, source, scale, r_prime)` = `(scale*0.1f)/((pos-source).GetMagnitude()-r_prime)` (Ego_r.pdb `?PotentialField@CTools@NCameraModes@@QBEMABVC3DVector@@0MM@Z`, inline everywhere in retail; body as the landed twin 0x00876E10) and the `PotentialFieldBufferObjects` declaration (0x00876CA0). No layout change. | **src/00876ca0.cpp (MATCH)** |
| `engine/CWorldMap.h` | `FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL` | declaration `C3DVector GetGroundFaceNormalAt(const C3DVector&) const; // 0x0063AB10` (Ego_r 0x006E7420, FableWin 0x024A4450). | differ/0066c900 |
| `engine/CRegionMinimap.h` | `FABLE_RETAIL_CREGIONMINIMAP_ILLUMINATION` | the +0x3C member as its PDB type `C3DVector IlluminationDirection` instead of three floats; static asserts follow. Same bytes and offsets. | differ/0066c900 |
| `engine/C3DVector.h` | `FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX` | `Dot` as running statements Z, Y, X (an `#elif` inside the existing FABLE_RETAIL_C3DVECTOR Dot choice). Body only. | differ/0066c900 |
| `engine/C3DVector.h` | `FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX` | `GetSquaredMagnitude` as `(Z*Z+Y*Y+X*X)` (an `#elif`). Body only. | differ/00a58790 |
| `engine/CCamera.h` | `FABLE_RETAIL_CCAMERA_MOVESIDEWAYS` | declaration `void MoveSideways(float); // 0x00A0C1B0`. | differ/00a0c1b0 |
| `engine/QuadricOptimiserTypes.h` | `FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART` | includes CMatrix3x4.h; declaration `CMatrix3x4 GetInverseMatrixPart() const;` (Ego_r 0x00C5F9C0, FableWin 0x034BF810; retail 0x00AC70F0). | differ/00ac70f0 |
| `engine/CSphere.h` | (new header) | `NBBBMaths::CSphere`: Radius +0x00, Base (C3DVector) +0x04, sizeof 0x10 (Ego_r.pdb); ctor and `GetFirstIntersectionWithFiniteLine(const C3DVector&, const C3DVector&, C3DVector&, C3DVector&) const` declarations. | differ/00a58790 |

The match (0x00876CA0) also needs these existing HEAD opt-ins: FABLE_RETAIL_CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING,
CTHINGPHYSICAL, CTHINGGAMEOBJECT, NCAMERAMODES_CTOOLS. The 0x00B80CE0 probe uses HEAD's FABLE_RETAIL_CMATRIX3X4 + _PREMULTIPLY
unchanged (the PreMultiply term-order probes were removed from the overlay; see NOTES.md).
