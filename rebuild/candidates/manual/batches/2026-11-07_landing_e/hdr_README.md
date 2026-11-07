# hdr/ overlay -- chunk 829

Both edits are opt-in `#ifdef` blocks appended to a fresh copy of the repo header; the default path is unchanged
(diffed against rebuild/include on hand-back: additions only, inside the new #ifdef).

## engine/C3DVector.h -- `FABLE_RETAIL_C3DVECTOR_INITIALISEFROMANGLES`
- Adds `const C3DVector& InitialiseFromAngles(float angleXY, float angleYZ, float length)` (inline, FABLE_VECTOR_INLINE).
- Evidence: FableWin.pdb `?InitialiseFromAngles@C3DVector@@QAEABV1@MMM@Z` (no out-of-line retail copy, so it is expanded);
  body as the landed local models at 0x00A0C450 / 0x00A14260; the turn-to-radian factor is the literal double
  6.2831854820251465 (pool 0x0124F2B8), which lets a constant 0.0f angleYZ fold to `fld qword 0.0 ; fcos` as retail
  does at 0x0068D3B4.
- The user must include <math.h> before the header (cos/sin).
- Used by: differ/0068cb90.cpp (CInputProcessControlFreeCamera::ProcessInput, parked DIFFER md 27).

## engine/CRenderManager2D.h -- `FABLE_RETAIL_CRENDERMANAGER2D_DRAW3DSCALEDTEXT`
- Adds the declaration `bool Draw3DScaledText(const C3DVector&, float, CWideString, const CFontBank&, const CRGBColour&, unsigned long);`
- Evidence: Ego_r.pdb / FableWin.pdb `?Draw3DScaledText@CRenderManager2D@@QAE_NABVC3DVector@@MVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z`,
  reloc_bindings target 0x009DBFF0; DoRender's track-name loop pushes flags, colour, font, a by-value CWideString built in
  its argument slot, the scale and the position.
- Used by: differ/00435530.cpp (CDisplayEngine::DoRender, parked).

## Includers
Opt-in only, so no landed includer can change. Checked anyway: g++ -E with and without the overlay on all 3255 direct
includers under rebuild/src: 3253 byte-identical; 2 differ only in a `__LINE__`-numbered static-assert typedef name
(0x00BBC460, 0x009DA9F0), both regraded with the overlay: RELOCATION_MATCH. See SIBLINGS.tsv.
