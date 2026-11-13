# Chunk 2035 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x006FDD60) The x87 operand orders inside an expanded C3DVector::Normalise on a local vector (square order, `X*r` / `Y*r`, i.e.
  `fst [r] ; fmulp st(1)` versus `fstp [r] ; fmul [r]` at 0x006FE22E) follow the c2 candidate ids of the vector's fields relative to
  the Normalise's own `one_on_magnitude`. A DIRECT field reference anywhere in the function (`total.X = 0.0f`, `total.Y = 0.0f`,
  `total.X += ...`) creates that field's candidate early; fields reached only through inlined `this->` members get theirs after
  inlining. Outer dead `long` locals (r3004 sweep) moved nothing here. What fixed it naturally: reading the list element through a
  named `const C3DVector& pos = *it;` in the loop (no instruction change elsewhere). Probe each candidate with ALL the deciding
  instructions in view: grade 5 fixed one order and broke two others the probe indicators did not cover.
- (0x006FDD60) FableWin COMDAT order Normalise 0x01ACDFD0, GFSqr 0x01ACE080, GFFastOneOverSqrt 0x01ACE0A0 proves the GFSqr Normalise
  body (FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER); the default GetSquaredMagnitude body squares X, Y, Z where retail has X, Z, Y.
- (0x006FDD60) `x += (fixed - x) * k;` on members re-reads the member (`fld fixed ; fsub x ; fmul k ; fadd x`); GFGetInterpolatedValue
  (by-value floats) loads x once and keeps it on the stack.
- (0x006FDD60) `if (a >= 0.0f) { A } else { B }` can come out as `test ah,1 ; je A` with B falling through and A laid out after the
  join's next block; `if (a < 0.0f) { B } else { A }` gives the IEEE `test ah,5 ; jp` form instead.
- (0x006FDD60) Two retail exits that each copy a vector into the camera with different register rotations (eax/ecx/edx versus
  edx/eax/ecx) came from ONE `InternalCam.MoveTo(new_pos)` after an `if (!LOS) { ...; new_pos = collision + (0,0,1.5f); }`: VC7.1
  duplicates the tail into both exits; two explicit MoveTo calls give a different rotation and keep the temporary's Y in a register.
- (0x006FDD60) A same-TU noinline body of the landed private callee GetRotationAngle (it only reads the two vectors Update passes)
  lets those vectors share frame words with later temporaries and moves an `empty()` test up the schedule (0x006FDFEE).
- (0x006FDD60) Catalog reservation lengths can run over a TU's unreferenced STLport COMDAT run (0x006FE3E0..0x006FE53F: 22 list
  helpers); the run itself names the list operations the TU's rows use (empty, front, begin, end, iterator ops).

Review at landing:
- 0x006FDD60 needs the overlay opt-ins FABLE_RETAIL_CGAMECAMERABASE_UPDATE (engine/CGameCameraBase.h) and
  FABLE_RETAIL_CWORLDMAP_FINDLASTUNBLOCKEDGROUND (engine/CWorldMap.h) merged into rebuild/include; the row includes engine/CWorldMap.h
  first so the overlay copy wins.
- NAME_FIXES.tsv: 0x006FD4F0 landed definer is public (QAE) but both PDBs say private (AAE) -- the row binds the AAE spelling; respell
  the definer. 0x006FDA20 label. 0x0049D9D0 GetCurrentPlayer binding (site 0x0095F9A0).
- 0x00415FD3 DivideByGameSpeed MULTINAME conflict is still open (row uses the definer's global spelling).
- 0x006ACA30 (second row of this chunk) was not started.
