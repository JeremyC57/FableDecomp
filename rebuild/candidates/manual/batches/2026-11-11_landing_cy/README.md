# 2026-11-11 landing cy -- chunk Q14 (parked near misses, x87-order, Opus 5.5): 1 row

vland --land: 1 win (RELOCATION_MATCH), test PASS. bindings_multi_check is at its NullCoord baseline.
- **0x00876CA0 NCameraModes::CTools::PotentialFieldBufferObjects** (Ego_r.pdb 0x0085D8B0).
  - The return is spelled like the landed twin 0x00876E10: each component divided by the step global, which VC7.1 turns into
    one reciprocal multiply, Z first.
  - The object position is copied by value through the shared C3DVector copy constructor.
  - New opt-in FABLE_RETAIL_NCAMERAMODES_CTOOLS_POTENTIALFIELD.

**Headers**

Q14's overlay was based on 07c35dd9f. None of its seven headers changed since, so they were installed as-is. All defaults are the
same; QuadricOptimiserTypes.h only splits the CQuadric line to add a declaration.
- New: engine/CSphere.h.
- Differ-row opt-ins: CWORLDMAP_GROUNDFACENORMAL, CREGIONMINIMAP_ILLUMINATION, C3DVECTOR_DOT_ORDERED_ZYX,
  C3DVECTOR_SQUAREDMAGNITUDE_ZYX, CCAMERA_MOVESIDEWAYS and CQUADRIC_GETINVERSEMATRIXPART.

**Names**
- Queued as land1011cy: 0x0063AB10 is `?GetGroundFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z`. The landed 0x00723A10 spells it
  `?Helper_use@@YIXPAUCFlushGroundHelper@@@Z`.
- Labels: ghidra_out/labels_landing_2026-11-11cy.tsv.

The chunk record is in pending/2026-11-11_linux_handQ14 (39 near misses).
